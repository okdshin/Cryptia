#ifdef CTRMODE_UNIT_TEST
#include "CtrMode.h"
#include <iostream>

using namespace cryptia;
using namespace cryptia::common_key;
using namespace cryptia::common_key::mode;

int main(int argc, char* argv[])
{
	TestCtrModeAesEncrypt();
    return 0;
}

#endif
