#ifdef RSA_UNIT_TEST
#include "Rsa.h"
#include <iostream>

using namespace cryptia;
using namespace cryptia::asymmetric_key;

int main(int argc, char* argv[])
{
	//TestRsaGenerateBigPrime();
	//TestRsaInitialize();
	//TestRsaEncryptAndDecrypt();
	TestRsaEncryptAndDecrypt2();

	//GreatestCommonDivisor(646, 408).Output(std::cout);
    return 0;
}

#endif
