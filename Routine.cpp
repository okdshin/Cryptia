#ifdef ROUTINE_UNIT_TEST
#include "Routine.h"
#include <iostream>
#include <boost/timer.hpp>
using namespace cryptia;

int main(int argc, char* argv[])
{
	auto seed = [](int n) {
	std::random_device rnd;
	std::vector<std::uint32_t> v(n);
	std::generate(v.begin(), v.end(), std::ref(rnd));
		return std::seed_seq(v.begin(), v.end());
	}(10);
	auto rand = std::bind(std::uniform_int_distribution<>(1, BigInteger(0).Base()-1), std::mt19937(seed));
	while(true){
		auto temp = BigInteger::GenerateRandomly([&rand](){
			return rand();
		}, 31);
		//auto num = temp*(temp+1)+41;
		auto num = temp*2+1;
		//std::cout << num << std::endl;
		if(MillerRabinPrimalityTest(num, 20)){
			std::cout << num << ":" << "MR" << std::endl;
		}
		std::cout << "." << std::flush;
	}
	/*
	if(FermatPrimalityTest(num)){
		std::cout << num << ":" << "FP"<< std::endl;
	}
	*/
	/*
	for(int i = 2; i < 100; ++i){
		if(FermatPrimalityTest(i)){
			std::cout << i << ":" << "FP"<< std::endl;
		}
		if(MillerRabinPrimalityTest(i, 2)){
			std::cout << i << ":" << "MR"<< std::endl;
		}
	}
	*/
    return 0;
}

#endif
