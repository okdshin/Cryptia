#pragma once
//Aes:20130904
#include <iostream>
#include <iomanip>
#include <array>
#include <cassert>
#include "../FundamentalTypes.h"
#include "CommonKeyCryptosystem.h"

namespace cryptia {
namespace code {

class Aes : public CommonKeyCryptosystem{
public:
	using Ptr = std::shared_ptr<Aes>;

	static auto Create() -> Ptr {
		return Ptr(new Aes());	
	}
	
    ~Aes(){}
private:
	using State = std::array<Word, 4>;
	using RoundKey = std::array<Word, 4>;
	using RoundKeyList = std::vector<RoundKey>;
	
	Aes(){}
	
	auto DebugOut() -> void {
		std::cout << "key:" ;
		for(const auto& round_key : round_key_list_){
			for(const auto k : round_key){
				std::cout << k << " ";
			}
		}
		std::cout << "\nstate: ";
		for(Word w : state_){
			std::cout << w << " ";
		}
		std::cout << std::flush;
	}

	auto DebugOut(const RoundKey& round_key) -> void {
		std::cout << "debugkey: ";
		for(unsigned int col = 0; col < 4; ++col){
			for(unsigned int row = 0; row < 4; ++row){
				std::cout.width(2);
				std::cout.fill('0');
				std::cout.setf(std::ios::hex, std::ios::basefield); 
				std::cout << (((round_key[col] >> (8*row))) & 0xff) << " ";
				std::cout.setf(std::ios::dec, std::ios::basefield); 
			}	
		}
		std::cout << "\n";
	}

	auto DoSetKey(const ByteArray& byte_array) -> void {
		round_key_list_.clear();
		ExpandKey(byte_array);
	}

	auto DoSetBlock(const ByteArray& message, unsigned int offset) -> void {
		assert(message.size() >= offset+16);
		for(unsigned int i = 0; i < 4; ++i){
			state_[i] = static_cast<Word>(message[offset + i*4])
				| static_cast<Word>(message[offset + i*4 + 1]) << 8
				| static_cast<Word>(message[offset + i*4 + 2]) << 16
				| static_cast<Word>(message[offset + i*4 + 3]) << 24;
		}
	}

	auto DoEncryptBlock() -> void {
		AddRoundKey(round_key_list_.front());
		for(unsigned int i = 1; i < round_key_list_.size()-1; ++i){
			SubBytes();
			ShiftRows();
			MixColumns();
			AddRoundKey(round_key_list_[i]);
		}
		SubBytes();
		ShiftRows();
		AddRoundKey(round_key_list_.back());
	}

	auto DoDecryptBlock() -> void {
		AddRoundKey(round_key_list_.back());
		InvShiftRows();
		InvSubBytes();
		for(unsigned int i = round_key_list_.size()-2; i >= 1; --i){
			AddRoundKey(round_key_list_[i]);
			InvMixColumns();
			InvShiftRows();
			InvSubBytes();
		}
		AddRoundKey(round_key_list_.front());
	}

	auto DoGetBlock(ByteArray& byte_array, unsigned int offset)const -> void {
		for(unsigned int col = 0; col < 4; ++col){
			for(unsigned int row = 0; row < 4; ++row){
				byte_array[offset+col*4+row] = 
					((state_[col] >> (8*row))) & 0xff;
			}	
		}
	}

	auto AddRoundKey(const RoundKey& key_state) -> void {
		for(unsigned int i = 0; i < 4; ++i){
			state_[i] ^= key_state[i];
		}			
	}

	auto SubBytes() -> void {
		for(int i = 0; i < 4; ++i){
			state_[i] = SubWord(state_[i]);
		}
	}

	auto SubWord(Word w)const -> Word {
		return SBOX[w & 0xff]
			| (SBOX[(w >> 8) & 0xff]) << 8
			| (SBOX[(w >> 16) & 0xff]) << 16
			| (SBOX[(w >> 24) & 0xff]) << 24;
	}

	auto InvSubBytes() -> void {
		for(int i = 0; i < 4; ++i){
			state_[i] = InvSubWord(state_[i]);
		}
	}

	auto InvSubWord(Word w)const -> Word {
		return INV_SBOX[w & 0xff]
			| (INV_SBOX[(w >> 8) & 0xff]) << 8
			| (INV_SBOX[(w >> 16) & 0xff]) << 16
			| (INV_SBOX[(w >> 24) & 0xff]) << 24;
	}

