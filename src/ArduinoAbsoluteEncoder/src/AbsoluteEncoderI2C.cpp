#include "AbsoluteEncoderI2C.h"

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

void AbsoluteEncoderI2C::writeError(byte data)
{

}

Array<byte> AbsoluteEncoderI2C::read() 
{
    SEQ = Wire.read();

    #ifdef DEBUG
        Serial.print("SEQ: ");
        Serial.println(SEQ, HEX);
    #endif

    byte length = Wire.read();

    #ifdef DEBUG
        Serial.print("length: ");
        Serial.println(length, HEX);
    #endif

    Array<byte> data(length + 3);
    data[0] = SEQ;
    data[1] = length;
    data[2] = Wire.read(); // reads register

    #ifdef DEBUG
        Serial.print("recived Register: ");
        Serial.println(data[2], HEX);
    #endif

    readData(data, length);
    byte CRC = Wire.read();
    checkCRC(CRC, calcCRC(data));

    return data.slice(2, data[1] + 1);
} 

void AbsoluteEncoderI2C::checkCRC(byte recCRC, byte CRC)
{
    #ifdef DEBUG 
        Serial.print("CRC: ");
        Serial.print(recCRC, HEX);
        Serial.print(", expected CRC: ");
        Serial.println(CRC, HEX);
    #endif

    if (recCRC != CRC) 
        ERROR::throw__i2c_invalidCRC();
}

void AbsoluteEncoderI2C::readData(Array<byte>& data, byte length)
{
    for (int i = 3; i < length + 3; i++)
    {
        data[i] = Wire.read();
        #ifdef DEBUG
            Serial.print("data index: ");
            Serial.print(i - 3);
            Serial.print(": ");
            Serial.println(data[i], HEX);
        #endif
    }
}

Array<byte> AbsoluteEncoderI2C::createData(Array<byte> data, byte error)
{
    Array<byte> sendBuffer(data.size + 4);
    sendBuffer[0] = SEQ;
    sendBuffer[1] = data.size;
    sendBuffer[2] = error;
    sendBuffer[sendBuffer.size - 1] = calcCRC(sendBuffer.slice(0, sendBuffer.size - 1));
}

void AbsoluteEncoderI2C::write(Array<byte> data)
{
    Array<byte> sendBuffer(data.size + 4);
    sendBuffer[0] = SEQ;
    sendBuffer[1] = data.size;
    sendBuffer[2] = 0x00; // no error
    memcpy(&sendBuffer[3], data.buffer, data.size); // copy buffer of data into the data section of sendBuffer
    sendBuffer[sendBuffer.size - 1] = calcCRC(sendBuffer.slice(0, sendBuffer.size - 1));

    #ifdef DEBUG
        Serial.print("Sending: ");
        for (int i = 0; i < sendBuffer.size -1; i++){
            Serial.print(sendBuffer[i], HEX);
            Serial.print(", ");
        }
        Serial.println(sendBuffer.size -1);
    #endif
    
    Wire.write(sendBuffer.buffer, sendBuffer.size);
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