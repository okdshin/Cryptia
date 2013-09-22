#pragma once
//CkcRandom:20130920
#include <iostream>
#include <cassert>
#include "Random.h"
#include "../common_key/CommonKeyCryptosystem.h"
namespace cryptia {
namespace random {

class CkcRandom : public Random {
public:
	using Ptr = std::shared_ptr<CkcRandom>;

	static auto Create(const common_key::CommonKeyCryptosystem::Ptr& ckc) -> Ptr {
		return Ptr(new CkcRandom(ckc));
	}
    ~CkcRandom(){}

private:
    CkcRandom(const common_key::CommonKeyCryptosystem::Ptr& ckc) : ckc_(ckc){}
	
	auto DoInitialize(const ByteArray& seed) -> void {
		assert(seed.size() == 48);
		ckc_->SetKey(ByteArray(seed.begin(), seed.begin()+32));
		counter_.assign(seed.begin()+32, seed.end());
	}

	auto DoGenerateRandomByteArray(unsigned int size) -> ByteArray {
		ByteArray res;
		while(res.size() < size){
			ckc_->SetBlock(counter_, 0);
			ckc_->EncryptBlock();
			ByteArray temp(16);
			ckc_->GetBlock(temp, 0);
			res.insert(res.end(), temp.begin(), temp.end());
			for(int j = counter_.size()-1; j >= 0; --j){
				if(counter_[j] == 255){
					counter_[j] = 0;	
				}
				else {
					++counter_[j];
					break;
				}
			}
		}
		return res;
	}

	common_key::CommonKeyCryptosystem::Ptr ckc_;
	ByteArray counter_;
};

}}

