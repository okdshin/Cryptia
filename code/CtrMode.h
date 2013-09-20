#pragma once
//CtrMode:20130916
#include <iostream>
#include <functional>
#include "../FundamentalTypes.h"
#include "../BigNatural.h"
#include "CommonKeyCryptosystem.h"

namespace cryptia{
namespace code
{
class CtrMode{
public:
	CtrMode(const CommonKeyCryptosystem::Ptr& ckc) : ckc_(ckc){}
    ~CtrMode(){}

	auto EncryptAndDecrypt(const ByteArray& data, const ByteArray& key, 
			ByteArray counter) -> ByteArray {
		ckc_->SetKey(key);
		ByteArray res(data.size()+BLOCK_BYTE_NUM-(data.size()%BLOCK_BYTE_NUM));
		ByteArray tmp(BLOCK_BYTE_NUM);
		for(unsigned int i = 0; i < data.size(); i+=BLOCK_BYTE_NUM){
			for(int j = 0; j < BLOCK_BYTE_NUM; ++j){
				tmp[BLOCK_BYTE_NUM-j-1] = (j < static_cast<int>(counter.size())) 
					? counter[counter.size()-j-1] : 0x00;
			}
			ckc_->SetBlock(tmp, 0);
			ckc_->EncryptBlock();
			ckc_->GetBlock(res, i);
			for(int j = 0; j < BLOCK_BYTE_NUM; ++j){
				res[i+j] ^= data[i+j];
			}
			for(int j = counter.size()-1; j >= 0; --j){
				if(counter[j] == 255){
					counter[j] = 0;	
				}
				else {
					++counter[j];
					break;
				}
			}
		}
		return res;
	}

private:
	CommonKeyCryptosystem::Ptr ckc_;
	static const int BLOCK_BYTE_NUM;
};
const int CtrMode::BLOCK_BYTE_NUM = 16;
}
}
