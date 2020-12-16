#include "utility.h"



bool test_readAllRegisters()
{
	print("Starte Test: ");
	println("test_readAllRegisters");
	consoleTabIn();
	println("Do not rotate the encoder for this test");
	bool fail = false;
	
	unsigned long cycles = 10;
	unsigned int cycleCounter = 0;
	unsigned int interval = 5;
	unsigned int minimalInterval = 0; 
	bool millisMicros_switch = false; // false -> use millis
	
	const byte anzRegister = 5;
	byte registerList[anzRegister] = 
	{
		RETURN_ABS_POSITION,
		SET_HOME,
		RETURN_REL_POSITION,
		RETURN_CURRENT_ERROR,
		RETURN_LAST_ERROR
	};
	byte expectedList[anzRegister];
	for(byte i=0; i<anzRegister; i++)
	{
		/*Wire.beginTransmission(address);
		Wire.write(registerList[i]);
		Wire.endTransmission();
		Wire.requestFrom(address, 1); 
		expectedList[i] = Wire.read();*/
		
		if(!readRegister(registerList[i],expectedList[i]))
		{
			fail = true;
			goto breakTest;
		}
	}
	
	
	//       Interval: 1000  us  ========== done
	println("                  |          |");
	
	// Testloop
	do
	{
		if(interval <= minimalInterval && !millisMicros_switch)
		{
			interval 			= 500;
			millisMicros_switch = true;
		}
		print("Interval: ");
		print(interval);
		if(millisMicros_switch)
			print("\tus  ");
		else
			print("\tms  ");
		
		for(unsigned long cycle = 0; cycle < cycles; cycle++)
		{
			byte recieved[anzRegister];
			for(byte i=0; i<anzRegister; i++)
			{
				if(!readRegister(registerList[i],recieved[i]))
				{
					fail = true;
					goto breakTest;
				}
		
				if(recieved[i] != expectedList[i])
				{
					fail = true;
					println("");
					print("wrong value recieved: ");
					println(recieved[i]);
					print("Value shuld be: ");
					println(expectedList[i]);
					println("Do not rotate the encoder for this test");
					goto breakTest;
				}
			}
			if(millisMicros_switch)
				delayMicroseconds(interval);
			else
				delay(interval); 
		
			if(cycle % (cycles/10) == 0 || cycles < 10)
			{
				print("=");
			}
	
		
		}
		print(" done ");
		print(cycles*anzRegister);
		println("\trequests sent and received");
	
		if(interval == 0)
			goto breakTest;
		interval /= 2;
		if(cycles < 3200/anzRegister)
			cycles   *= 2;
		
		
	}while(interval >= minimalInterval || !millisMicros_switch);
	
	breakTest:
	println("");
	println("Test stoped");
	
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




