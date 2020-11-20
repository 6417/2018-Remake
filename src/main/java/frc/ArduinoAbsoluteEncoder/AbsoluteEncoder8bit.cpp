/*
Autor:    Tim Koelbl
Datum:    16.06.2020
Version:  1.0.1
*/

#include "Arduino.h"
#include "AbsoluteEncoder8bit.h"

const int Encoder8bit::_conversionTable[256] = 
  {
    256, 56, 40, 55, 24, 256, 39, 52, 8, 57, //0
    256, 256, 23, 256, 36, 13, 120, 256, 41, 54, //1
    256, 256, 256, 53, 7, 256, 256, 256, 20, 19, //2
    125, 18, 104, 105, 256, 256, 25, 106, 38, 256, //3
    256, 58, 256, 256, 256, 256, 37, 14, 119, 118, //4
    256, 256, 256, 107, 256, 256, 4, 256, 3, 256, //5
    109, 108, 2, 1, 88, 256, 89, 256, 256, 256, //6
    256, 51, 9, 10, 90, 256, 22, 11, 256, 12, //7
    256, 256, 42, 43, 256, 256, 256, 256, 256, 256, //8
    256, 256, 21, 256, 126, 127, 103, 256, 102, 256, //9
    256, 256, 256, 256, 256, 256, 91, 256, 256, 256, //10
    256, 256, 116, 117, 256, 256, 115, 256, 256, 256, //11
    93, 94, 92, 256, 114, 95, 113, 0, 72, 71, //12
    256, 68, 73, 256, 256, 29, 256, 70, 256, 69, //13
    256, 256, 35, 34, 121, 256, 122, 256, 74, 256, //14
    256, 30, 6, 256, 123, 256, 256, 256, 124, 17, //15
    256, 256, 256, 67, 26, 256, 27, 28, 256, 59, //16
    256, 256, 256, 256, 256, 15, 256, 256, 256, 256, //17
    256, 256, 256, 256, 5, 256, 256, 256, 110, 256, //18
    111, 16, 87, 84, 256, 45, 86, 85, 256, 50, //19
    256, 256, 256, 46, 256, 256, 256, 33, 256, 83, //20
    256, 44, 75, 256, 256, 31, 256, 256, 256, 256, //21
    256, 256, 256, 32, 100, 61, 101, 66, 256, 62, //22
    256, 49, 99, 60, 256, 47, 256, 256, 256, 48, //23
    77, 82, 78, 65, 76, 63, 256, 64, 98, 81, //24
    79, 80, 97, 96, 112, 256 //25
  };

Encoder8bit::Encoder8bit(int p0, int p1, int p2, int p3, int p4, int p5, int p6, int p7)
{
  _pins[0] = p0;
  _pins[1] = p1;
  _pins[2] = p2;
  _pins[3] = p3;
  _pins[4] = p4;
  _pins[5] = p5;
  _pins[6] = p6;
  _pins[7] = p7;
  
  for(int i = 0; i < 8; i++)
  {
    pinMode(_pins[i], INPUT);
  }
  
  _encoderPosAbs = 0;
  _encoderPosRel = 0;
  _home = 0;
}

void Encoder8bit::update()
{
  noInterrupts();
  _encoderPosAbs = 0;
    for(int i = 0; i < 8; i++)
    {
    _encoderPosAbs |= digitalRead(_pins[i]) << i;
    }
    _encoderPosAbs = _conversionTable[_encoderPosAbs];
  _encoderPosRel = _encoderPosAbs - _home;
  if(_encoderPosRel < 0)
  {
    _encoderPosRel += 128;
  }
  interrupts();
}

int Encoder8bit::getPosAbs()
{
  return _encoderPosAbs;
}

float Encoder8bit::getDegAbs()
{
  return (float)_encoderPosAbs * 2.8125;
}

float Encoder8bit::getRadAbs()
{
  return (float)_encoderPosAbs * 2.8125 * DEG_TO_RAD;
}

int Encoder8bit::getPosRel()
{
  return _encoderPosRel;
}

float Encoder8bit::getDegRel()
{
  return (float)_encoderPosRel * 2.8125;
}

float Encoder8bit::getRadRel()
{
  return (float)_encoderPosRel * 2.8125 * DEG_TO_RAD;
}

void Encoder8bit::setHome()
{
  _home = _encoderPosAbs;
}

void Encoder8bit::setHome(uint8_t pos) 
{
  _home = pos;
}

uint8_t Encoder8bit::getHome()
{
  return _home;
}
