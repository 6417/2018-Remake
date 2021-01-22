#ifndef UTILITY_H
#define UTILITY_H
#include "Arduino.h"
#include "print.h"
#include "AbsoluteEncoderI2C-Master.h"

// Error codes
namespace ERROR
{
  const uint8_t NO_ERROR = 0x00;
  const uint8_t BAD_I2C_REGISTER_REQUEST = 0xf2;
  const uint8_t BAD_I2C_REGISTER_ACCESS = 0x03;
  const uint8_t I2C_TO_MUCH_DATA_RECIEVED = 0x04;
  const uint8_t I2C_TO_LESS_DATA_RECIEVED = 0xf5;
  const uint8_t INVALID_CRC = 0xf6;
};

enum Rquest {
    RETURN_ABS_POSITION          = 0x00,
    SET_HOME                     = 0x01,
    RETURN_REL_POSITION          = 0x02,
    RETURN_LATEST_ERROR_ON_STACK = 0x10, 
    CLEAR_ERROR                  = 0x11,
    RETURN_VERSION               = 0x20,
    INITIALZE_TEST               = 0x40,
	    
	RETURN_ALL_POSITIONS         = 0x30,
	RETURN_ALL_REGISTERS         = 0x31
};

byte getError();
void printError(uint8_t err);
bool readRegister(uint8_t reg, byte &retValue);
bool writeRegister(uint8_t reg, byte value);
bool checkForCRCError();

bool checkForCRCError() 
{
	if (AbsoluteEncoderI2C::Error::invalidCRCOcoured)
	{
		AbsoluteEncoderI2C::Error::invalidCRCOcoured = false;
		return true;
	}
	return false;
}

byte getError()
{
	byte lastError = 0;
	byte currentError = 0;
	
	byte returnError = 0;
	
	println("Check for errors");
	consoleTabIn();
	delay(10);
    Wire.beginTransmission(address);
    Wire.write();
    // Wire.write(0x01);
    Wire.endTransmission();
    byte recievedBytes = Wire.requestFrom(address, 1);      // request 1 bytes from slave device
    // print(recievedBytes);
    // println(" bytes empfangen");
  
    while (Wire.available()) { // slave may send less than requested
      lastError = Wire.read(); // receive a byte as character
    }
    delay(10);

  
    Wire.beginTransmission(address);
    Wire.write(RETURN_CURRENT_ERROR);
    // Wire.write(0x01);
    Wire.endTransmission();
    recievedBytes = Wire.requestFrom(address, 1);      // request 1 bytes from slave device
    // print(recievedBytes);
    // println(" bytes empfangen");
  
    while (Wire.available()) { // slave may send less than requested
        currentError = Wire.read(); // receive a byte as character
    }
    print("Current Error: ");
    printError(currentError);
    print("Last Error:    ");
    printError(lastError);
    
    returnError = lastError;
    if(currentError)
		returnError = currentError;
  
    delay(10);
    
	if(currentError || lastError)
	{
		//printError(err);
		println("Waiting for red blink");
		delay(8000);
		Wire.beginTransmission(address);
		Wire.write(CLEAR_ERROR);
		// Wire.write(0x01);
		Wire.endTransmission();
		delay(10);
	}
	consoleTabOut();
	return returnError;
}

void printError(uint8_t err)
{
	print("ERROR Code: ");
	print(err);
	print("\t");
	switch(err)
	{
		case 0: println("No error"); break;
		case ERROR::BAD_I2C_REGISTER_REQUEST: println("BAD_I2C_REGISTER_REQUEST"); break;
		case ERROR::BAD_I2C_REGISTER_ACCESS: println("BAD_I2C_REGISTER_ACCESS"); break;
		case ERROR::I2C_TO_MUCH_DATA_RECIEVED: println("I2C_TO_MUCH_DATA_REVIEVED"); break;
		case ERROR::I2C_TO_LESS_DATA_RECIEVED: println("I2C_TO_LESS_DATA_REVIEVED"); break;
		default: print("Error not known. Code: "); println(err);
	}
}


bool readRegister(uint8_t reg, byte &retValue)
{
	bool success = true;
	
	Array<byte> recievedBytes = i2c.read(reg, 2);
	
    if(recievedBytes.size != 2)
    {
		success = false;
		println("");
		print("wrong byteAmount recieved from register: ");
		print(reg);
		print(" bytes: ");
		println(recievedBytes.size);
		return success;
    }

	retValue = recievedBytes[1];
<<<<<<< HEAD:src/ArduinoAbsoluteEncoder/test/tests/utility.h
	if (AbsoluteEncoderI2C::Error::invalidCRCOcoured)
		success = false;
=======
	success = success && checkForCRCError();

>>>>>>> 7221cf17772f690ffc9c06850a3d93ac64f37777:src/ArduinoAbsoluteEncoder/test/src/tests/utility.h
	if (!recievedBytes[0])
		success = false;
	
	return success;
}
bool readAllPositions(byte &absPos,byte &homePos, byte &relPos)
{
	bool success = true;
	Array<byte> recievedBytes = i2c.read(Rquest::RETURN_ALL_POSITIONS);
	
    if(recievedBytes.size != 4)
    {
		success = false;
		println("");
		print("wrong byteAmount recieved from register: ");
		print(RETURN_ALL_POSITIONS);
		print(" bytes: ");
		println(recievedBytes.size);
		return success;
	}

    absPos  = recievedBytes[1];
	homePos = recievedBytes[2];
	relPos  = recievedBytes[3];

	if (!recievedBytes[0])
		success = false;
<<<<<<< HEAD:src/ArduinoAbsoluteEncoder/test/tests/utility.h
=======

	success = success && checkForCRCError();
>>>>>>> 7221cf17772f690ffc9c06850a3d93ac64f37777:src/ArduinoAbsoluteEncoder/test/src/tests/utility.h
	
	return success;
}
bool readAllRegisters(byte &absPos,byte &homePos, byte &relPos,
					  byte &currentError,byte &lastError, byte &version)
{
	bool success = true;
	
	Array<byte> recievedBytes = i2c.read(Rquest::RETURN_ALL_POSITIONS, 7);

    if(recievedBytes.size != 7)
    {
		success = false;
		println("");
		print("wrong byteAmount recieved from register: ");
		print(RETURN_ALL_REGISTERS);
		print(" bytes: ");
		println(recievedBytes.size);
		return success;
    }
    absPos  		= recievedBytes[1];
	homePos 		= recievedBytes[2];
	relPos  		= recievedBytes[3];
	currentError  	= recievedBytes[4];
	lastError 		= recievedBytes[5];
	version  		= recievedBytes[6];
	
	if (!recievedBytes[0])
		success = false;
	
	success = success && checkForCRCError;
	
	return success;
}
bool writeRegister(uint8_t reg, byte value)
{
	Array<byte> sendBuffer(2);
	sendBuffer[0] = reg;
	sendBuffer[1] = value;
	i2c.write(sendBuffer);
	return true;
}

#endif








