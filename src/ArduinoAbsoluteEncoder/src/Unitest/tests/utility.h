#ifndef UTILITY_H
#define UTILITY_H
#include "Arduino.h"
#include "print.h"

// Error codes
namespace ERROR
{
  const uint8_t BAD_I2C_REGISTER_REQUEST 	= 2;
  const uint8_t BAD_I2C_REGISTER_ACCESS  	= 3;
  const uint8_t I2C_TO_MUCH_DATA_REVIEVED  	= 4;
  const uint8_t I2C_TO_LESS_DATA_REVIEVED  	= 5;
};

const int address = 1;
enum Rquest {
    RETURN_ABS_POSITION         = 0x00,
    SET_HOME                    = 0x01,
    RETURN_REL_POSITION         = 0x02,
    RETURN_CURRENT_ERROR        = 0x10,
    RETURN_LAST_ERROR           = 0x11,
    CLEAR_ERROR                 = 0x12,
    RETURN_VERSION              = 0x20,
	
	RETURN_ALL_POSITIONS		= 0x30,
	RETURN_ALL_REGISTERS        = 0x31
};

byte getError();
void printError(uint8_t err);
bool readRegister(uint8_t reg, byte &retValue);
bool writeRegister(uint8_t reg, byte value);

byte getError()
{
	byte lastError = 0;
	byte currentError = 0;
	
	byte returnError = 0;
	
	println("Check for errors");
	consoleTabIn();
	delay(10);
    Wire.beginTransmission(address);
    Wire.write(RETURN_LAST_ERROR);
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
		case ERROR::I2C_TO_MUCH_DATA_REVIEVED: println("I2C_TO_MUCH_DATA_REVIEVED"); break;
		case ERROR::I2C_TO_LESS_DATA_REVIEVED: println("I2C_TO_LESS_DATA_REVIEVED"); break;
		default: print("Error not known. Code: "); println(err);
	}
}


bool readRegister(uint8_t reg, byte &retValue)
{
	bool success = true;
	
	Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.endTransmission();
	
	byte recievedBytes = Wire.requestFrom(address, 1); 
    if(recievedBytes != 1)
    {
		success = false;
		println("");
		print("wrong byteAmount recieved from register: ");
		print(reg);
		print(" bytes: ");
		println(recievedBytes);
		return success;
    }
    retValue = Wire.read();
	
	return success;
}
bool readAllPositions(byte &absPos,byte &homePos, byte &relPos)
{
	bool success = true;
	Wire.beginTransmission(address);
    Wire.write(RETURN_ALL_POSITIONS);
    Wire.endTransmission();
	
	byte recievedBytes = Wire.requestFrom(address, 3); 
    if(recievedBytes != 3)
    {
		success = false;
		println("");
		print("wrong byteAmount recieved from register: ");
		print(RETURN_ALL_POSITIONS);
		print(" bytes: ");
		println(recievedBytes);
		return success;
    }
    absPos  = Wire.read();
	homePos = Wire.read();
	relPos  = Wire.read();
	
	return success;
}
bool readAllRegisters(byte &absPos,byte &homePos, byte &relPos,
					  byte &currentError,byte &lastError, byte &version)
{
	bool success = true;
	Wire.beginTransmission(address);
    Wire.write(RETURN_ALL_REGISTERS);
    Wire.endTransmission();
	
	byte recievedBytes = Wire.requestFrom(address, 6); 
    if(recievedBytes != 6)
    {
		success = false;
		println("");
		print("wrong byteAmount recieved from register: ");
		print(RETURN_ALL_REGISTERS);
		print(" bytes: ");
		println(recievedBytes);
		return success;
    }
    absPos  		= Wire.read();
	homePos 		= Wire.read();
	relPos  		= Wire.read();
	currentError  	= Wire.read();
	lastError 		= Wire.read();
	version  		= Wire.read();
	
	return success;
}
bool writeRegister(uint8_t reg, byte value)
{
	Wire.beginTransmission(address);
    Wire.write(reg);
	Wire.write(value);
    Wire.endTransmission();
	return true;
}

#endif








