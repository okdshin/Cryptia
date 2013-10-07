#pragma once
//BasicBigNatural:20130710
#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <cassert>
#include "../FundamentalTypes.h"

namespace cryptia{
namespace asymmetric_key{

using BaseType = uint32_t;

class BasicBigNatural{
public:
	using FigureList = std::vector<BaseType>;

	BasicBigNatural(BaseType num);
	explicit BasicBigNatural(const ByteArray& byte_array){
		assert(byte_array.size()%2 == 0);
		//static_assert(sizeof(BASE) >= 32, "implemented under int32_t");
		for(unsigned int i = 0; i < byte_array.size(); i += 2){
			figure_list_.push_back(
				static_cast<BaseType>(byte_array.at(i))
				| (static_cast<BaseType>(byte_array.at(i+1)) << 8)
			);	
		}	
	}
    explicit BasicBigNatural(const std::string& num_str);

	auto Output(std::ostream& os)const -> void;
	auto ToByteArray(const BigNatural& num) -> ByteArray {
		ByteArray res;
		for(int i = figure_list_.size()-1; i >= 0; --i){
			auto figure = figure_list_.at(i);
			res.push_back(static_cast<uint8_t>(figure & 0xff));
			res.push_back(static_cast<uint8_t>((figure >> 8) & 0xff));
		}
		return res;	
	}

	auto operator==(const BasicBigNatural& right)const -> bool;
	auto operator!=(const BasicBigNatural& right)const -> bool;
	auto operator<(const BasicBigNatural& right)const -> bool;
	auto operator>(const BasicBigNatural& right)const -> bool;
	auto operator>=(const BasicBigNatural& right)const -> bool;
	auto operator<=(const BasicBigNatural& right)const -> bool;

	auto operator+=(BasicBigNatural right) -> BasicBigNatural&;
	auto operator-=(BasicBigNatural right) -> BasicBigNatural&;
	auto operator*=(const BasicBigNatural& right) -> BasicBigNatural&;
	auto operator/=(const BasicBigNatural& right) -> BasicBigNatural&;
	auto operator%=(const BasicBigNatural& right) -> BasicBigNatural&;
	
	auto ShiftFigureLeft(BaseType shift_num) -> void;
	auto ShiftFigureRight(BaseType shift_num) -> void;
	
	auto operator+(BasicBigNatural right)const -> BasicBigNatural;
	auto operator-(BasicBigNatural right)const -> BasicBigNatural;
	auto operator*(BasicBigNatural right)const -> BasicBigNatural;
	auto operator/(BasicBigNatural right)const -> BasicBigNatural;
	auto operator%(BasicBigNatural right)const -> BasicBigNatural;

	static auto DivideModulate(const BasicBigNatural& left, const BasicBigNatural& right) 
			-> std::pair<BasicBigNatural, BasicBigNatural>;

	static auto Power(BasicBigNatural base, BaseType exponent) -> BasicBigNatural;
	static auto Power(BasicBigNatural base, BasicBigNatural exponent) -> BasicBigNatural;

	static auto PowerModulate(BasicBigNatural base, BaseType exponent, const BasicBigNatural& n) -> BasicBigNatural;
	static auto PowerModulate(BasicBigNatural base, BasicBigNatural exponent, const BasicBigNatural& n) -> BasicBigNatural;
	

	friend auto operator>>(std::istream& is, BasicBigNatural& val) -> std::istream& {
		std::string num_str;
		is >> num_str;
		val = BasicBigNatural(num_str);
		return is;
	}

	friend auto operator<<(std::ostream& os, const BasicBigNatural& val) -> std::ostream& {
	/*
		BasicBigNatural<10> res(0);
		BasicBigNatural<10> base(1);
		for(auto figure : val.figure_list_){
			res += BasicBigNatural<10>(figure) * base;
			base *= BasicBigNatural<10>(val.BASE_);
		}
		for(int i = res.figure_list_.size()-1; i >= 0; --i){
			os << res.figure_list_[i];
		}
	*/
		return os;
	}
private:
	static const BaseType BASE = 65535;
	auto Normalize() -> void;

