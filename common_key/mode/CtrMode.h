#pragma once
//CtrMode:20130916
#include <iostream>
#include <functional>
#include "Mode.h"
#include "../../FundamentalTypes.h"
#include "../CommonKeyCryptosystem.h"
#include "../Aes.h"
#include "../../Test.h"

namespace cryptia {
namespace common_key {
namespace mode {

class CtrMode : public Mode {
public:
	using Ptr = std::shared_ptr<CtrMode>;
	static auto Create(const CommonKeyCryptosystem::Ptr& ckc) -> Ptr {
		return Ptr(new CtrMode(ckc));	
	}

    ~CtrMode(){}

private:
	CtrMode(const CommonKeyCryptosystem::Ptr& ckc) : ckc_(ckc){}

	auto DoEncrypt(const ByteArray& data, const ByteArray& key, 
			const ByteArray& iv) -> ByteArray {
		return EncryptAndDecrypt(data, key, iv);		
	}

	auto DoDecrypt(const ByteArray& data, const ByteArray& key,
			const ByteArray& iv) -> ByteArray {
		return EncryptAndDecrypt(data, key, iv);				
	}

	auto EncryptAndDecrypt(ByteArray data, const ByteArray& key, 
			ByteArray counter) -> ByteArray {
		ckc_->SetKey(key);
		//ByteArray res(data.size()%16 ? data.size()+BLOCK_BYTE_NUM-(data.size()%BLOCK_BYTE_NUM) : data.size());
		
		ByteArray res(data.size());
		while(res.size()%BLOCK_BYTE_NUM){
			printf("+");
			res.push_back(0);
			data.push_back(0);
		}
		
		ByteArray tmp(BLOCK_BYTE_NUM);
		assert(res.size() % BLOCK_BYTE_NUM == 0);
		for(unsigned int i = 0; i < res.size(); i+=BLOCK_BYTE_NUM){
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

public:
	friend auto TestCtrModeAesEncrypt() -> void;
	
	CommonKeyCryptosystem::Ptr ckc_;
	static const int BLOCK_BYTE_NUM;
};
const int CtrMode::BLOCK_BYTE_NUM = 16;

auto TestCtrModeAesEncrypt() -> void {
	auto aes = Aes::Create();
	auto ctr_mode = CtrMode::Create(aes);
	auto data = ByteArray({
		0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 
		0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,	
	});
	auto key = ByteArray({
		0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 
		0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
	});
	auto counter = ByteArray({
		0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
		0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
	});
	auto encrypted = ctr_mode->Encrypt(data, key, counter);
	CRYPTIA_CHECK_EQUAL(encrypted, 
		HexStringToByteArray("874d6191b620e3261bef6864990db6ce"));
	/*
	OutputHex(std::cout << "encrypted: ", encrypted);
	std::cout << std::endl;
	auto decrypted = ctr_mode->Decrypt(encrypted, key, counter);
	OutputHex(std::cout << "decrypted: ", decrypted);	
	*/
}

}}}
