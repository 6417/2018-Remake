#ifndef ERROR_H
#define ERROR_H

#include "Arduino.h"

namespace ERROR
{
	static volatile bool errorOccured = false;
	static volatile uint8_t currentError = 0;
	static uint8_t lastError             = 0;
	
	const uint8_t BAD_I2C_REGISTER_REQUEST 		= 2;
	const uint8_t BAD_I2C_REGISTER_ACCESS  		= 3;
	const uint8_t I2C_TO_MUCH_DATA_RECIEVED  	= 4;
	const uint8_t I2C_TO_LESS_DATA_RECIEVED  	= 5;
	
	void clear();
	void setError(uint8_t code);
	
	void throw__i2c_badRegisterRequest();
	void throw__i2c_badRegisterAccess();
	void throw__i2c_toMuchDataRecieved();
	void throw__i2c_toLessDataRecieved();

	
};



void ERROR::clear()
{
	if(currentError != 0)
	{
		errorOccured = false;
		lastError    = currentError;
		currentError = 0;
	}
}
void ERROR::setError(uint8_t code)
{
	clear();
	currentError = code;
	errorOccured = true;
}


void ERROR::throw__i2c_badRegisterRequest()
{
	setError(BAD_I2C_REGISTER_REQUEST);
	#ifdef DEBUG
		Serial.println("ERROR: Bad I2C register request");
    #endif
}
void ERROR::throw__i2c_badRegisterAccess()
{
	setError(BAD_I2C_REGISTER_ACCESS);
	#ifdef DEBUG
		Serial.println("ERROR: Bad I2C register access");
    #endif
}
void ERROR::throw__i2c_toMuchDataRecieved()
{
	setError(I2C_TO_MUCH_DATA_RECIEVED);
	#ifdef DEBUG
		Serial.println("ERROR: I2C to much data recieved");
    #endif
}
void ERROR::throw__i2c_toLessDataRecieved()
{
	setError(I2C_TO_LESS_DATA_RECIEVED);
	#ifdef DEBUG
		Serial.println("ERROR: I2C to less data recieved");
    #endif
}

#endif