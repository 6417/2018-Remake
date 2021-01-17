#include "AbsoluteEncoderI2C.h"

AbsoluteEncoderI2C i2c = AbsoluteEncoderI2C();
Exception::ErrorCodes Exception::error = NO_ERROR;

Exception::ErrorHandler Exception::handlerList[] = {
    []() {},
    Exception::Handler::invalidCRC
};

byte AbsoluteEncoderI2C::calcCRC(Array<byte> data) noexcept
{
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

void AbsoluteEncoderI2C::write(byte data)
{
    write(Array<byte>(&data, 1));
}

void Exception::Handler::invalidCRC()
{
    Serial.println("Invalid CRC");
    Exception::error = Exception::ErrorCodes::INVALID_CRC; 
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
    data[2] = Wire.read();
    #ifdef DEBUG
    Serial.print("recived Register: ");
    Serial.println(data[2], HEX);
    #endif
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
    byte CRC = Wire.read();
    #ifdef DEBUG 
    Serial.print("CRC: ");
    Serial.print(CRC, HEX);
    Serial.print(", expected CRC: ");
    Serial.println(calcCRC(data), HEX);
    #endif
    if (calcCRC(data) != CRC) 
        Exception::handlerList[Exception::ErrorCodes::INVALID_CRC]();
    else 
        Exception::error = Exception::ErrorCodes::NO_ERROR;
    Array<byte> returnArr(data.size - 2);
    memcpy(returnArr.buffer, &data.buffer[2], data.buffer[1] + 1);
    return returnArr;
} 

void AbsoluteEncoderI2C::writeError(byte errorCode)
{
    Array<byte> sendBuffer(4);
    sendBuffer[0] = SEQ;
    sendBuffer[1] = 0;
    sendBuffer[2] = errorCode;
    sendBuffer.size--;
    sendBuffer[sendBuffer.size] = calcCRC(sendBuffer);
    sendBuffer.size++;
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

void AbsoluteEncoderI2C::write(Array<byte> data)
{
    Array<byte> sendBuffer(data.size + 4);
    sendBuffer[0] = SEQ;
    sendBuffer[1] = data.size;
    sendBuffer[2] = 0x00; // no error
    memcpy(&sendBuffer[3], data.buffer, data.size);
    sendBuffer.size--;
    sendBuffer[sendBuffer.size] = calcCRC(sendBuffer);
    sendBuffer.size++;
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
T Array<T>::operator[](int index) const
{
    return buffer[index];
}