	auto ShiftRows() -> void {
		auto base = state_;
		state_[0] = (base[0] & 0x000000ff) 
			| (base[1] & 0x0000ff00)
			| (base[2] & 0x00ff0000)
			| (base[3] & 0xff000000);
		state_[1] = (base[1] & 0x000000ff) 
			| (base[2] & 0x0000ff00)
			| (base[3] & 0x00ff0000)
			| (base[0] & 0xff000000);
		state_[2] = (base[2] & 0x000000ff) 
			| (base[3] & 0x0000ff00)
			| (base[0] & 0x00ff0000)
			| (base[1] & 0xff000000);
		state_[3] = (base[3] & 0x000000ff) 
			| (base[0] & 0x0000ff00)
			| (base[1] & 0x00ff0000)
			| (base[2] & 0xff000000);
	}

	auto InvShiftRows() -> void {
		auto base = state_;
		state_[0] = (base[0] & 0x000000ff) 
			| (base[3] & 0x0000ff00)
			| (base[2] & 0x00ff0000)
			| (base[1] & 0xff000000);
		state_[1] = (base[1] & 0x000000ff) 
			| (base[0] & 0x0000ff00)
			| (base[3] & 0x00ff0000)
			| (base[2] & 0xff000000);
		state_[2] = (base[2] & 0x000000ff) 
			| (base[1] & 0x0000ff00)
			| (base[0] & 0x00ff0000)
			| (base[3] & 0xff000000);
		state_[3] = (base[3] & 0x000000ff) 
			| (base[2] & 0x0000ff00)
			| (base[1] & 0x00ff0000)
			| (base[0] & 0xff000000);
	}

	auto MixColumns() -> void {
		for(int i = 0; i < 4; ++i){
			state_[i] = MixColumn(state_[i]);
		}
	}
	
	auto MixColumn(Word col)const -> Word {
		Byte one0 = static_cast<Byte>(col);
		Byte one1 = static_cast<Byte>(col >> 8);
		Byte one2 = static_cast<Byte>(col >> 16);
		Byte one3 = static_cast<Byte>(col >> 24);

		Byte two0 = Square(one0);
		Byte two1 = Square(one1);
		Byte two2 = Square(one2);
		Byte two3 = Square(one3);

		return ((two0		^one1^two1	^one2		^one3		) & 0xff)
			| ((one0		^two1		^one2^two2	^one3		) & 0xff) << 8
			| ((one0		^one1		^two2		^one3^two3	) & 0xff) << 16
			| ((one0^two0	^one1		^one2		^two3		) & 0xff) << 24;
	}
	
	auto Square(Byte b)const -> Byte {
		bool msb = (b & 0x80) != 0;
		b <<= 1;
		if(msb){
			b ^= 0x11b;	
		}
		return b;
	}

	auto InvMixColumns() -> void {
		for(int i = 0; i < 4; ++i){
			state_[i] = InvMixColumn(state_[i]);
		}
	}

	auto InvMixColumn(Word col)const -> Word {
		Byte a0 = static_cast<Byte>(col);
		Byte a1 = static_cast<Byte>(col >> 8);
		Byte a2 = static_cast<Byte>(col >> 16);
		Byte a3 = static_cast<Byte>(col >> 24);

		return    (static_cast<Word>(Square2(a0, 14)^Square2(a1, 11)
									^Square2(a2, 13)^Square2(a3,  9)) & 0xff)
			|     (static_cast<Word>(Square2(a0,  9)^Square2(a1, 14)
									^Square2(a2, 11)^Square2(a3, 13)) & 0xff) << 8
			|     (static_cast<Word>(Square2(a0, 13)^Square2(a1,  9)
									^Square2(a2, 14)^Square2(a3, 11)) & 0xff) << 16
			|     (static_cast<Word>(Square2(a0, 11)^Square2(a1, 13)
									^Square2(a2,  9)^Square2(a3, 14)) & 0xff) << 24;
	}

	auto Square2(Byte a, Byte b)const -> Byte {
		Byte x = 0;
		for(unsigned int i = 0; i < 8; ++i){
			if(b&1){
				x ^= a;
			}
			a = Square(a);
			b >>= 1;
		}
		return x;
	}

