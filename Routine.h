#pragma once
//Routine:20130702
#include <iostream>
#include "BigInteger.h"

namespace cryptia
{

auto GreatestCommonDivisor(BigInteger u, BigInteger v) -> BigInteger {
	while(v != 0){
		BigInteger r = u % v;
		u = v;
		v = r;
	}
	return u;
}

auto FermatPrimalityTest(const BigInteger& num) -> bool {
	if(num == 2){ return true; }
	if(num < 2 || num%2 == 0){
		return false;	
	}
		
	return BigInteger::PowerModulate(2, num-1, num) == 1;
}

auto MillerRabinPrimalityTest(const BigInteger& num, int try_count) -> bool {
	/*
	if(num == 2){ return true; }
	if(num < 2 || num%2 == 0){
		return false;	
	}
	*/
	BigInteger d = num-1;
	while(true){
		auto p = BigInteger::DivideModulate(d, 2);
		if(p.second == 0){
			break;	
		}
		d = p.first;
	}
	auto one_trial = [&num, &d](const BigInteger& a) -> bool {
		if(BigInteger::PowerModulate(a, d, num) != 1){
			BigInteger t(1);
			while(t*d != num-1){
				if(BigInteger::PowerModulate(a, t*d, num) == num-1){
					return true;
				}
				t *= 2;
			}
			return false;
		}
		return true;
	};
	for(int i = 1; i <= try_count; ++i){
		if(!one_trial(BigInteger(i)%(num-2)+1)){
			return false;	
		}
	}
	return true;
}

}

