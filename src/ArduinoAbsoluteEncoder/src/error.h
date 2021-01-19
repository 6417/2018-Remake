#ifndef ERROR_H
#define ERROR_H

#include "pch.h"

#include "Arduino.h"
#include "Stack.h"

namespace ERROR
{
	enum ErrorCode
	{
		NO_ERROR = 0x00,
		BAD_I2C_REGISTER_REQUEST = 0xf2,
		BAD_I2C_REGISTER_ACCESS = 0x03,
		I2C_TO_MUCH_DATA_RECIEVED = 0x04,
		I2C_TO_LESS_DATA_RECIEVED = 0xf5,
		INVALID_CRC = 0xf6
	};

	static Stack<ErrorCode, 4> errorStack;

	void clear();
	void setError(ErrorCode code);

	void throw__i2c_badRegisterRequest();
	void throw__i2c_badRegisterAccess();
	void throw__i2c_toMuchDataRecieved();
	void throw__i2c_toLessDataRecieved();
	void throw__i2c_transmitionFailed();
	void throw__i2c_invalidCRC();
}; // namespace ERROR

#endif