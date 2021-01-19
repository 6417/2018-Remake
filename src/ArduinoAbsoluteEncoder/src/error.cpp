#include "error.h"

void ERROR::clear()
{
	ERROR::errorStack.clear();
}
void ERROR::setError(ERROR::ErrorCode code)
{
	ERROR::errorStack.push(code);
}

void ERROR::throw__i2c_badRegisterRequest()
{
	setError(ERROR::ErrorCode::BAD_I2C_REGISTER_REQUEST);
	#ifdef DEBUG
		Serial.println("ERROR: Bad I2C register request");
	#endif
}
void ERROR::throw__i2c_badRegisterAccess()
{
	setError(ERROR::ErrorCode::BAD_I2C_REGISTER_ACCESS);
	#ifdef DEBUG
		Serial.println("ERROR: Bad I2C register access");
	#endif
}
void ERROR::throw__i2c_toMuchDataRecieved()
{
	setError(ERROR::ErrorCode::I2C_TO_MUCH_DATA_RECIEVED);
	#ifdef DEBUG
			Serial.println("ERROR: I2C to much data recieved");
	#endif
}
void ERROR::throw__i2c_toLessDataRecieved()
{
	setError(ERROR::ErrorCode::I2C_TO_LESS_DATA_RECIEVED);
	#ifdef DEBUG
		Serial.println("ERROR: I2C to less data recieved");
	#endif
}

void ERROR::throw__i2c_invalidCRC()
{
	setError(ERROR::ErrorCode::INVALID_CRC);
	#ifdef DEBUG
		Serial.println("ERROR: Invaid CRC");
	#endif
}