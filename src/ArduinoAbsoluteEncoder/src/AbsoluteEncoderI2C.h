#ifndef ABSOLUTE_ENCODER_I2C_H
#define ABSOLUTE_ENCODER_I2C_H

typedef unsigned char byte;

#define DEBUG

#include <Wire.h>
#include <Arduino.h>

template <typename T>
class Array
{
public:
    T buffer[10];
    int size = 0;
    Array(T *data, int size);
    Array(int size);
    Array() = default;
    T &operator[](int index);
    T operator[](int index) const;
};

namespace Exception
{
    namespace Handler
    {
        void invalidCRC();
    }

    enum ErrorCodes
    {
        NO_ERROR = 0,
        INVALID_CRC = 1
    };

    extern ErrorCodes error;

    typedef void (*ErrorHandler)(void);

    extern ErrorHandler handlerList[];
}

class AbsoluteEncoderI2C
{
private:
    byte SEQ;
    byte calcCRC(Array<byte> data) noexcept;

public:
    AbsoluteEncoderI2C() = default;
    void write(Array<byte> data);
    void write(byte data);
    void writeError(byte errorCode);
    Array<byte> read();
};

extern AbsoluteEncoderI2C i2c;

#endif