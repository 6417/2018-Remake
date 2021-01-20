#ifndef ABSOLUTE_ENCDER_I2C_MASTER_H
#define ABSOLUTE_ENCDER_I2C_MASTER_H

#include "pch.h"

#include <Wire.h>

typedef unsigned char byte;

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
    Array<byte> createData(Array<byte> data);

public:
    struct Error // used as an inner namespace
    {
        static bool invalidCRCOcoured;
        static void throwInvalidCRC();
    };

    AbsoluteEncoderI2C() = default;
    void write(Array<byte> data);
    void write(byte data);
    Array<byte> read(byte numOfBytes);
    Array<byte> read(byte reg, byte numOfBytes);
};

extern AbsoluteEncoderI2C i2c;

#endif 