	auto ExpandKey(ByteArray key) -> void {
		assert(key.size() == 16 || key.size() == 24 || key.size() == 32);
		unsigned int key_length = key.size() / 4;
		unsigned int round_count = key_length + 6;
		round_key_list_.resize(round_count+1);
		for(unsigned int i = 0; i < key_length; ++i){
			round_key_list_[i >> 2][i & 3] = static_cast<Word>(key[i*4] & 0xff)
				| static_cast<Word>(key[i*4+1] & 0xff) << 8
				| static_cast<Word>(key[i*4+2] & 0xff) << 16
				| static_cast<Word>(key[i*4+3] & 0xff) << 24;
		}
		Word x = round_key_list_[(key_length-1) >> 2][(key_length-1) & 3];
		for(unsigned int i = key_length; i < (round_count+1)*4; ++i){
			if(!(i%key_length)){
				x = SubWord(RotateRight(8, x)) ^ RCON[i / key_length];
			}
			if(key_length == 8 && (i%key_length) == 4){
				x = SubWord(x);	
			}
			x ^= round_key_list_[(i-key_length) >> 2][(i-key_length) & 3];
			round_key_list_[i >> 2][i & 3] = x;
		}
	}

	State state_;
	RoundKeyList round_key_list_;

	static const std::array<Byte, 256> SBOX;
	static const std::array<Byte, 256> INV_SBOX;
	static const std::array<Byte, 12> RCON;

	friend auto TestExpandKey128(const Aes::Ptr& aes) -> void {
		ByteArray key(16, 0x00);
		aes->ExpandKey(key);
		std::array<std::array<Word, 4>, 11> answer_list({
			RoundKey({ 0x00000000, 0x00000000, 0x00000000, 0x00000000 }),
			RoundKey({ 0x62636363, 0x62636363, 0x62636363, 0x62636363 }),
			RoundKey({ 0x9b9898c9, 0xf9fbfbaa, 0x9b9898c9, 0xf9fbfbaa }),
			RoundKey({ 0x90973450, 0x696ccffa, 0xf2f45733, 0x0b0fac99 }),
			RoundKey({ 0xee06da7b, 0x876a1581, 0x759e42b2, 0x7e91ee2b }),
			RoundKey({ 0x7f2e2b88, 0xf8443e09, 0x8dda7cbb, 0xf34b9290 }),
			RoundKey({ 0xec614b85, 0x1425758c, 0x99ff0937, 0x6ab49ba7 }),
            RoundKey({ 0x21751787, 0x3550620b, 0xacaf6b3c, 0xc61bf09b }),
            RoundKey({ 0x0ef90333, 0x3ba96138, 0x97060a04, 0x511dfa9f }),
            RoundKey({ 0xb1d4d8e2, 0x8a7db9da, 0x1d7bb3de, 0x4c664941 }),
            RoundKey({ 0xb4ef5bcb, 0x3e92e211, 0x23e951cf, 0x6f8f188e })
		});
		for(unsigned int i = 0; i < aes->round_key_list_.size(); ++i){
			for(unsigned int j = 0; j < aes->round_key_list_[i].size(); ++j){
				auto k = aes->round_key_list_[i][j];
				auto reverse_k = static_cast<Word>((k >>  0) & 0xff) << 24 
					| static_cast<Word>((k >>  8) & 0xff) << 16
					| static_cast<Word>((k >> 16) & 0xff) <<  8
					| static_cast<Word>((k >> 24) & 0xff) <<  0;
				if(reverse_k != answer_list[i][j]){
					std::cout << 
						"TestExpandKey128 Failed.\n"
						"\tindex: " << i << "\n"
						"\t(calc)" << reverse_k 
							<< " != (answer)" << answer_list[i][j] << std::endl;	
					return;
				}
			}
		}
		std::cout << "TestExpandKey128 Succeeded." << std::endl;
	}

