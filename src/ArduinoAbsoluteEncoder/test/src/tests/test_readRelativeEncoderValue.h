#include "utility.h"

bool test_readRelativeEncoderValue()
{
	print("Starte Test: ");
	println("test_readRelativeEncoderValue");
	consoleTabIn();
	unsigned int successCounter = 0;
	bool fail = false;
	unsigned int cycles = 100;
	
	for(unsigned int cycle=0; cycle<cycles; cycle++)
	{
		byte relPos;
		if(!readRegister(RETURN_REL_POSITION,relPos))
		{
			fail = 1;
			println("failed to read register");
			goto breakTest;
		}
	
		disablePrints();
		byte error = getError();
		enablePrints();
		if(error != 0)
		{
			fail = 1;
			printError(error);
		}
		
	
		if(cycle % (cycles/10) == 0 || cycles < 10)
		{
			print("=");
		}
		successCounter++;
	}
	breakTest:
	print(" done ");
	print(successCounter);
	println("\trequests sent and received");
	
	consoleTabOut();
	if(!fail)
	{
		println("Test OK");
		return true;
	}
	else
	{
		println("Test FAIL");
		return false;
	}
  
}