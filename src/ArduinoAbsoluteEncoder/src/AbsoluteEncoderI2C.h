#ifndef ABSOLUTE_ENCODER_I2C_H
#define ABSOLUTE_ENCODER_I2C_H

#include "pch.h"

#include <Wire.h>
#include <Arduino.h>
#include <error.h>

template <typename T>
class Array
{
public:
    T buffer[10];
    int size = 0;
    Array(T *data, int size) __attribute__((noinline));
    Array(int size) __attribute__((noinline));
    Array() = default;
    T &operator[](int index) __attribute__((noinline));
    Array<T> slice(byte offset, byte length) __attribute__((noinline));
};

class AbsoluteEncoderI2C
{
private:
    byte SEQ;
    bool sendErrorThroughRegister = true;
    byte calcCRC(Array<byte> data) noexcept;
    void readData(Array<byte> &data, byte length);
    void checkCRC(byte recCRC, byte CRC);
    void setRegisterToError(Array<byte>& sendBuffer);
    Array<byte> createData(Array<byte> data);

public:
    AbsoluteEncoderI2C() = default;
    void write(Array<byte> data);
    void write(byte data);
    void writeCurrentCriticalError();
    void disableSendErrorThoughRegister();
    void enableSendErrorThroughRegister();
    Array<byte> read();
};

extern AbsoluteEncoderI2C i2c;

#endif