#include "utility.h"



bool test_readRelativeEncoderValue_muchData()
{
	print("Starte Test: ");
	println("test_readRelativeEncoderValue_muchData");
	consoleTabIn();
	unsigned int cycles = 2;
	int successCounter = 0;
	bool fail = false;
	
	for(int cycle=0; cycle<cycles; cycle++)
	{
		// Sending data to a register, which shuldn't get data
		writeRegister(RETURN_REL_POSITION, 1);
	
		byte response;
		if(!readRegister(RETURN_REL_POSITION,response))
		{
			fail = true;
			goto breakTest;
		}
		else
		{
			successCounter++;
		}
		Wire.requestFrom(address, 1);      // request 1 bytes from slave device
	
		
		if(getError() != ERROR::I2C_TO_MUCH_DATA_RECIEVED)
			fail = 1;
		
		/*if(cycle % (cycles/10) == 0 || cycles < 10)
		{
			print("=");
		}*/
		delay(500);
	}
	breakTest:
	print(" done ");
	print(successCounter);
	println("\trequests sent and received");
	
	consoleTabOut();
	if(successCounter == 2 && !fail)
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




