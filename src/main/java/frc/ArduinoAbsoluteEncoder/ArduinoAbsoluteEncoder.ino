


#include <Wire.h>
#include "AbsoluteEncoder8bit.h"

//#define DEBUG
#ifdef DEBUG
 #define SERIAL_BAUD 9600
#endif

//#define USE_TIMED_UPDATE
#ifndef USE_TIMED_UPDATE
 #define USE_UPDATE_ON_REQUEST
#endif



//variables for pins
const uint16_t dipSwitchPins[4] {A0, A1, A2, A3};

//variables for code
uint8_t address             = 0;
#ifdef USE_TIMED_UPDATE
const unsigned long encoderUpdateInterval = 100; // us
#endif

uint8_t ledState            = 0;
uint8_t prevLedState        = 0;
bool ledGreenState          = false;
bool ledRedState            = false;
uint16_t ledCounter1        = 0;
uint16_t ledCounter2        = 0;
volatile byte registerRequest = 0x00;

#ifdef USE_TIMED_UPDATE
unsigned long lastMicros = 0;
#endif

enum Rquest {
  RETURN_ABS_POSITION = 0x00,
  SET_HOME = 0x01,
  RETURN_REL_POSITION = 0x02
};

//functions for code
void receiveEvent(int numBytes);
void requestEvent();
void returnAbsPosition();

// PIND -> Inputregister for Arduino Nano Pins: 0, 1, 2, 3, 4, 5, 6, 7
// DDRD -> Pinmoderegister for the same pins
Encoder8bit encoder{&PIND,&DDRD};

void setup()
{
  //set pinMode and read address
  for (int i = 0; i < 4; i++)
  {
    pinMode(dipSwitchPins[i], INPUT);
    address |= digitalRead(dipSwitchPins[i]) << i;
  }

  //begin I2C communication
  Wire.begin(address);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

#ifdef DEBUG
  //begin Serial communication
  Serial.begin(SERIAL_BAUD);
  Serial.println("Encoder8bit Version: "+String(Encoder8bit_version));
  Serial.print("Address: ");
  Serial.println(address,BIN);
  Serial.println("Setup completed");
#endif


}

void loop()
{
  //volatile uint8_t *encoderPort = &PIND;

  
  //Serial.println(*encoderPort);
#ifdef USE_TIMED_UPDATE
  unsigned long _micros = micros();
  if(_micros - lastMicros >= encoderUpdateInterval)
  {
    lastMicros = _micros;
    encoder.update();
    #ifdef DEBUG
      Serial.print(encoder.getPosAbs());
      Serial.print(" ");
      Serial.println(encoder.getPosRel());
    #endif
  }
#endif
}

void receiveEvent(int numBytes)//on receive event save requested register
{
  while (Wire.available())
  {
    registerRequest = Wire.read();
    switch (registerRequest)
    {
      case SET_HOME: encoder.setHome(Wire.read()); break;
      default: break;
    }
  }
#ifdef DEBUG
  Serial.print("requested register: ");
  Serial.println(registerRequest);
#endif
}

void requestEvent()//on request event transmit requested data
{
#ifdef DEBUG
  Serial.println("master requested response");
#endif
#ifdef USE_UPDATE_ON_REQUEST
  encoder.update();
#endif
  switch (registerRequest)
  {
    case RETURN_ABS_POSITION: returnAbsPosition(); break; //return absolute position of encoder, 1 byte
    case SET_HOME: returnHomePosition(); break; //set relative homepoint of encoder to sent position
    case RETURN_REL_POSITION: returnRelPosition(); break; // return relative position to home
    default: break;
  }
}

void returnAbsPosition()
{
  Wire.write(encoder.getPosAbs());
}

void returnRelPosition()
{
  Wire.write(encoder.getPosRel());
}

void returnHomePosition()
{
  Wire.write(encoder.getHome());
}
