#pragma once
//Random:20130920
#include <iostream>
#include <memory>
#include "../FundamentalTypes.h"

namespace cryptia{
namespace random{

class Random{
public:
	using Ptr = std::shared_ptr<Random>;

    Random(){}
    virtual ~Random(){}

	auto Initialize(const ByteArray& seed) -> void {
		DoInitialize(seed);	
	}

	auto GenerateRandomByteArray(unsigned int size) -> ByteArray {
		return DoGenerateRandomByteArray(size);
	}

private:
	virtual auto DoInitialize(const ByteArray& seed) -> void = 0;
	virtual auto DoGenerateRandomByteArray(unsigned int size) -> ByteArray = 0;
};

}
}
