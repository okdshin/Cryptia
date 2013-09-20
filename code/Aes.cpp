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
	auto aes = code::Aes::Create();

	TestExpandKey128(aes);
	TestExpandKey256(aes);
	TestEncryptAndDecryptBlock(aes);
	//TestEncryptBlock(aes);
	return 0;
}

#endif
