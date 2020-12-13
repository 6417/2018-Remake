


#include <Wire.h>
#include "AbsoluteEncoder8bit.h"
#include "Timer.h"

//#define DEBUG
#ifdef DEBUG
 #define SERIAL_BAUD 9600
#endif

#define USE_TIMED_UPDATE
#ifndef USE_TIMED_UPDATE
 #define USE_UPDATE_ON_REQUEST
#endif



//variables for pins
const uint16_t dipSwitchPins[4] {A0, A1, A2, A3};

//variables for code
uint8_t address             = 0;
#ifdef USE_TIMED_UPDATE
const unsigned long encoderUpdateInterval = 0; // us
#endif

//uint8_t ledState            = 0;
//uint8_t prevLedState        = 0;
//bool ledGreenState          = false;
//bool ledRedState            = false;
//uint16_t ledCounter1        = 0;
//uint16_t ledCounter2        = 0;
volatile byte registerRequest = 0x00;

// Led stuff
const byte led_red   = 9;
const byte led_green = 10;
const byte led_blue  = 11;

const int encoderRun_blinkInterval = 500; // ms
bool encoderRunBlinkToggle = true;
Timer encoderRunTimer;
// LedStuff end

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

void handleLed();

void setup()
{
  //set pinMode and read address
  for (int i = 0; i < 4; i++)
  {
    pinMode(dipSwitchPins[i], INPUT);
    address |= digitalRead(dipSwitchPins[i]) << i;
  }
  pinMode(led_red,OUTPUT);
  pinMode(led_green,OUTPUT);
  pinMode(led_blue,OUTPUT);
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
  handleLed();
}

void receiveEvent(int numBytes)//on receive event save requested register
{
  digitalWrite(led_green,1);
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
  digitalWrite(led_green,0);
}

void requestEvent()//on request event transmit requested data
{
  digitalWrite(led_green,1);
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
  digitalWrite(led_green,0);
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

void handleLed()
{
  /*const float pi = 3.1415;
  float angle = map(encoder.getPosRel()*10,0,2550,0,20*pi)/10;
  if(angle>pi)
    angle = 2*pi-angle;
  if(angle < pi/4)
  {
    analogWrite(led_blue,map(angle*10,0,20*pi,255,0));
  }*/
  
  if((encoder.getPosRel() < 5) || (encoder.getPosRel() > 250))
  {
    analogWrite(led_blue,100);
    analogWrite(led_green,0);
  }
  else
  {
    digitalWrite(led_blue,0);
    if(encoderRunTimer.start(encoderRun_blinkInterval))
    {

      analogWrite(led_green,50*encoderRunBlinkToggle);
      encoderRunBlinkToggle = !encoderRunBlinkToggle;
    }
  }
}
