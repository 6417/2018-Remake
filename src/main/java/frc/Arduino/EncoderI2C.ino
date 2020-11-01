#include <Wire.h>
#include <Encoder8bit.h>

//variables for pins
const uint16_t dipSwitchPins[4] {A3, A2, A1, A0};
const byte ledPinRed = 12;
const byte ledPinGreen = 13;

//variables for code
uint8_t adress = 0;
uint8_t ledState = 0;
uint8_t prevLedState = 0;
bool ledGreenState = false;
bool ledRedState = false;
uint16_t ledCounter1 = 0;
uint16_t ledCounter2 = 0;
byte registerRequest = 0x00;

//functions for code
void receiveEvent();
void requestEvent();
void returnPosition();
void manageLed();

Encoder8bit encoder(6, 7, 8, 9, 2, 3, 4, 5);

void setup()
{
  //set pinMode & read adress
  for(int i = 0; i < 4; i++)
  {
    pinMode(dipSwitchPins[i], INPUT);
    adress = adress | digitalRead(dipSwitchPins[i]) << i;
  }
  if(adress == 0)
  {
    ledState = 1;
  }
  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);

  //begin I2C communication
  Wire.begin(adress);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  //begin Serial communication
  Serial.begin(115200);
}

void loop()
{
  encoder.update();
  manageLed();
  
  delayMicroseconds(10);
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
    case 0x00: returnPosition(); break; //return absolute position of encoder, 1 byte
    case 0x01: encoder.update(); encoder.setHome(); break; //set relative homepoint of encoder
    case 0X02: prevLedState = ledState; ledState = 2; break; // blink led
  }
}

void returnPosition()
{
  Wire.write(encoder.getPosAbs());
}

void manageLed()
{
  switch(ledState)
  {
    case 0:
      ledGreenState = true;
      ledRedState = false;
      break;
    case 1:
      ledGreenState = false;
      ledRedState = true;
      break;
    case 2:
      ledState = 3;
      ledCounter1 = 0;
      ledCounter2 = 0;
      ledRedState = true;
    case 3:
      if(ledCounter1 > 1000)
      {
        ledCounter1 = 0;
        ledCounter2++;
        ledGreenState = !ledGreenState;
      }
      if(ledCounter2 > 10)
      {
        ledState = prevLedState;
      }
      ledCounter1++;
      break;
  }
  digitalWrite(ledPinGreen, ledGreenState);
  digitalWrite(ledPinRed, ledRedState);
}
