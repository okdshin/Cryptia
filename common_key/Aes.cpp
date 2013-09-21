#ifdef AES_UNIT_TEST
#include "Aes.h"
#include <iostream>

using namespace cryptia;
using namespace cryptia::common_key;

int main(int argc, char* argv[])
{
	TestAesExpandKey128();
	TestAesExpandKey256();
	TestAesEncryptAndDecryptBlock();
	//TestAesEncryptBlock();
	return 0;
}

#endif
