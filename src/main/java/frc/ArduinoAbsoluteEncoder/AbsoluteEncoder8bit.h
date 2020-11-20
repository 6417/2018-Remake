/*
Autor:    Tim Koelbl
Datum:    16.06.2020
Version:  1.0.1

Encoder8bit is a arduino library for 8bit absolute encoders. It converts the gray code using
a conversion array. The library has the ability to set a homepoint. The relative positions to this
homepoint can be calculated as well.
NOTE: This feature dous not make it a relative encoder! The position value is still limited to 0-127
both absolute and relative positions can be return as position from 0-127, in degrees or in radians
*/

#ifndef Encoder8bit_h
#define Encoder8bit_h

#include "Arduino.h"

class Encoder8bit
{
  public:
  
    Encoder8bit(int p0, int p1, int p2, int p3, int p4, int p5, int p6, int p7);
  //constructor. saves pins of encoder into array. sets pinMode
  //pins are sorted from LSB to MSB
  
  void update();    //encoder calculates new absolute and relative position
  void setHome();   //set homepoint for relative position
  void setHome(uint8_t pos); //set homepoint to specified position
  uint8_t getHome(); // get homepoint
  int getPosAbs();  //get absolute position value. integer value from 0-127
  float getDegAbs();  //get absolute position value in degrees. float value from 0-360
  float getRadAbs();  //get absolute position value in radians. float value flom 0-2xPI
  int getPosRel();  //get relative position value. integer value from 0-127
  float getDegRel();  //get relative position value in degrees. float value from 0-360
  float getRadRel();  //get relative position value in radians. float value flom 0-2xPI
  
  
  private:
  
  int _pins[8]; //array for pins of encoder
  static const int _conversionTable[256]; //array for convesion from gray code to binary
  int _encoderPosAbs; //integer to save absolute encoder position
  int _encoderPosRel; //integer to save relative encoder position
  uint8_t _home;      //integer to save homepoint
};

#endif