	FigureList figure_list_;
};

auto BasicBigNatural::ShiftFigureLeft(BaseType shift_num) -> void {
	figure_list_.insert(figure_list_.begin(), shift_num, 0);
	return;
}

auto BasicBigNatural::ShiftFigureRight(BaseType shift_num) -> void {	
	if(figure_list_.size() <= shift_num){
		*this = BasicBigNatural(0);
		return;
	}
	figure_list_.erase(figure_list_.begin(), figure_list_.begin()+shift_num);	
	return;
}

auto BasicBigNatural::Normalize() -> void {
	auto iter = figure_list_.end()-1; 
	while(!(*iter) && iter != figure_list_.begin()){
		--iter;
	}
	figure_list_.erase(iter+1, figure_list_.end());
}


auto BasicBigNatural::DivideModulate(const BasicBigNatural& left, 
		const BasicBigNatural& right) -> std::pair<BasicBigNatural, BasicBigNatural> {
	BasicBigNatural x(right), q(0), r(0);
	for(int i = left.figure_list_.size()-1; i >= 0; --i){
		r = r*BasicBigNatural(BASE)+left.figure_list_[i];
		BaseType smaller(0), bigger(BASE);
		if(r >= x){
			while(smaller+1 < bigger){
				BaseType mid = (smaller + bigger) / 2;
				if(x*mid > r){
					bigger = mid;
				}
				else {
					smaller = mid;
				}
			}
			r -= x*smaller;
		}
		q.figure_list_.push_back(smaller);
	}
	std::reverse(q.figure_list_.begin(), q.figure_list_.end());
	q.Normalize();
	r.Normalize();
	return std::make_pair(q, r);
}


auto BasicBigNatural::Power(BasicBigNatural base, BaseType exponent) -> BasicBigNatural {
	BasicBigNatural res(1);
	while(exponent > 0){
		if(exponent&1){//IsOdd
			res *= base;
		}
		exponent >>= 1;
		base *= base;
	}
	return res;
}

/*

auto BasicBigNatural::Power(BasicBigNatural base, BasicBigNatural exponent) -> BasicBigNatural {
	BasicBigNatural res(1);
	while(exponent != 0){
		if((exponent.figure_list_.front() != 0)){
			res *= Power(base, exponent.figure_list_.front());	
		}
		exponent.ShiftFigureRight(1);
		base = Power(base, BASE);
	}
	return res;
}
*/

auto BasicBigNatural::Power(BasicBigNatural base, BasicBigNatural exponent) -> BasicBigNatural {
	BasicBigNatural res(1);
	while(exponent > 0){
		auto p = DivideModulate(exponent, 2);
		if(p.second == 1){
			res *= base;
		}
		exponent = p.first;
		base *= base;
	}
	return res;
}


auto BasicBigNatural::PowerModulate(BasicBigNatural base, 
		BaseType exponent, const BasicBigNatural& n) -> BasicBigNatural {
	BasicBigNatural res(1);
	while(exponent > 0){
		if(exponent&1){//IsOdd
			res *= base;
		}
		exponent >>= 1;
		base *= base;
		base %= n;
	}
	return res%n;
}


auto BasicBigNatural::PowerModulate(BasicBigNatural base, 
		BasicBigNatural exponent, const BasicBigNatural& n) -> BasicBigNatural {
	BasicBigNatural res(1);
	while(exponent > 0){
		auto p = DivideModulate(exponent, 2);
		if(p.second == 1){
			res *= base;
		}
		exponent = p.first;
		base *= base;
		base %= n;
	}
	return res%n;
}


BasicBigNatural::BasicBigNatural(BaseType num) : figure_list_() {
	figure_list_.push_back(abs(num));
	while(figure_list_.back() >= BASE){
		auto temp = figure_list_.back();
		figure_list_.back() %= BASE;
		figure_list_.push_back(temp/BASE);
	}
}


BasicBigNatural::BasicBigNatural(const std::string& num_str) : figure_list_() {
	int begin = 0;
	BasicBigNatural dec_figure(1);
	for(int i = num_str.size()-1; i >= begin; --i){
		const char c = num_str[i]; 
		int x = c - '0';
		if(x < 0 || 9 < x){
			std::cerr << "ParseError" << num_str << std::endl;	
			exit(1);
		}
		*this += dec_figure*BasicBigNatural(x);
		dec_figure *= 10;
	}	
}


auto BasicBigNatural::Output(std::ostream& os)const -> void {
	os << "{";
	for(int i = figure_list_.size()-1; i >= 0; --i){
		os << figure_list_[i];
		if(i != 0){
			os << " ";
		}
	}
	os << "}";
}


auto BasicBigNatural::operator==(const BasicBigNatural& right)const -> bool {
	if(figure_list_.size() != right.figure_list_.size()){
		return false;	
	}
	for(unsigned int i = 0; i < figure_list_.size(); ++i){
		if(figure_list_[i] != right.figure_list_[i]){
			return false;
		}
	}
	return true;
}


auto BasicBigNatural::operator!=(const BasicBigNatural& right)const -> bool {
	return !((*this) == right);
}


auto BasicBigNatural::operator<(const BasicBigNatural& right)const -> bool {
	if(figure_list_.size() < right.figure_list_.size()){
		return true;
	}
	if(figure_list_.size() > right.figure_list_.size()){
		return false;
	}
	for(int i = figure_list_.size()-1; i >= 0; --i){
		if(figure_list_[i] > right.figure_list_[i]){
			return false;
		}
		if(figure_list_[i] < right.figure_list_[i]){
			return true;
		}
	}
	return false;
}


auto BasicBigNatural::operator>(const BasicBigNatural& right)const -> bool {
	return right < (*this);	
}


auto BasicBigNatural::operator>=(const BasicBigNatural& right)const -> bool {
	return (*this) > right || (*this) == right;	
}


auto BasicBigNatural::operator<=(const BasicBigNatural& right)const -> bool {
	return (*this) < right || (*this) == right;	
}


auto BasicBigNatural::operator-=(BasicBigNatural right) -> BasicBigNatural& {
	if(*this < right){
		std::swap(*this, right);
	}
	const auto size = 
		std::max(figure_list_.size(), right.figure_list_.size());
	figure_list_.resize(size);
	right.figure_list_.resize(size);
	for(unsigned int i = 0; i < size; ++i){
		if(figure_list_[i] >= right.figure_list_[i]){
			figure_list_[i] -= right.figure_list_[i];
		}
		else {
			figure_list_[i] += BASE;
			figure_list_[i] -= right.figure_list_[i];
			for(unsigned int j = i+1; j < size; ++j){
				if(figure_list_[j] != 0){
					figure_list_[j] -= 1;
					break;
				}
				else {
					figure_list_[j] = BASE-1;	
				}
			}
		}
	}
	Normalize();
	return *this;
}


auto BasicBigNatural::operator+=(BasicBigNatural right) -> BasicBigNatural& {
	const auto size = 
		1+std::max(figure_list_.size(), right.figure_list_.size());
	figure_list_.resize(size);
	right.figure_list_.resize(size);
	int carry = 0;
	for(unsigned int i = 0; i < size; ++i){
		figure_list_[i] += right.figure_list_[i] + carry;
		if(figure_list_[i] >= BASE){
			figure_list_[i] -= BASE;
			carry = 1;	
		}
		else{
			carry = 0;	
		}
	}
	Normalize();
	return *this;
}


auto BasicBigNatural::operator*=(const BasicBigNatural& right) -> BasicBigNatural& {
	FigureList res(figure_list_.size()*right.figure_list_.size()+1, 0);
	for(unsigned int i = 0; i < figure_list_.size(); ++i){
		BaseType carry = 0;
		for(unsigned int j = 0; j < right.figure_list_.size(); ++j){
			BaseType val = figure_list_[i]*right.figure_list_[j] + res[i+j] + carry;
			res[i+j] = val%BASE;
			carry = val / BASE;
		}
		res[i+right.figure_list_.size()] += carry;
	}
	figure_list_ = res;
	Normalize();
	return *this;
}


auto BasicBigNatural::operator*(BasicBigNatural right)const -> BasicBigNatural {
	right *= (*this);
	return right;
}


auto BasicBigNatural::operator+(BasicBigNatural right)const -> BasicBigNatural {
	right += (*this);
	return right;
}


auto BasicBigNatural::operator-(BasicBigNatural right)const -> BasicBigNatural {
	auto temp = *this;
	temp -= right;
	return temp;
}


auto BasicBigNatural::operator/=(const BasicBigNatural& right) -> BasicBigNatural& {
	(*this) = DivideModulate(*this, right).first;
	return *this;
}


auto BasicBigNatural::operator%=(const BasicBigNatural& right) -> BasicBigNatural& {
	(*this) = DivideModulate(*this, right).second;
	return *this;
}


auto BasicBigNatural::operator/(BasicBigNatural right)const -> BasicBigNatural {
	auto temp = *this;
	temp /= right;
	return temp;
}


auto BasicBigNatural::operator%(BasicBigNatural right)const -> BasicBigNatural {
	auto temp = *this;
	temp %= right;
	return temp;
}

}}