	friend auto TestExpandKey256(const Aes::Ptr& aes) -> void {
		ByteArray key(32, 0x00);
		aes->ExpandKey(key);
		std::array<std::array<Word, 4>, 15> answer_list({
			RoundKey({ 0x00000000, 0x00000000, 0x00000000, 0x00000000 }),
			RoundKey({ 0x00000000, 0x00000000, 0x00000000, 0x00000000 }),
			RoundKey({ 0x62636363, 0x62636363, 0x62636363, 0x62636363 }),
			RoundKey({ 0xaafbfbfb, 0xaafbfbfb, 0xaafbfbfb, 0xaafbfbfb }),
			RoundKey({ 0x6f6c6ccf, 0x0d0f0fac, 0x6f6c6ccf, 0x0d0f0fac }),
			RoundKey({ 0x7d8d8d6a, 0xd7767691, 0x7d8d8d6a, 0xd7767691 }),
			RoundKey({ 0x5354edc1, 0x5e5be26d, 0x31378ea2, 0x3c38810e }),
			RoundKey({ 0x968a81c1, 0x41fcf750, 0x3c717a3a, 0xeb070cab }),
			RoundKey({ 0x9eaa8f28, 0xc0f16d45, 0xf1c6e3e7, 0xcdfe62e9 }),
			RoundKey({ 0x2b312bdf, 0x6acddc8f, 0x56bca6b5, 0xbdbbaa1e }),
			RoundKey({ 0x6406fd52, 0xa4f79017, 0x553173f0, 0x98cf1119 }),
			RoundKey({ 0x6dbba90b, 0x07767584, 0x51cad331, 0xec71792f }),
			RoundKey({ 0xe7b0e89c, 0x4347788b, 0x16760b7b, 0x8eb91a62 }),
			RoundKey({ 0x74ed0ba1, 0x739b7e25, 0x2251ad14, 0xce20d43b }),
			RoundKey({ 0x10f80a17, 0x53bf729c, 0x45c979e7, 0xcb706385 })
		});
		for(unsigned int i = 0; i < aes->round_key_list_.size(); ++i){
			for(unsigned int j = 0; j < aes->round_key_list_[i].size(); ++j){
				auto k = aes->round_key_list_[i][j];
				auto reverse_k = static_cast<Word>((k >>  0) & 0xff) << 24 
					| static_cast<Word>((k >>  8) & 0xff) << 16
					| static_cast<Word>((k >> 16) & 0xff) <<  8
					| static_cast<Word>((k >> 24) & 0xff) <<  0;
				if(reverse_k != answer_list[i][j]){
					std::cout << 
						"TestExpandKey256 Failed.\n"
						"\tindex: " << i << "\n"
						"\t(calc)" << reverse_k 
							<< " != (answer)" << answer_list[i][j] << std::endl;	
					return;
				}
			}
		}
		std::cout << "TestExpandKey256 Succeeded." << std::endl;
	}

	friend auto TestEncryptAndDecryptBlock(const Aes::Ptr& aes) -> void {
		auto key_byte_array = ByteArray(16, 0);
		aes->SetKey(key_byte_array);

		auto byte_array = StringToByteArray("hello world.....");
		
		aes->SetBlock(byte_array, 0);
		aes->EncryptBlock();
		
		ByteArray encrypted_byte_array(16);
		aes->GetBlock(encrypted_byte_array, 0);

		aes->SetKey(key_byte_array);
		aes->SetBlock(encrypted_byte_array, 0);
		aes->DecryptBlock();
		
		ByteArray decrypted_byte_array(16);
		aes->GetBlock(decrypted_byte_array, 0);

		for(unsigned int i = 0; i < byte_array.size(); ++i){
			if(byte_array[i] != decrypted_byte_array[i]){
				std::cout << "TestEncryptAndDecryptBlock Failed." << std::endl;
				return;
			}
		}
		std::cout << "TestEncryptAndDecryptBlock Succeeded." << std::endl;
	}

	/*TODO
	friend auto TestEncryptBlock(Aes& aes) -> void {
		auto key_byte_array = HexStringToByteArray("2b7e151628aed2a6abf7158809cf4f3c");
		aes->SetKey(key_byte_array);

		auto plain_text_block = HexStringToByteArray("f0f1f2f3f5f6f7f8f9fafbfcfdfeff");
		
		aes->SetBlock(plain_text_block, 0);
		aes->EncryptBlock();
		
		ByteArray encrypted_byte_array(16);
		aes->GetBlock(encrypted_byte_array, 0);
		OutputHex(std::cout, encrypted_byte_array);
	}
	*/
};

const std::array<Byte, 256> Aes::SBOX = {
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 
		0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 
		0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 
		0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 
		0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 
		0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 
		0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 
		0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 
		0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 
		0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 
		0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 
		0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 
		0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 
		0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 
		0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 
		0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 
		0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

const std::array<Byte, 256> Aes::INV_SBOX = {
	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 
		0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, 
	0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 
		0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, 
	0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 
		0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 
		0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
	0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 
		0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
	0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 
		0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 
		0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
	0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 
		0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
	0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 
		0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 
		0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
	0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 
		0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
	0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 
		0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 
		0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
	0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 
		0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
	0xa0, 0xe0 ,0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 
		0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 
		0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

const std::array<Byte, 12> Aes::RCON {
	0x8d, 0x01, 0x02, 0x04, 0x08, 0x10,
	0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c
};

}
}
