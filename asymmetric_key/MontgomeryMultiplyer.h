#pragma once
//MontgomeryMultiplyer:20130702
#include <iostream>
#include <cassert>
#include "BigNatural.h"
namespace cryptia {
namespace asymmetric_key {

class MontgomeryMultiplyer{
public:
    MontgomeryMultiplyer(const BigNatural& n) : 
			n_(n), 
			n_length_(CalcBitLength(n_)),
			squared_r_(CalcSquaredR(n_, n_length_)), 
			n_dash_(CalcNDash(n_, n_length_)){}
	
	auto Multiply(const BigNatural& a, const BigNatural& b)const -> BigNatural {
		const auto ma = MontgomeryReduction(a*squared_r_);
		const auto mb = MontgomeryReduction(b*squared_r_);
		const auto mc = MontgomeryReduction(ma*mb);
		return MontgomeryReduction(mc);
	}

	auto Exponential(const BigNatural& a, BigNatural k)const -> BigNatural {
		BigNatural ma = MontgomeryReduction(a*squared_r_);
		BigNatural mres = MontgomeryReduction(squared_r_);
		while(k != 0){
			if(k.IsOdd()){
				mres = MontgomeryReduction(mres*ma);
			}
			k >>= 1;	
			ma = MontgomeryReduction(ma*ma);
		}
		return MontgomeryReduction(mres);
	}

	friend auto operator<<(
			std::ostream& os, const MontgomeryMultiplyer& mm) -> std::ostream& {
		os << "n: " << mm.n_ << "\n" 
			<< "n_length: " << mm.n_length_ << "\n" 
			<< "r: " << (BigNatural(1) << mm.n_length_) << "\n" 
			<< "squared_r: " << mm.squared_r_ << "\n" 
			<< "n_dash: " << mm.n_dash_ << "\n"
			<< "(n*n_dash)%r: " << (mm.n_*mm.n_dash_)%(BigNatural(1) << mm.n_length_);
		return os;	
	}

private:
	auto MontgomeryReduction(const BigNatural& a)const -> BigNatural {
		return MontgomeryReduction(a, n_, n_length_, n_dash_);
	}

	static auto MontgomeryReduction(const BigNatural& a, const BigNatural& n, 
			unsigned int n_len, const BigNatural& n_dash) -> BigNatural {
		{
			BigNatural r(1);
			r <<= n_len;
			assert(n < r);
			assert(a < n*r);
		}

		BigNatural mask(1);
		mask <<= n_len;
		mask-=1;
		//TODO
		BigNatural res = (a+((a*n_dash)&mask)*n) >> n_len;
		return res >= n ? res-n : res;
	}

	static auto CalcBitLength(BigNatural n) -> unsigned int {
		unsigned int n_len(1);
		while(n > 1){
			n >>= 1;
			++n_len;	
		}
		return n_len;
	}

	static auto CalcSquaredR(const BigNatural& n, unsigned int n_len) -> BigNatural {
		BigNatural r(1);
		r <<= n_len;
		auto temp_r = r%n;
		return (temp_r*temp_r)%n;		
	}

	static auto CalcNDash(const BigNatural& n, unsigned int n_len) -> BigNatural {
		BigNatural t(0);
		BigNatural res(0);
		for(unsigned int i = 0; i < n_len; ++i){
			if(!t.IsOdd()){
				res += (BigNatural(1)<<i);
				t += n;
			}
			t >>= 1;
		}
		return res;
		/*
		BigNatural res(0);
		BigNatural r = BigNatural(1) << n_len;
		BigNatural t = 0;
		BigNatural i = 1;
		while(r > 1){
			if(t%2 == 0){
				t+=n;
				res += i;
			}
			t /= 2;
			r /= 2;
			i *= 2;
		}
		return res;
		*/
	}

	BigNatural n_;
	unsigned int n_length_;
	BigNatural squared_r_, n_dash_;
};

}}

