#include "utility.h"





bool test_wrongRequest()
{
	print("Starte Test: ");
	println("test_wrongRequest");
	consoleTabIn();
	
	unsigned int cycles = 2;
	byte wrongRegister  = 0xFF; 
	
	bool fail = false;
	
	
	println("Send only to wrong register ");
	for(int cycle=0; cycle<cycles; cycle++)
	{
		
		
		Wire.beginTransmission(address);
		Wire.write(wrongRegister);
	// Wire.write(0x01);
		Wire.endTransmission();
		//writeRegister(wrongRegister,0);
		
		if(getError() != ERROR::BAD_I2C_REGISTER_ACCESS)
		{
			println("ERROR != ERROR::BAD_I2C_REGISTER_ACCESS");
			fail = 1;
		}
		//delay(500);

		/*if(cycle % (cycles/10) == 0 || cycles < 10)
		{
			print("=");
		}*/
	}
	//println(" done");
	println("Send and receive to wrong register ");
	for(int cycle=0; cycle<cycles; cycle++)
	{
		writeRegister(wrongRegister,0);
		byte response = 0;
		if(!readRegister(wrongRegister,response))
		{
			fail = true;
			goto breakTest;
		}
		if(response != 0)
		{
			print("Data recieved from invalid Register: ");
			println(response);
			fail = true;
		}
		/*byte recievedBytes = Wire.requestFrom(address, 1);      // request 1 bytes from slave device
		print(recievedBytes);
		println(" bytes empfangen");
		delay(10);
		while (Wire.available()) { // slave may send less than requested
			byte c = Wire.read(); // receive a byte as character
			//println(c,DEC);         // print the character
			if(c != 0)
			{
				print("Data recieved from invalid Register: ");
				println(c);
				fail = true;
			}
		}*/
		if(getError() != ERROR::BAD_I2C_REGISTER_REQUEST)
		{
			println("ERROR != ERROR::BAD_I2C_REGISTER_REQUEST");
			fail = 1;
		}
		//delay(500);
	}
	breakTest:
	println(" done");
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


