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
    Serial.print("SEQ: ");
    Serial.println(SEQ, HEX);
    byte length = Wire.read();
    Serial.print("length: ");
    Serial.println(length, HEX);
    Array<byte> data(length + 2);
    data[0] = SEQ;
    data[1] = length;
    for (int i = 2; i < length + 2; i++)
    {
        data[i] = Wire.read();
        Serial.print("data index: ");
        Serial.print(i - 2);
        Serial.print(": ");
        Serial.println(data[i], HEX);
    }
    byte CRC = Wire.read();
    Serial.print("CRC: ");
    Serial.print(CRC, HEX);
    Serial.print(", expected CRC: ");
    Serial.println(calcCRC(data), HEX);
    if (calcCRC(data) != CRC) 
        Exception::handlerList[Exception::ErrorCodes::INVALID_CRC]();
    else 
        Exception::error = Exception::ErrorCodes::NO_ERROR;
    // (byte*) data.buffer += 2;
    // data.size -= 2;
    Array<byte> returnArr(data.size - 2);
    memcpy(returnArr.buffer, &data.buffer[2], data.buffer[1]);
    return returnArr;
} 

void AbsoluteEncoderI2C::write(Array<byte> data)
{
    Array<byte> sendBuffer(data.size + 3);
    sendBuffer[0] = SEQ;
    sendBuffer[1] = data.size;
    memcpy(&sendBuffer[2], data.buffer, data.size);
    sendBuffer.size--;
    sendBuffer[sendBuffer.size] = calcCRC(sendBuffer);
    sendBuffer.size++;
    Serial.print("Sending: ");
    for (int i = 0; i < sendBuffer.size -1; i++){
        Serial.print(sendBuffer[i], HEX);
        Serial.print(", ");
    }
    Serial.println(sendBuffer.size -1);
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

template<typename T>
T Array<T>::get(int index)
{
    return buffer[index];
}
