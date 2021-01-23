#include "AbsoluteEncoderI2C-Master.h"

bool AbsoluteEncoderI2C::Error::invalidCRCOcoured = false;

AbsoluteEncoderI2C i2c = AbsoluteEncoderI2C();

byte AbsoluteEncoderI2C::calcCRC(Array<byte> data) noexcept
{
    // DO NOT CHANGE
    byte crc = 0x00;
    for (int i = 0; i < data.size; i++)
    {
        byte extract = data.buffer[i];
        for (byte tempI = 8; tempI; tempI--)
        {
            byte sum = (crc ^ extract) & 0x01;
            crc >>= 1;
            if (sum)
            {
                crc ^= 0x8C;
            }
            extract >>= 1;
        }
    }
    return crc;
}

Array<byte> AbsoluteEncoderI2C::read(byte numOfBytes) 
{
    Wire.requestFrom(address, numOfBytes + 3);
    while (!Wire.available()) {}
    
    byte recSEQ = Wire.read();

    byte length = Wire.read();
    Array<byte> data(length + 3);
    data[0] = recSEQ;
    data[1] = length;
    data[2] = Wire.read(); // reads register

    readData(data, length);
    byte CRC = Wire.read();
    checkCRC(CRC, calcCRC(data));

    return data.slice(2, data[1] + 1);
} 

Array<byte> AbsoluteEncoderI2C::read(byte reg, byte numOfBytes)
{
    write(reg);
    delay(10);
    return read(numOfBytes);
}

void AbsoluteEncoderI2C::checkCRC(byte recCRC, byte CRC)
{
    if (recCRC != CRC) 
        AbsoluteEncoderI2C::Error::throwInvalidCRC();
}

void AbsoluteEncoderI2C::Error::throwInvalidCRC()
{
    invalidCRCOcoured = true;
}

void AbsoluteEncoderI2C::readData(Array<byte>& data, byte length)
{
    for (int i = 3; i < length + 3; i++)
        data[i] = Wire.read();
}

Array<byte> AbsoluteEncoderI2C::createData(Array<byte> data)
{
    Array<byte> sendBuffer(data.size + 3);
    sendBuffer[0] = SEQ;
    sendBuffer[1] = data.size - 1;

    memcpy(&sendBuffer[2], data.buffer, data.size); // copies <data> in to the data section of <sendBuffer>
    sendBuffer[sendBuffer.size - 1] = calcCRC(sendBuffer.slice(0, sendBuffer.size - 1));
    return sendBuffer;
}

void AbsoluteEncoderI2C::write(Array<byte> data)
{
    Array<byte> sendBuffer = createData(data);

    Wire.beginTransmission(address);
    Wire.write(sendBuffer.buffer, sendBuffer.size);
    Wire.endTransmission();
    SEQ++;
}

void AbsoluteEncoderI2C::write(byte data) 
{
    write(Array<byte>(&data, 1));
}

template<typename T>
Array<T>::Array(int size) : size(size) {}

template<typename T> 
Array<T>::Array(T* data, int size) : size(size)
{
    memcpy(this->buffer, data, size);
}

template<typename T> 
T& Array<T>::operator[](int index)
{
    return buffer[index];
}

template<typename T>
Array<T> Array<T>::slice(byte offset, byte length)
{
    Array<T> sliced(length);
    memcpy(sliced.buffer, &buffer[offset], length);
    return sliced;
}