#ifdef FUNDAMENTALTYPES_UNIT_TEST
#include "FundamentalTypes.h"
#include <iostream>

using namespace cryptia;

auto TestInputHex() -> void {
	std::istringstream iss("0df0f10bf20a");
	std::ostringstream oss;
	OutputHex(oss, InputHex(iss));
	oss.str();
}

int main(int argc, char* argv[])
{
	TestInputHex();

    return 0;
}

#endif
