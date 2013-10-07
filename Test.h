#pragma once
//Test:20130920
#include <iostream>

#define CRYPTIA_CHECK_EQUAL(a, b){\
	if(a == b){\
		std::cout << "\033[32m" << __FUNCTION__ << "(LINE:" << __LINE__ << ") is succeeded." << "\033[39m" << std::endl;\
	}\
	else{\
		std::cout << "\033[31m" << __FUNCTION__ << "(LINE:" << __LINE__ << ") is failed.\n\t" << #a << " != " << #b \
		<< "\033[39m" << std::endl;\
	}\
}

#define CRYPTIA_CHECK_NOT_EQUAL(a, b){\
	if(a != b){\
		std::cout << __FUNCTION__ << " is succeeded." << std::endl;\
	}\
	else{\
		std::cout << __FUNCTION__ << " is failed.\n\t"\
			<< #a << " == " << #b << std::endl;\
	}\
}
