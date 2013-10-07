#pragma once
//BigNatural:20130710
#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <cassert>
#include "../FundamentalTypes.h"

namespace cryptia{
namespace asymmetric_key{

using BaseType = uint32_t;

class BigNatural{
public:
	using FigureList = std::vector<BaseType>;

	BigNatural(BaseType num);
	explicit BigNatural(const ByteArray& byte_array);
	explicit BigNatural(const std::string& num_str);

	auto Output(std::ostream& os)const -> void;
	auto ToByteArray()const -> ByteArray;

	auto operator==(const BigNatural& right)const -> bool;
	auto operator!=(const BigNatural& right)const -> bool;
	auto operator<(const BigNatural& right)const -> bool;
	auto operator>(const BigNatural& right)const -> bool;
	auto operator>=(const BigNatural& right)const -> bool;
	auto operator<=(const BigNatural& right)const -> bool;

	auto operator+=(BigNatural right) -> BigNatural&;
	auto operator-=(BigNatural right) -> BigNatural&;
	auto operator*=(const BigNatural& right) -> BigNatural&;
	auto operator/=(const BigNatural& right) -> BigNatural&;
	auto operator%=(const BigNatural& right) -> BigNatural&;
	auto operator<<=(BaseType right) -> BigNatural&;
	auto operator>>=(BaseType right) -> BigNatural&;
	auto operator&=(BigNatural right) -> BigNatural&;
	
	auto ShiftLeftAlittle(unsigned int num) -> void {
		AppendFigureUpper(1);
		BaseType carry = 0;
		for(unsigned int i = 0; i < figure_list_.size(); ++i){
			auto before_val = figure_list_[i];
			figure_list_[i] <<= num;
			figure_list_[i] |= carry;	
			figure_list_[i] &= MAX_NUM;	
			carry = (before_val >> (BASE_BIT_NUM-num)) & MAX_NUM; 
		}
		Normalize();
	}
	
	auto ShiftRightAlittle(unsigned int num) -> void {
		//assert(num < BASE_BIT_NUM/2);
		BaseType carry = 0;
		for(int i = figure_list_.size()-1; i >= 0; --i){
			auto before_val = figure_list_[i];
			figure_list_[i] >>= num;
			figure_list_[i] |= carry;	
			figure_list_[i] &= MAX_NUM;	
			carry = (before_val << (BASE_BIT_NUM-num)) & MAX_NUM; 	
		}
		Normalize();
	}

	auto AppendFigureLower(BaseType append_num) -> void;
	auto AppendFigureUpper(BaseType append_num) -> void;
	
	auto operator+(BigNatural right)const -> BigNatural;
	auto operator-(const BigNatural& right)const -> BigNatural;
	auto operator*(BigNatural right)const -> BigNatural;
	auto operator/(const BigNatural& right)const -> BigNatural;
	auto operator%(const BigNatural& right)const -> BigNatural;
	auto operator<<(BaseType right)const -> BigNatural;
	auto operator>>(BaseType right)const -> BigNatural;
	auto operator&(BigNatural right)const -> BigNatural;

	static auto DivideModulate(const BigNatural& left, const BigNatural& right) 
			-> std::pair<BigNatural, BigNatural>;

	static auto Power(BigNatural base, BaseType exponent) -> BigNatural;
	static auto Power(BigNatural base, BigNatural exponent) -> BigNatural;

	static auto PowerModulate(BigNatural base, BaseType exponent, const BigNatural& n) -> BigNatural;
	static auto PowerModulate(BigNatural base, BigNatural exponent, const BigNatural& n) -> BigNatural;
	

	friend auto operator>>(std::istream& is, BigNatural& val) -> std::istream& {
		std::string num_str;
		is >> num_str;
		val = BigNatural(num_str);
		return is;
	}

	friend auto operator<<(std::ostream& os, const BigNatural& val) -> std::ostream& {
		std::vector<char> dec_figure_list;

		/*
		BigNatural<10> res(0);
		BigNatural<10> base(1);
		for(auto figure : val.figure_list_){
			res += BigNatural<10>(figure) * base;
			base *= BigNatural<10>(val.MAX_NUM_);
		}
		for(int i = res.figure_list_.size()-1; i >= 0; --i){
			os << res.figure_list_[i];
		}
		*/
		val.Output(os);
		return os;
	}

	auto IsOdd()const -> bool {
		return figure_list_.front() & 1;	
	}
private:
	static const BaseType BASE_NUM = 65536;
	static const BaseType BASE_BIT_NUM = 16;
	static const BaseType MAX_NUM = 65535;
	auto Normalize() -> void;

