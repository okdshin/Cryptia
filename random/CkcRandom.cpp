#ifdef CKCRANDOM_UNIT_TEST
#include "CkcRandom.h"
#include <iostream>
#include "../common_key/Aes.h"

using namespace cryptia;
using namespace cryptia::random;

int main(int argc, char* argv[])
{
	const auto random = CkcRandom::Create(common_key::Aes::Create());
	random->Initialize(ByteArray(48, 1));
	auto random_array = random->GenerateRandomByteArray(1000);
	std::vector<int> histgram(256, 0);
	std::cout << "random:{";
	for(const auto random_byte : random_array){
		++histgram.at(random_byte);
		std::cout << static_cast<int>(random_byte) << ", ";
	}
	std::cout << "}" << std::endl;

	std::cout << "hist:{";
	for(auto h : histgram){
		std::cout << h << ", ";
	}
	std::cout << "}" << std::endl;

    return 0;
}

#endif
