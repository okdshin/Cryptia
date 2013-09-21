#pragma once
//CkcRandom2:20130920
#include <iostream>
#include <cassert>
#include "Random.h"
#include "../common_key/CommonKeyCryptosystem.h"
namespace cryptia {
namespace random {

class CkcRandom2 : public Random {
public:
    CkcRandom2(const common_key::CommonKeyCryptosystem::Ptr& ckc) : ckc_(ckc){}
    ~CkcRandom2(){}

private:
	auto DoInitialize(const ByteArray& seed) -> void {
		assert(seed.size() == 64);
		ckc_->SetKey(ByteArray(seed.begin(), seed.begin()+32));
		counter_.assign(seed.begin()+32, seed.begin()+48);
		state_.assign(seed.begin()+48, seed.begin()+64);
	}

	auto DoGenerateRandomByteArray(unsigned int size) -> ByteArray {
		ByteArray res;
		while(res.size() < size){
			ckc_->SetBlock(counter_, 0);
			ckc_->EncryptBlock();
			ByteArray mixer(16);
			ckc_->GetBlock(mixer, 0);
			for(unsigned int i = 0; i < 16; ++i){
				state_[i] ^= mixer[i];
			}
			ckc_->SetBlock(state_, 0);
			ckc_->EncryptBlock();
			ByteArray res_part(16);
			ckc_->GetBlock(res_part, 0);
			res.insert(res.end(), res_part.begin(), res_part.end());
			for(unsigned int i = 0; i < 16; ++i){
				res_part[i] ^= mixer[i];
			}
			ckc_->SetBlock(res_part, 0);
			ckc_->EncryptBlock();
			ckc_->GetBlock(state_, 0);

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
	ByteArray state_;
};

}}

