#ifdef BIGNATURAL_UNIT_TEST
#include "BigNatural.h"
#include <iostream>
#include <sstream>
#include "../Test.h"

using namespace cryptia;
using namespace cryptia::asymmetric_key;

using BigInteger = BigNatural;

auto BigNaturalToDebugString(const BigNatural& target) -> std::string {
	std::ostringstream oss;
	target.Output(oss);
	oss << std::flush;
	return oss.str();
}

auto TestBigNaturalConstructor1() -> void {
	CRYPTIA_CHECK_EQUAL(BigInteger(300), 300);
	CRYPTIA_CHECK_EQUAL(BigNaturalToDebugString(BigInteger(65535)), "{65535}");
	CRYPTIA_CHECK_EQUAL(BigNaturalToDebugString(BigInteger(65536)), "{1 0}");
	CRYPTIA_CHECK_EQUAL(BigNaturalToDebugString(BigInteger(65536*2)), "{2 0}");
}

auto TestBigNaturalConstructor2() -> void {
	CRYPTIA_CHECK_EQUAL(BigNaturalToDebugString(BigNatural("300")), "{300}");
	CRYPTIA_CHECK_EQUAL(BigNaturalToDebugString(BigNatural("65535")), "{65535}");
	CRYPTIA_CHECK_EQUAL(BigNaturalToDebugString(BigNatural("65536")), "{1 0}");
	CRYPTIA_CHECK_EQUAL(BigNaturalToDebugString(BigNatural("1208925819614629174706176")), "{1 0 0 0 0 0}");
}

auto TestBigNaturalConstructor3() -> void {
	CRYPTIA_CHECK_EQUAL(BigNaturalToDebugString(BigNatural(ByteArray({0x00, 0x01}))), "{1}");
	CRYPTIA_CHECK_EQUAL(BigNaturalToDebugString(BigNatural(ByteArray({0xff, 0xff}))), "{65535}");
}

auto TestBigNaturalToByteArray() -> void {
	CRYPTIA_CHECK_EQUAL(BigNatural("1").ToByteArray(), 
		ByteArray({0x00, 0x01}));
	CRYPTIA_CHECK_EQUAL(BigNatural("65535").ToByteArray(), 
		ByteArray({0xff, 0xff}));
	CRYPTIA_CHECK_EQUAL(BigNatural(65536+65535).ToByteArray(), 
		ByteArray({0x00, 0x01, 0xff, 0xff}));
}

auto TestBigNaturalPlus() -> void {
	CRYPTIA_CHECK_EQUAL(BigNaturalToDebugString(
		BigNatural("300")+BigNatural("20")), "{320}");
	CRYPTIA_CHECK_EQUAL(BigNaturalToDebugString(
		BigNatural("65535")+BigNatural("65535")), "{1 65534}");
	CRYPTIA_CHECK_EQUAL(BigNaturalToDebugString(
		BigNatural("65535")+BigNatural("65536")), "{1 65535}");
}

auto TestBigNaturalMinus() -> void {
	CRYPTIA_CHECK_EQUAL(BigNaturalToDebugString(
		BigNatural("300")-BigNatural("20")), "{280}");
	CRYPTIA_CHECK_EQUAL(BigNaturalToDebugString(
		BigNatural("65535")-BigNatural("65535")), "{0}");
	CRYPTIA_CHECK_EQUAL(BigNaturalToDebugString(
		BigNatural("65536")-BigNatural("65535")), "{1}");
	CRYPTIA_CHECK_EQUAL(BigNaturalToDebugString(
		BigNatural("65537")-BigNatural("2")), "{65535}");
}

auto TestBigNaturalMultiply() -> void {
	CRYPTIA_CHECK_EQUAL(BigNaturalToDebugString(
		BigNatural("123456789")*BigNatural("123456789")), "{54 9762 38712 41913}");	
}

auto TestBigNaturalShiftAlttle() -> void {
	BigNatural num = BigNatural("65535");
	num.ShiftLeftAlittle(2);
	CRYPTIA_CHECK_EQUAL(num, BigNatural("65535")*4);	
}

