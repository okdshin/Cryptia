#pragma once
//FundamentalTypes:20130907
#include <iostream>
#include <vector>
#include <cstdint>
#include <sstream>

namespace cryptia {
using Byte = uint8_t;
using ByteArray = std::vector<Byte>;
using Word = uint32_t;

auto Output(std::ostream& os, const ByteArray& byte_array) -> void {
	os.setf(std::ios::hex, std::ios::basefield);
	for(Byte c : byte_array){
		os.width(2);
		os.fill('0');
		os << "'" << static_cast<char>(c) << "'" << "(" << static_cast<int>(c) << ")";
	}	
	os << std::flush;
	os.setf(std::ios::dec, std::ios::basefield);
}

auto OutputHex(std::ostream& os, const ByteArray& byte_array) -> void {
	os.setf(std::ios::hex, std::ios::basefield);
	for(Byte c : byte_array){
		os.width(2);
		os.fill('0');
		os << static_cast<int>(c);
	}	
	os << std::endl;
	os.setf(std::ios::dec, std::ios::basefield);
}

auto InputHex(std::istream& is) -> ByteArray {
	char c;
	std::vector<char> buffer;
	ByteArray res;
	while(is){
		if(buffer.size() == 2){
			res.push_back(buffer.front()*16+buffer.back());
			buffer.clear();
		}
		is >> c;
		if('0' <= c && c <= '9'){
			buffer.push_back(c-'0');
		}else
		if('a' <= c && c <= 'f'){
			buffer.push_back(c-'a'+10);	
		}
	}
	return res;
}

auto RotateRight(int count, Word w) -> Word {
	return (w >> count) | w << (32-count);
}

auto ShiftRight(int count, Word w) -> Word {
	return w >> count;
}

auto StringToByteArray(const std::string& str) -> ByteArray {
	return ByteArray(str.begin(), str.end());	
}

auto HexStringToByteArray(const std::string& str) -> ByteArray {
	std::istringstream iss(str);
	return InputHex(iss);	
}

auto operator==(const ByteArray& a, const ByteArray& b) -> bool {
	for(unsigned int i = 0; i < a.size(); ++i){
		if(a != b){
			return false;
		}
	}
	return true;
}

}
