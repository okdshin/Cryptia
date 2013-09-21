#pragma once
//Mode:20130920
#include <iostream>
#include <memory>
#include "../../FundamentalTypes.h"

namespace cryptia {
namespace common_key {
namespace mode {

class Mode{
public:
	using Ptr = std::shared_ptr<Mode>;

    Mode(){}
    virtual ~Mode(){}

	auto Encrypt(const ByteArray& data, const ByteArray& key, 
			const ByteArray& iv) -> ByteArray {
		return DoEncrypt(data, key, iv);
	}

	auto Decrypt(const ByteArray& data, const ByteArray& key,
			const ByteArray& iv) -> ByteArray {
		return DoDecrypt(data, key, iv);		
	}

private:
	virtual auto DoEncrypt(const ByteArray& data, const ByteArray& key, 
			const ByteArray& iv) -> ByteArray = 0;
	virtual auto DoDecrypt(const ByteArray& data, const ByteArray& key,
			const ByteArray& iv) -> ByteArray = 0;
};

}}}

