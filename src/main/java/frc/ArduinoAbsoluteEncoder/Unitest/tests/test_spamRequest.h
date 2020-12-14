#include "utility.h"



bool test_spamRequest()
{
	print("Starte Test: ");
	println("test_spamRequest");
	consoleTabIn();
	println("Do not rotate the encoder for this test");
	bool fail = false;
	
	unsigned long cycles = 50;
	unsigned long successCounter = 0;
	unsigned int interval = 20;
	unsigned int minimalInterval = 0; 
	bool millisMicros_switch = false; // false -> use millis
	
	
	
	
	/*Wire.beginTransmission(address);
	Wire.write(RETURN_REL_POSITION);
	Wire.endTransmission();
	
	Wire.requestFrom(address, 1); 
	byte expectedValue = Wire.read();  */
	// store the current encoder value as checksum  
	byte expectedValue;
	if(!readRegister(RETURN_REL_POSITION,expectedValue))
	{
		fail = true;
		goto breakTest;
	}
	
	//      Interval: 1000  us  ========== done
	println("                  |          |");
	
	do
	{
		if(interval <= minimalInterval && !millisMicros_switch)
		{
			interval = 500;
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
			byte recievedBytes = Wire.requestFrom(address, 1); 
			if(recievedBytes != 1)
			{
				fail = true;
				println("");
				print("wrong byteAmount recieved: ");
				println(recievedBytes);
				goto breakTest;
			}
			byte recieved = Wire.read();
			if(recieved != expectedValue)
			{
				fail = true;
				println("");
				print("wrong value recieved: ");
				println(recieved);
				print("Value shuld be: ");
				println(expectedValue);
				println("Do not rotate the encoder for this test");
				goto breakTest;
			}
			if(millisMicros_switch)
				delayMicroseconds(interval);
			else
				delay(interval); 
		
			if(cycle % (cycles/10) == 0 || cycles < 10)
			{
				print("=");
			}
			successCounter++;
	
		
		}
		print(" done ");
		print(cycles);
		println("\trequests sent and received");
	
		if(interval == 0)
		goto breakTest;
		interval /= 2;
		if(cycles < 25600)
		cycles   *= 2;
		
		
	}while(interval >= minimalInterval || !millisMicros_switch);
	
	
	
	breakTest:
	print("Total: ");
	print(successCounter);
	println("\trequests sent and received");
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




