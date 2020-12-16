#include "utility.h"



bool test_checkRelativePos()
{
	print("Starte Test: ");
	println("test_checkRelativePos");
	consoleTabIn();
	bool fail = false;
	unsigned int cycles = 10000;
	unsigned int successCounter = 0;
	
	byte maxPos = 128;
	byte homePos;
	byte relativePos;
	byte absolutPos;
	
	for(unsigned int cycle=0; cycle<cycles; cycle++)
	{
		homePos = rand() % maxPos;
		writeRegister(SET_HOME,homePos);
		//delay(100);
		/*if(!readRegister(SET_HOME,homePos))
		{
			println("can't read Register: SET_HOME");
			fail = true;
			goto breakTest;
		}
		if(!readRegister(RETURN_REL_POSITION,relativePos))
		{
			println("can't read Register: RETURN_REL_POSITION");
			fail = true;
			goto breakTest;
		}
		if(!readRegister(RETURN_ABS_POSITION,absolutPos))
		{
			println("can't read Register: RETURN_ABS_POSITION");
			fail = true;
			goto breakTest;
		}*/
		//delay(100);
		if(!readAllPositions(absolutPos,homePos,relativePos))
		{
			println("can't read Register: RETURN_ALL_POSITIONS");
			fail = true;
			goto breakTest;
		}
		if(homePos+relativePos - absolutPos != 0 && homePos+relativePos - absolutPos != 128)
		{
			println("Positionsmessung ist falsch");
			print("homePos: ");
			print(homePos);
			print("\trelPos: ");
			print(relativePos);
			print("\tabsPos: ");
			print(absolutPos);
			print("\tdiv: ");
			println(homePos+relativePos - absolutPos);
		}
		else
		{
			successCounter++;
		}
		if(cycle % (cycles/10) == 0 || cycles < 10)
		{
			print("=");
		}
	}
	print(" done ");
	
	
	breakTest:
	print(successCounter);
	println("\trequests sent and received");
	writeRegister(SET_HOME,0);
	
	consoleTabOut();
	if(!fail && successCounter == cycles)
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




