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
    byte calcCRC(Array<byte> data) noexcept;
    void readData(Array<byte> &data, byte length);
    void checkCRC(byte recCRC, byte CRC);
    Array<byte> createData(Array<byte> data, byte error = 0x00);

public:
    AbsoluteEncoderI2C() = default;
    void write(Array<byte> data) __attribute__((noinline));
    void writeError(byte error);
    Array<byte> read();
};

extern AbsoluteEncoderI2C i2c;

#endif