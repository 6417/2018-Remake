#include <Wire.h>
#include "AbsoluteEncoder8bit.h"

//variables for pins
const uint16_t dipSwitchPins[4] {A3, A2, A1, A0};

//variables for code
uint8_t adress = 0;
uint8_t ledState = 0;
uint8_t prevLedState = 0;
bool ledGreenState = false;
bool ledRedState = false;
uint16_t ledCounter1 = 0;
uint16_t ledCounter2 = 0;
volatile byte registerRequest = 0x00;

long lastMillis = 0;

enum Rquest {
  RETURN_ABS_POSITION = 0x00,
  SET_HOME = 0x01,
  RETURN_REL_POSITION = 0x02
};

//functions for code
void receiveEvent();
void requestEvent();
void returnAbsPosition();

Encoder8bit encoder{6, 7, 8, 9, 2, 3, 4, 5};

void setup()
{
  //set pinMode & read adress
  for (int i = 0; i < 4; i++)
  {
    pinMode(dipSwitchPins[i], INPUT);
    // adress = adress | digitalRead(dipSwitchPins[i]) << i;
  }
  adress = 5;

  //begin I2C communication
  Wire.begin(adress);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  //begin Serial communication
  Serial.begin(9600);
  Serial.println("setup completed");

  pinMode(12, OUTPUT);
}

void loop()
{
  encoder.update();
}

void receiveEvent()//on receive event save requested register
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
  Serial.print("requested register: ");
  Serial.println(registerRequest);
}

void requestEvent()//on request event transmit requested data
{
  Serial.println("master requested response");
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