	FigureList figure_list_;
};

auto BigNatural::AppendFigureLower(BaseType append_num) -> void {
	figure_list_.insert(figure_list_.begin(), append_num, 0);
}

auto BigNatural::AppendFigureUpper(BaseType append_num) -> void {	
	figure_list_.insert(figure_list_.end(), append_num, 0);
}

auto BigNatural::Normalize() -> void {
	auto iter = figure_list_.end()-1; 
	while(!(*iter) && iter != figure_list_.begin()){
		--iter;
	}
	figure_list_.erase(iter+1, figure_list_.end());
}


auto BigNatural::DivideModulate(const BigNatural& left, 
		const BigNatural& right) -> std::pair<BigNatural, BigNatural> {
	BigNatural x(right), q(0), r(0);
	for(int i = left.figure_list_.size()-1; i >= 0; --i){
		r = r*BigNatural(BASE_NUM)+left.figure_list_[i];
		BaseType smaller(0), bigger(MAX_NUM);
		if(r >= x){
			while(smaller+1 < bigger){
				BaseType mid = (smaller + bigger) >> 1;
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


auto BigNatural::Power(BigNatural base, BaseType exponent) -> BigNatural {
	BigNatural res(1);
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

auto BigNatural::Power(BigNatural base, BigNatural exponent) -> BigNatural {
	BigNatural res(1);
	while(exponent != 0){
		if((exponent.figure_list_.front() != 0)){
			res *= Power(base, exponent.figure_list_.front());	
		}
		exponent.AppendFigureRight(1);
		base = Power(base, MAX_NUM);
	}
	return res;
}
*/

auto BigNatural::Power(BigNatural base, BigNatural exponent) -> BigNatural {
	BigNatural res(1);
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


auto BigNatural::PowerModulate(BigNatural base, 
		BaseType exponent, const BigNatural& n) -> BigNatural {
	BigNatural res(1);
	while(exponent > 0){
		if(exponent&1){//IsOdd
			res *= base;
			res %= n;
		}
		exponent >>= 1;
		base *= base;
		base %= n;
	}
	return res;
}


auto BigNatural::PowerModulate(BigNatural base, 
		BigNatural exponent, const BigNatural& n) -> BigNatural {
	BigNatural res(1);
	while(exponent > 0){
		if(exponent.figure_list_.front() & 1){
			res *= base;
			res %= n;
		}
		exponent >>= 1;
		base *= base;
		base %= n;
	}
	return res;
}


BigNatural::BigNatural(BaseType num) : figure_list_() {
	figure_list_.push_back(abs(num));
	while(figure_list_.back() >= BASE_NUM){
		auto temp = figure_list_.back();
		figure_list_.back() &= MAX_NUM;
		figure_list_.push_back(temp >> BASE_BIT_NUM);
	}
}

BigNatural::BigNatural(const ByteArray& byte_array) : figure_list_() {
	assert(byte_array.size()%2 == 0);
	for(int i = byte_array.size()-1; i > 0; i -= 2){
		figure_list_.push_back(
			(static_cast<BaseType>(byte_array.at(i)) << 0)
			|(static_cast<BaseType>(byte_array.at(i-1)) << 8)
		);	
	}
	Normalize();
}

auto BigNatural::ToByteArray()const -> ByteArray {
	ByteArray res;
	for(int i = figure_list_.size()-1; i >= 0; --i){
		auto figure = figure_list_.at(i);
		res.push_back(static_cast<uint8_t>((figure >> 8) & 0xff));
		res.push_back(static_cast<uint8_t>(figure & 0xff));
	}
	return res;	
}

BigNatural::BigNatural(const std::string& num_str) : figure_list_() {
	int begin = 0;
	BigNatural dec_figure(1);
	for(int i = num_str.size()-1; i >= begin; --i){
		const char c = num_str[i]; 
		int x = c - '0';
		if(x < 0 || 9 < x){
			std::cerr << "ParseError" << num_str << std::endl;	
			exit(1);
		}
		*this += dec_figure*BigNatural(x);
		dec_figure *= 10;
	}	
}

auto BigNatural::Output(std::ostream& os)const -> void {
	os << "{";
	for(int i = figure_list_.size()-1; i >= 0; --i){
		os << figure_list_[i];
		if(i != 0){
			os << " ";
		}
	}
	os << "}";
}

auto BigNatural::operator==(const BigNatural& right)const -> bool {
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

auto BigNatural::operator!=(const BigNatural& right)const -> bool {
	return !((*this) == right);
}

auto BigNatural::operator<(const BigNatural& right)const -> bool {
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

auto BigNatural::operator>(const BigNatural& right)const -> bool {
	return right < (*this);	
}

auto BigNatural::operator>=(const BigNatural& right)const -> bool {
	return (*this) > right || (*this) == right;	
}

auto BigNatural::operator<=(const BigNatural& right)const -> bool {
	return (*this) < right || (*this) == right;	
}

auto BigNatural::operator-=(BigNatural right) -> BigNatural& {
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
			figure_list_[i] += BASE_NUM;
			figure_list_[i] -= right.figure_list_[i];
			for(unsigned int j = i+1; j < size; ++j){
				if(figure_list_[j] != 0){
					figure_list_[j] -= 1;
					break;
				}
				else {
					figure_list_[j] = MAX_NUM;	
				}
			}
		}
	}
	Normalize();
	return *this;
}

auto BigNatural::operator+=(BigNatural right) -> BigNatural& {
	const auto size = 
		1+std::max(figure_list_.size(), right.figure_list_.size());
	figure_list_.resize(size);
	right.figure_list_.resize(size);
	int carry = 0;
	for(unsigned int i = 0; i < size; ++i){
		figure_list_[i] += right.figure_list_[i] + carry;
		if(figure_list_[i] >= BASE_NUM){
			figure_list_[i] -= BASE_NUM;
			carry = 1;	
		}
		else{
			carry = 0;	
		}
	}
	Normalize();
	return *this;
}

auto BigNatural::operator*=(const BigNatural& right) -> BigNatural& {
	FigureList res(figure_list_.size()*right.figure_list_.size()+1);
	for(unsigned int i = 0; i < figure_list_.size(); ++i){
		BaseType carry = 0;
		for(unsigned int j = 0; j < right.figure_list_.size(); ++j){
			BaseType val = figure_list_[i]*right.figure_list_[j] + res[i+j] + carry;
			res[i+j] = val & MAX_NUM;
			carry = val >> BASE_BIT_NUM;
		}
		res[i+right.figure_list_.size()] += carry;
	}
	figure_list_ = res;
	Normalize();
	return *this;
	
}

auto BigNatural::operator<<=(BaseType right) -> BigNatural& {
	BaseType append_num = right/BASE_BIT_NUM;
	right %= BASE_BIT_NUM;
	AppendFigureLower(append_num);
	ShiftLeftAlittle(right);
	return *this;
}

auto BigNatural::operator>>=(BaseType right) -> BigNatural& {
	BaseType erase_num = right/BASE_BIT_NUM;
	right %= BASE_BIT_NUM;
	figure_list_.erase(figure_list_.begin(), figure_list_.begin()+erase_num);
	ShiftRightAlittle(right);
	return *this;
}

auto BigNatural::operator*(BigNatural right)const -> BigNatural {
	right *= (*this);
	return right;
}

auto BigNatural::operator+(BigNatural right)const -> BigNatural {
	right += (*this);
	return right;
}

auto BigNatural::operator-(const BigNatural& right)const -> BigNatural {
	auto temp = *this;
	temp -= right;
	return temp;
}

auto BigNatural::operator<<(BaseType right)const -> BigNatural {
	auto temp = *this;
	temp <<= right;
	return temp;
}

auto BigNatural::operator>>(BaseType right)const -> BigNatural {
	auto temp = *this;
	temp >>= right;
	return temp;
	
}

auto BigNatural::operator/=(const BigNatural& right) -> BigNatural& {
	(*this) = DivideModulate(*this, right).first;
	return *this;
}

auto BigNatural::operator%=(const BigNatural& right) -> BigNatural& {
	(*this) = DivideModulate(*this, right).second;
	return *this;
}

auto BigNatural::operator/(const BigNatural& right)const -> BigNatural {
	auto temp = *this;
	temp /= right;
	return temp;
}

auto BigNatural::operator%(const BigNatural& right)const -> BigNatural {
	auto temp = *this;
	temp %= right;
	return temp;
}

auto BigNatural::operator&=(BigNatural right) -> BigNatural& {
	if(figure_list_.size() < right.figure_list_.size()){
		right.figure_list_.resize(figure_list_.size());	
	}
	else {
		figure_list_.resize(right.figure_list_.size());	
	}
	for(unsigned int i = 0; i < figure_list_.size(); ++i){
		figure_list_[i] &= right.figure_list_[i];
	}
	return *this;
}

auto BigNatural::operator&(BigNatural right)const -> BigNatural {
	right &= *this;
	return right;
}
}}
