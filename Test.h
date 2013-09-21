#pragma once
//Test:20130920
#include <iostream>

#define CRYPTIA_CHECK_EQUAL(a, b){\
	if(a == b){\
		std::cout << __FUNCTION__ << " is succeeded." << std::endl;\
	}\
	else{\
		std::cout << __FUNCTION__ << " is failed.\n\t"\
			<< #a << " != " << #b << std::endl;\
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
