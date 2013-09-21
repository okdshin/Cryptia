#pragma once
//CommonKeyCryptosystem:20130918
#include <iostream>
#include <memory>
#include "../FundamentalTypes.h"
namespace cryptia {
namespace common_key {

class CommonKeyCryptosystem{
public:
	using Ptr = std::shared_ptr<CommonKeyCryptosystem>;

    CommonKeyCryptosystem(){}
    virtual ~CommonKeyCryptosystem(){}

	auto SetKey(const ByteArray& byte_array) -> void {
		DoSetKey(byte_array);	
	}

	auto SetBlock(const ByteArray& byte_array, unsigned int offset) -> void {
		DoSetBlock(byte_array, offset);	
	}

	auto EncryptBlock() -> void {
		DoEncryptBlock();
	}

	auto DecryptBlock() -> void {
		DoDecryptBlock();
	}

	auto GetBlock(ByteArray& byte_array, unsigned int offset)const -> void {
		DoGetBlock(byte_array, offset);
	}

private:
	virtual auto DoSetKey(const ByteArray& byte_array) -> void = 0;
	virtual auto DoSetBlock(
		const ByteArray& byte_array, unsigned int offset) -> void = 0;
	virtual auto DoEncryptBlock() -> void = 0;
	virtual auto DoDecryptBlock() -> void = 0;
	virtual auto DoGetBlock(
		ByteArray& byte_array, unsigned int offset)const -> void = 0;

};

}}
