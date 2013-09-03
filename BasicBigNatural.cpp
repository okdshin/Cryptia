#ifdef BASICBIGNATURAL_UNIT_TEST
#include "BasicBigNatural.h"
#include <iostream>
#include <sstream>

using namespace cryptia;

//using BigInteger = BasicBigNatural<65535>;
//using BigInteger = BasicBigNatural<214748364>;
//using BigInteger = BasicBigNatural<2147483647>;
using BigInteger = BasicBigNatural<7>;

auto Check(const BigInteger& target, const std::string& collect_answer, const std::string& message ="") -> void {
	std::ostringstream oss;
	target.Output(oss);
	oss << std::flush;
	if(oss.str() != collect_answer){
		std::cout << message << "\"" << oss.str() << "\" != \"" 
			<< collect_answer << "\"" << std::endl;
	}
}

auto TestConstructor1() -> void {
	auto num = BigInteger(300);	
	Check(num, "{6 0 6}");
}

auto TestConstructor2() -> void {
	Check(BigInteger("49"), "{1 0 0}");
	Check(BigInteger("343"), "{1 0 0 0}");
	Check(BigInteger("344"), "{1 0 0 1}");
	Check(BigInteger("348"), "{1 0 0 5}");
	Check(BigInteger("22539340290692258087863249"), 
		"{1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0}");
}

auto TestPlus() -> void {
	auto num = BigInteger(30);
	auto num2 = BigInteger(300);
	Check(num+num, "{1 1 4}");
	Check(num+num2, "{6 5 1}");
}

auto TestMinus() -> void {
	auto num = BigInteger(30);
	auto num2 = BigInteger(300);
	Check(num-num, "{0}");
	Check(num2-num, "{5 3 4}");
}

auto TestMultiply() -> void {
	auto num = BigInteger(30);
	auto num2 = BigInteger(4);
	Check(num*num2, "{2 3 1}");	
}

auto TestDivide() -> void {
	auto num = BigInteger(30);
	auto num2 = BigInteger(4);
	Check(num/num2, "{1 0}");	
}

auto TestModulate() -> void {
	auto num = BigInteger("1234000");
	auto num2 = BigInteger("1234");
	std::cout << (num % num2) << std::endl;
	std::cout << (num / num2) << std::endl;
}

auto TestPower1() -> void {
	auto num = BigInteger("12345");
	BigInteger::Power(num, 12345).Output(std::cout);
}

auto TestPower2() -> void {
	/*
	auto num = BigInteger(30);
	auto num2 = BigInteger(-30);
	Check(BigInteger::Power(num, BigInteger(3)), "{1 4 1 5 0 1}");
	Check(BigInteger::Power(num, BigInteger(3)), "-{1 4 1 5 0 1}");
	std::cout << BigInteger::Power(num, num) << std::endl;
	*/
	auto num3 = BigInteger("12345");
	auto num4 = BigInteger("12345");
	BigInteger::Power(num3, num4).Output(std::cout);
	/*
	//std::cout << 
	std::cout << std::endl;
	auto num5 = BigInteger("12345");
	auto num6 = BigInteger("12345");
	BigInteger::Power(num5, num6).Output(std::cout);
	std::cout << std::endl;
	*/
}

auto TestPowerModulate1() -> void {
	auto num = BigInteger("13407807929942597099574024998205846127479365820592393377723561443721764030073546976801874298166903427690031858186486050853753882811946569946433649006084093");
	BigInteger::PowerModulate(num, 12345, 1234).Output(std::cout);
}

auto TestPowerModulate2() -> void {
	auto base = BigInteger("13407807929942597099574024998205846127479365820592393377723561443721764030073546976801874298166903427690031858186486050853753882811946569946433649006084093");
	auto exponential = BigInteger("65535");
	auto num = BigInteger("23407807929942597099574024998205846127479365820592393377723561443721764030073546976801874298166903427690031858186486050853753882811946569946433649006084093");
	BigInteger::PowerModulate(base, exponential, num).Output(std::cout);
}

auto TestOutput() -> void {
	std::cout << BigInteger("31") << std::endl;
	std::cout << BigInteger("1000000000") << std::endl;
	std::cout << BigInteger("30")+BigInteger("300") << std::endl;
	std::cout << BigInteger("30")-BigInteger("300") << std::endl;	
}

auto TestInput() -> void {
	std::istringstream iss("12345");
	BigInteger num(0);
	iss >> num;
	std::cout << num;
}

auto DebugTest1() -> void {
	BigInteger a(1234), b(12345);
	std::cout << a*b << std::endl;
	std::cout << a*b << std::endl;
}

auto TestShift() -> void {
	auto num = BigInteger("12345");
	num.Output(std::cout);
	std::cout << num << std::endl;
	num.ShiftFigureLeft(1);
	num.Output(std::cout);
	std::cout << num << std::endl;
	num.ShiftFigureRight(2);
	num.Output(std::cout);
	std::cout << num << std::endl;
	
}

int main(int argc, char* argv[])
{
	TestConstructor1();
	TestConstructor2();
	TestPlus();
	TestMinus();
	TestMultiply();
	TestDivide();
	/*
	DebugTest1();
	TestModulate();
	TestPower1();
	TestPower2();
	TestPowerModulate1();
	TestPowerModulate2();
	*/
	//TestOutput();
	//TestInput();
	return 0;
}

#endif
