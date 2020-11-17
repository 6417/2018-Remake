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
byte registerRequest = 0x00;

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
  for(int i = 0; i < 4; i++)
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

  pinMode(12, OUTPUT);
}

void loop()
{
  encoder.update();  
  delayMicroseconds(10);

  if(millis() > lastMillis + 1000)
  {
    lastMillis = millis();
    ledState = !ledState;
    digitalWrite(12, ledState);
  }

  if(Serial.available())
  {
    Serial.println(Serial.read());
  }
  
}

void receiveEvent()//on receive event save requested register
{
  while(Wire.available())
  {
    registerRequest = Wire.read();
    Serial.print("requested register: ");
    Serial.println(registerRequest);
    
  }
}

void requestEvent()//on request event transmit requested data
{
  Serial.println("master requested response");
  switch(registerRequest)
  {
    case (int) RETURN_ABS_POSITION: returnAbsPosition(); break; //return absolute position of encoder, 1 byte
    case (int) SET_HOME: encoder.update(); encoder.setHome(Wire.read()); break; //set relative homepoint of encoder to sent position
    case (int) RETURN_REL_POSITION: returnRelPosition(); break; // return relative position to home
  }
}

void returnAbsPosition()
{
  Wire.write(::encoder.getPosAbs());
}

void returnRelPosition()
{
  Wire.write(::encoder.getPosRel());
}
