#ifdef AES_UNIT_TEST
#include "Aes.h"
#include <iostream>

using namespace cryptia;
using namespace cryptia::code;

auto StartNewLine(std::ostream& os) -> void {
	os << std::endl;
}

int main(int argc, char* argv[])
{
	code::Aes aes;
	TestExpandKey128(aes);
	//auto key_byte_array = StringToByteArray("2b7e151628aed2a6abf7158809cf4f3c");
	auto key_byte_array = ByteArray(32, 0);//StringToByteArray("aaaaaaaaaaaaaaaa");
	key_byte_array.at(3) = 'a';
	aes.SetKey(key_byte_array);

	//auto byte_array = StringToByteArray("3243f6a8885a308d313198a2e0370734");
	auto byte_array = StringToByteArray("hello world.....");
	std::cout << "input: ";
	Output(std::cout, byte_array); StartNewLine(std::cout);
	
	aes.SetBlock(byte_array, 0);
	aes.DebugOut(); StartNewLine(std::cout);
	
	aes.EncryptBlock();
	
	ByteArray encrypted_byte_array(16);
	aes.GetBlock(encrypted_byte_array, 0);
	StartNewLine(std::cout);
	std::cout << "encrypted: ";
	Output(std::cout, encrypted_byte_array); StartNewLine(std::cout);

	auto decrypt_key_byte_array = ByteArray(32, 0);//StringToByteArray("aaaaaaaaaaaaaaaa");
	aes.SetKey(decrypt_key_byte_array);
	aes.SetBlock(encrypted_byte_array, 0);
	aes.DecryptBlock();
	
	ByteArray decrypted_byte_array(16);
	aes.GetBlock(decrypted_byte_array, 0);
	std::cout << "decrypted: ";
	Output(std::cout, decrypted_byte_array); StartNewLine(std::cout);
    
	return 0;
}

#endif
