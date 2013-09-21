#ifdef CKCRANDOM2_UNIT_TEST
#include "CkcRandom2.h"
#include <iostream>
#include "../common_key/Aes.h"

using namespace cryptia;
using namespace cryptia::random;

int main(int argc, char* argv[])
{
	CkcRandom2 random(common_key::Aes::Create());
	random.Initialize(ByteArray(64, 1));
	auto random_array = random.GenerateRandomByteArray(99999);
	std::vector<int> histgram(256, 0);
	for(const auto random_byte : random_array){
		++histgram.at(random_byte);
		std::cout << static_cast<int>(random_byte) << std::endl;
	}
	/*
	for(auto h : histgram){
		std::cout << h << std::endl;
	}
	*/

    return 0;
}

#endif
