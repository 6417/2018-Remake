//#define DEBUG
#ifdef DEBUG
 #define SERIAL_BAUD 9600
#endif

#define USE_TIMED_UPDATE
#ifndef USE_TIMED_UPDATE
 #define USE_UPDATE_ON_REQUEST
#endif


#include <Wire.h>
#include "AbsoluteEncoder8bit.h"
#include "Timer.h"
#include "error.h"





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

Timer encoderIdleTimer;
volatile bool encoderIsIdle = false;
Timer idleLedDimTimer;
float encoderIdleBrightness = 0;

Timer i2cEventCooldownTimer;
volatile bool i2cEventRecieved = false;
unsigned int i2cEventRecievedOnTime = 100;
void on_i2cEventCooldownTimer();


unsigned int errorRepeatAmount = 3; 
unsigned int errorRepeatCounter = 0;
//volatile byte errorCode = 0;
Timer errorBlinkTimer;
unsigned int errorBlinkInterval = 300;
bool errorBlinkState = 0;
byte errorCode_currentBlinkStep = 0;
// LedStuff end

#ifdef USE_TIMED_UPDATE
unsigned long lastMicros = 0;
#endif

enum Rquest {
  RETURN_ABS_POSITION = 0x00,
  SET_HOME            = 0x01,
  RETURN_REL_POSITION = 0x02,
  RETURN_CURRENT_ERROR        = 0x10,
  RETURN_LAST_ERROR           = 0x11,
  CLEAR_ERROR                 = 0x12
};

//functions for code
void receiveEvent(int numBytes);
void requestEvent();
void returnAbsPosition();
void returnCurrentError();
void returnLastError();

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
  pinMode(13,OUTPUT);
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

  i2cEventCooldownTimer.onFinished(on_i2cEventCooldownTimer);

  
}

void loop()
{
  //volatile uint8_t *encoderPort = &PIND;
  i2cEventCooldownTimer.update();


  if(encoderIdleTimer.start(5000))
  {
    encoderIsIdle = true;
    digitalWrite(13,!digitalRead(13));
  }
  
  //Serial.println(*encoderPort);
#ifdef USE_TIMED_UPDATE
  unsigned long _micros = micros();
  if(_micros - lastMicros >= encoderUpdateInterval)
  {
    lastMicros = _micros;
    encoder.update();
    #ifdef DEBUG
 /*     Serial.print(encoder.getPosAbs());
      Serial.print(" ");
      Serial.println(encoder.getPosRel());*/
    #endif
  }
#endif
  handleLed();
}

void receiveEvent(int numBytes)//on receive event save requested register
{
  encoderIsIdle    = false;
  encoderIdleTimer.stop();
  
  i2cEventRecieved = true;
  i2cEventCooldownTimer.start(i2cEventRecievedOnTime);
  
  if (Wire.available())
  {
    registerRequest = Wire.read();
    switch (registerRequest)
    {
      case SET_HOME:
        if(numBytes == 2) 
          encoder.setHome(Wire.read()); 
        break;
      case CLEAR_ERROR:
          ERROR::currentError = 0;
          ERROR::lastError    = 0;
          ERROR::errorOccured = false;
        break;
      case RETURN_ABS_POSITION: 
      case RETURN_REL_POSITION: 
      case RETURN_CURRENT_ERROR: 
      case RETURN_LAST_ERROR: break;
      default:
      {
          ERROR::throw__i2c_badRegisterAccess();
          
        break;
      }
    }
  }
#ifdef DEBUG
  Serial.print("requested register: ");
  Serial.println(registerRequest);
#endif
  if(Wire.available())
  {
    ERROR::throw__i2c_toMuchDataRecieved();
    while(Wire.available())
    {
      Wire.read();
    }
  }
  
}

