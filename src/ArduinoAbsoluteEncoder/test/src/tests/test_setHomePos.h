#include "utility.h"



bool test_setHomePos()
{
	print("Starte Test: ");
	println("test_setHomePos");
	consoleTabIn();
	
	unsigned int cycles = 10;
	int successCounter = 0;
	bool fail = false;
	
	for(int cycle=0; cycle<cycles; cycle++)
	{
		byte homePos = rand() % 256;

		writeRegister(SET_HOME,homePos);
	

		byte received;
		if(!readRegister(SET_HOME,received))
		{
			fail = true;
			goto breakTest;
		}

		
		// Die Position kann 128 nicht überschreiten und der encoder muss dies handhaben können
		if(homePos > 127)
			homePos -= 128;
		if(received == homePos)
		{
			successCounter++;
		}
		else
		{
			println("");
			print("\tFAIL\t");
			print("Wrong homePos. Is: ");
			print(received);
			print("\tshuld be: ");
			println(homePos);
			fail = true;
		}
		
		if(cycle % (cycles/10) == 0 || cycles < 10)
		{
			print("=");
		}
		
	}
	breakTest:
	print(" done ");
	print(successCounter);
	println("\trequests sent and received");
	
	consoleTabOut();
	if(!fail && successCounter == 10)
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