auto TestBigNaturalShift() -> void {
	CRYPTIA_CHECK_EQUAL(BigNatural("123") << 1, BigNatural("246"));
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") << 0, BigNatural("1234567890123"));
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") << 1, BigNatural("1234567890123")*2);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") << 2, BigNatural("1234567890123")*4);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") << 3, BigNatural("1234567890123")*8);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") << 4, BigNatural("1234567890123")*16);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") << 5, BigNatural("1234567890123")*32);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") << 6, BigNatural("1234567890123")*64);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") << 7, BigNatural("1234567890123")*128);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") << 8, BigNatural("1234567890123")*256);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") << 9, BigNatural("1234567890123")*512);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") << 10, BigNatural("1234567890123")*1024);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") << 11, BigNatural("1234567890123")*2048);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") << 12, BigNatural("1234567890123")*4096);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") << 15, BigNatural("1234567890123")*32768);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") << 16, BigNatural("1234567890123")*65536);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") << 17, BigNatural("1234567890123")*65536*2);
	
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") >> 0, BigNatural("1234567890123"));
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") >> 1, BigNatural("1234567890123")/2);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") >> 2, BigNatural("1234567890123")/4);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") >> 3, BigNatural("1234567890123")/8);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") >> 4, BigNatural("1234567890123")/16);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") >> 5, BigNatural("1234567890123")/32);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") >> 6, BigNatural("1234567890123")/64);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") >> 7, BigNatural("1234567890123")/128);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") >> 8, BigNatural("1234567890123")/256);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") >> 9, BigNatural("1234567890123")/512);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") >> 10, BigNatural("1234567890123")/1024);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") >> 11, BigNatural("1234567890123")/2048);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") >> 12, BigNatural("1234567890123")/4096);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") >> 13, BigNatural("1234567890123")/8192);
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") >> 14, BigNatural("1234567890123")/(8192*2));
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") >> 15, BigNatural("1234567890123")/(8192*4));
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") >> 16, BigNatural("1234567890123")/(8192*8));
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") >> 17, BigNatural("1234567890123")/(8192*16));
	CRYPTIA_CHECK_EQUAL(BigNatural("1234567890123") >> 18, BigNatural("1234567890123")/(8192*32));
}

auto TestBigNaturalDivide() -> void {
	CRYPTIA_CHECK_EQUAL(BigNaturalToDebugString(
		BigNatural("10")/BigNatural("3")), "{3}");	
	CRYPTIA_CHECK_EQUAL(BigNaturalToDebugString(
		BigNatural("123456789")/BigNatural("1234567")), "{100}");	
}

auto TestBigNaturalModulate() -> void {
	CRYPTIA_CHECK_EQUAL(BigNaturalToDebugString(
		BigNatural("10")%BigNatural("3")), "{1}");	
}

auto TestBigNaturalBitAnd() -> void {
	//CRYPTIA_CHECK_EQUAL(BigNatural(1)&BigNatural(2), BigNatural(0));	
	//CRYPTIA_CHECK_EQUAL(BigNatural(3)&BigNatural(1), BigNatural(1));	
}

auto TestBigNaturalPower1() -> void {
	CRYPTIA_CHECK_EQUAL(
		BigNatural::Power(BigNatural("123456789"), 3), 
		BigNatural("1881676371789154860897069"));	
}

auto TestBigNaturalPower2() -> void {
	/*
	CRYPTIA_CHECK_EQUAL(
		BigNatural::Power(BigNatural("123456"), BigNatural("123456")), 
		BigNatural("1881676371789154860897069"));	
	*/
	//BigNatural::Power(BigNatural("123456"), BigNatural("123456")), 
}

auto TestBigNaturalPowerModulate1() -> void {
	CRYPTIA_CHECK_EQUAL(
		BigNatural::PowerModulate(BigNatural("123456789"), 3, 1234), 
		BigNatural("817"));	
}

auto TestBigNaturalPowerModulate2() -> void {
	CRYPTIA_CHECK_EQUAL(
		BigNatural::PowerModulate(BigNatural("1234567"), BigNatural("1234567"), BigNatural("123456789")), 
		BigNatural("46006624"));	
}

auto TestBigNaturalOutput() -> void {
	std::cout << BigInteger("31") << std::endl;
	std::cout << BigInteger("1000000000") << std::endl;
	std::cout << BigInteger("30")+BigInteger("300") << std::endl;
	std::cout << BigInteger("30")-BigInteger("300") << std::endl;	
}

auto TestBigNaturalInput() -> void {
	std::istringstream iss("12345");
	BigInteger num(0);
	iss >> num;
	std::cout << num;
}

auto TestBigNaturalAppendFigure() -> void {
	auto num = BigInteger("12345");
	num.Output(std::cout);
	std::cout << num << std::endl;
	num.AppendFigureLower(1);
	num.Output(std::cout);
	std::cout << num << std::endl;
	num.AppendFigureUpper(2);
	num.Output(std::cout);
	std::cout << num << std::endl;	
}

int main(int argc, char* argv[])
{
	TestBigNaturalConstructor1();
	TestBigNaturalConstructor2();
	TestBigNaturalConstructor3();
	TestBigNaturalToByteArray();
	TestBigNaturalPlus();
	TestBigNaturalMinus();
	TestBigNaturalMultiply();
	TestBigNaturalDivide();
	//TestBigNaturalShiftAlittle();
	TestBigNaturalShift();
	TestBigNaturalModulate();
	//TestBigNaturalBitAnd();
	TestBigNaturalPower1();
	//TestBigNaturalPower2();
	TestBigNaturalPowerModulate1();
	TestBigNaturalPowerModulate2();
	//TestBigNaturalOutput();
	//TestBigNaturalInput();
	return 0;
}

#endif