void requestEvent()//on request event transmit requested data
{
  encoderIsIdle    = false;
  encoderIdleTimer.stop();
  i2cEventRecieved = true;
  i2cEventCooldownTimer.start(i2cEventRecievedOnTime);
#ifdef DEBUG
  Serial.print("master requested response on Register: ");
  Serial.println(registerRequest,HEX);
#endif
#ifdef USE_UPDATE_ON_REQUEST
  encoder.update();
#endif
  switch (registerRequest)
  {
    case RETURN_ABS_POSITION: returnAbsPosition(); break; //return absolute position of encoder, 1 byte
    case SET_HOME: returnHomePosition(); break; //set relative homepoint of encoder to sent position
    case RETURN_REL_POSITION: returnRelPosition(); break; // return relative position to home
    case RETURN_CURRENT_ERROR: returnCurrentError(); break;
    case RETURN_LAST_ERROR: returnLastError(); break;
    default: 
      ERROR::throw__i2c_badRegisterRequest();
    break;
  }
}

void returnAbsPosition()
{
 #ifdef DEBUG
  Serial.print("returnAbsPosition:  ");
  Serial.println(encoder.getPosAbs());
#endif
  Wire.write(encoder.getPosAbs());
}

void returnRelPosition()
{
   #ifdef DEBUG
  Serial.print("returnRelPosition:  ");
  Serial.println(encoder.getPosRel());
#endif
  Wire.write(encoder.getPosRel());
}

void returnHomePosition()
{
#ifdef DEBUG
  Serial.print("returnHomePosition:  ");
  Serial.println(encoder.getHome());
#endif
  Wire.write(encoder.getHome());
}
void returnCurrentError()
{
  #ifdef DEBUG
  Serial.print("returnCurrentError:  ");
  Serial.println(ERROR::currentError);
#endif
  Wire.write(ERROR::currentError);
}
void returnLastError()
{
#ifdef DEBUG
  Serial.print("returnLastError:  ");
  Serial.println(ERROR::lastError);
#endif
  Wire.write(ERROR::lastError);
}

void handleLed()
{
  byte red   = 0; 
  byte green = 0;
  byte blue  = 0;
  /*const float pi = 3.1415;
  float angle = map(encoder.getPosRel()*10,0,2550,0,20*pi)/10;
  if(angle>pi)
    angle = 2*pi-angle;
  if(angle < pi/4)
  {
    analogWrite(led_blue,map(angle*10,0,20*pi,255,0));
  }*/
  
  if((encoder.getPosRel() < 5) || (encoder.getPosRel() > 123))
  {
    blue = 100;
    //analogWrite(led_green,0);
  }

  if(encoderIsIdle)
  {
    if(idleLedDimTimer.start(10))
    {
      encoderIdleBrightness+=0.01;
      if(encoderIdleBrightness >= 3.141592)
        encoderIdleBrightness = 0;
    }
    red   = 128 * sin(encoderIdleBrightness);
    green = 128 * sin(encoderIdleBrightness);
  }

  if(i2cEventRecieved)
  {
    green = 128;
  }

  if(ERROR::errorOccured)
  {
    green = 0;
    blue  = 0;
    if(errorCode_currentBlinkStep % 2 == 0 && errorCode_currentBlinkStep < 2*ERROR::currentError)
    {
      red = 255;
    }
    else
    {
      red = 0;
    }
    if(errorBlinkTimer.start(errorBlinkInterval))
    {
      errorCode_currentBlinkStep++;
      if(errorCode_currentBlinkStep > 2*ERROR::currentError+1)
      {
        errorCode_currentBlinkStep = 0;
        errorRepeatCounter++;
        if(errorRepeatCounter >= errorRepeatAmount)
        {
          ERROR::errorOccured = false;
          ERROR::clear();
          errorRepeatCounter = 0;
          encoderIsIdle    = false;
        }
      }
    }
  }

  analogWrite(led_red,red);
  analogWrite(led_green,green);
  analogWrite(led_blue,blue);
}
void on_i2cEventCooldownTimer()
{
  i2cEventRecieved = false;
}
