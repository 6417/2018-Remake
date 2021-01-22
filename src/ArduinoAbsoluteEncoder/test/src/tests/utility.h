#ifndef UTILITY_H
#define UTILITY_H
#include "Arduino.h"
#include "print.h"
#include "AbsoluteEncoderI2C-Master.h"
#include "Stack.h"

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

Stack<uint8_t, 2> errorStack;

byte getError();
void printError(uint8_t err);
bool readRegister(uint8_t reg, byte &retValue);
bool writeRegister(uint8_t reg, byte value);
bool checkForCRCError();
bool checkForIncomingError(uint8_t error);

bool checkForIncomingError(uint8_t error)
{
	if (error)
	{
		errorStack.push(error);
		return false;
	}
	return true;
}

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
	Stack<byte, 2> slaveErrorStack;	
	byte returnError = 0;
	
	println("Check for errors");
	consoleTabIn();
	delay(10);
	slaveErrorStack.push(i2c.read(Rquest::RETURN_LATEST_ERROR_ON_STACK, 2)[1]);
	delay(10);
	slaveErrorStack.push(i2c.read(Rquest::RETURN_LATEST_ERROR_ON_STACK, 2)[1]);

	if (!errorStack.isEmpty())
		errorStack.push(slaveErrorStack.pop());
	else
	{
		errorStack.push(slaveErrorStack.pop());
		errorStack.push(slaveErrorStack.pop());
	}

    delay(10);

    print("Current Error: ");
    printError(errorStack.peek());
    print("Last Error:    ");
    printError(errorStack.peek(1));

	returnError = errorStack.pop();
	if (!returnError)
		returnError = errorStack.pop();

    delay(10);
    
	if(returnError)
	{
		//printError(err);
		println("Waiting for red blink");
		delay(8000);
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
		case ERROR::NO_ERROR: println("No error"); break;
		case ERROR::BAD_I2C_REGISTER_REQUEST: println("BAD_I2C_REGISTER_REQUEST"); break;
		case ERROR::BAD_I2C_REGISTER_ACCESS: println("BAD_I2C_REGISTER_ACCESS"); break;
		case ERROR::I2C_TO_MUCH_DATA_RECIEVED: println("I2C_TO_MUCH_DATA_REVIEVED"); break;
		case ERROR::I2C_TO_LESS_DATA_RECIEVED: println("I2C_TO_LESS_DATA_REVIEVED"); break;
		case ERROR::INVALID_CRC: println("INVAILD_CRC"); break;
		default: print("Error not known. Code: "); println(err);
	}
}


bool readRegister(uint8_t reg, byte &retValue)
{
	bool success = true;
	errorStack.clear();
	
	Array<byte> recievedBytes = i2c.read(reg, 2);
	
    if(recievedBytes.size != 2)
    {
		success = false;
		println("");
		print("[readRegister] wrong byteAmount recieved from register: ");
		print(reg);
		print(" bytes: ");
		println(recievedBytes.size);
		return success;
    }

	retValue = recievedBytes[1];
	success = success && checkForCRCError();

	success = success && checkForIncomingError(recievedBytes[0]);
	
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
		print("[readAllPositions] wrong byteAmount recieved from register: ");
		print(RETURN_ALL_POSITIONS);
		print(" bytes: ");
		println(recievedBytes.size);
		return success;
	}

    absPos  = recievedBytes[1];
	homePos = recievedBytes[2];
	relPos  = recievedBytes[3];

	success = success && checkForCRCError();

	success = success && checkForIncomingError(recievedBytes[0]);
	
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
		print("[readAllRegisters] wrong byteAmount recieved from register: ");
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
	
	success = success && checkForCRCError();

	success = success && checkForIncomingError(recievedBytes[0]);
	
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








