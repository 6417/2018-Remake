/*
 AbsolutEncoder Test Script
 Autor:   Alex Krieg
 Version: 0.0.0
 Date:    13.12.2020

  Setup:
   Connect the I2C interface of the Test Arduino (which is running this programm) with the Encoder Print.
   For some tests you have to look at he LED and check if it behaves the right way.
 */


#include <Wire.h>

const byte address = 1;
enum Rquest {
  RETURN_ABS_POSITION = 0x00,
  SET_HOME            = 0x01,
  RETURN_REL_POSITION = 0x02,
  RETURN_CURRENT_ERROR        = 0x10,
  RETURN_LAST_ERROR           = 0x11,
  CLEAR_ERROR                 = 0x12
};

bool testResault = false;
unsigned int testPassCounter = 0;
unsigned int testFailCounter = 0;

bool test_readRelativeEncoderValue();
bool test_readRelativeEncoderValue_muchData();
bool test_idle();
bool test_wrongRequest();

bool checkForError();

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  //Wire.setClock(50);
  Serial.begin(9600);  // start serial for output


  delay(10);
    Wire.beginTransmission(address);
    Wire.write(CLEAR_ERROR);
   // Wire.write(0x01);
    Wire.endTransmission();
  delay(10);
}

void loop() {
  /*for(int i=0; i<10; i++)
  {
    Wire.beginTransmission(address);
    Wire.write(0x02);
    //Wire.write(0x01);
   // Wire.write(0x01);
    Wire.endTransmission();
    Wire.requestFrom(address, 1);      // request 1 bytes from slave device
  
    while (Wire.available()) { // slave may send less than requested
      char c = Wire.read(); // receive a byte as character
      Serial.println(c,DEC);         // print the character
    }
  
    delay(500);
  }*/

  testResault = true;
  testResault &= test_readRelativeEncoderValue();
  testResault &= test_idle();
  testResault &= test_readRelativeEncoderValue_muchData();

  if(testResault)
  {
    testPassCounter++;
  }
  else
  {
    testFailCounter++;
  }
  Serial.println("Testresultat: ");
  Serial.print("Aktueller Test bestanden: ");
  Serial.println(testResault);
  Serial.print("Passes: ");
  Serial.println(testPassCounter);
  Serial.print("Fails: ");
  Serial.println(testFailCounter);

  //delay(10000);
}

bool test_readRelativeEncoderValue()
{
  Serial.print("Starte Test: ");
  Serial.println("test_readRelativeEncoderValue");
  int successCounter = 0;
  bool fail = false;
  for(int i=0; i<10; i++)
  {
    Wire.beginTransmission(address);
    Wire.write(RETURN_REL_POSITION);
    //Wire.write(0x01);
   // Wire.write(0x01);
    Wire.endTransmission();
    Wire.requestFrom(address, 1);      // request 1 bytes from slave device
  
    while (Wire.available()) { // slave may send less than requested
      byte c = Wire.read(); // receive a byte as character
      //Serial.println(c,DEC);         // print the character
      successCounter++;
      Serial.print("Encoder relative Pos: ");
      Serial.println(c);
    }

    fail |= checkForError();

    delay(500);
  }
  if(successCounter == 10 && !fail)
  {
    Serial.println("Test OK");
    return true;
  }
  else
  {
    Serial.println("Test FAIL");
    return false;
  }
}

bool test_readRelativeEncoderValue_muchData()
{
  Serial.print("Starte Test: ");
  Serial.println("test_readRelativeEncoderValue_muchData");
  int successCounter = 0;
  bool fail = false;
  for(int i=0; i<5; i++)
  {
    Wire.beginTransmission(address);
    Wire.write(RETURN_REL_POSITION);
    //Wire.write(0x01);
   // Wire.write(0x01);
    Wire.endTransmission();
    Wire.requestFrom(address, 1);      // request 1 bytes from slave device
  
    while (Wire.available()) { // slave may send less than requested
      byte c = Wire.read(); // receive a byte as character
      //Serial.println(c,DEC);         // print the character
      successCounter++;
      Serial.print("Encoder relative Pos: ");
      Serial.println(c);
    }
    fail |= checkForError();
    
    delay(500);
  }
  for(int i=0; i<1; i++)
  {
    Wire.beginTransmission(address);
    Wire.write(RETURN_REL_POSITION);
    Wire.write(1);
   // Wire.write(0x01);
    Wire.endTransmission();
    Wire.requestFrom(address, 1);      // request 1 bytes from slave device
  
    while (Wire.available()) { // slave may send less than requested
      byte c = Wire.read(); // receive a byte as character
      //Serial.println(c,DEC);         // print the character
      successCounter++;
      Serial.print("Encoder relative Pos: ");
      Serial.println(c);
    }
    fail |= !checkForError();
    delay(500);
  }
  if(successCounter == 6 && !fail)
  {
    Serial.println("Test OK");
    return true;
  }
  else
  {
    Serial.println("Test FAIL");
    return false;
  }
}

bool test_idle()
{
  Serial.print("Starte Test: ");
  Serial.println("test_idle");

  Serial.println("Waiting for orange led to shine up");
  delay(8000);
  return true;
}

bool test_wrongRequest()
{
  Serial.print("Starte Test: ");
  Serial.println("test_readRelativeEncoderValue_muchData");
  
}

bool checkForError()
{
  bool lastErrorOccured = true;
  bool errorOccured = true;
  byte c;
  delay(10);
    Wire.beginTransmission(address);
    Wire.write(RETURN_CURRENT_ERROR);
   // Wire.write(0x01);
    Wire.endTransmission();
    Wire.requestFrom(address, 1);      // request 1 bytes from slave device
  
    while (Wire.available()) { // slave may send less than requested
      c = Wire.read(); // receive a byte as character
      if(c == 0)
        errorOccured = false;
    }
  delay(10);
    Wire.beginTransmission(address);
    Wire.write(RETURN_LAST_ERROR);
   // Wire.write(0x01);
    Wire.endTransmission();
    Wire.requestFrom(address, 1);      // request 1 bytes from slave device
  
    while (Wire.available()) { // slave may send less than requested
      c = Wire.read(); // receive a byte as character
      if(c == 0)
        lastErrorOccured = false;
    }
  delay(10);
    
  if(lastErrorOccured || errorOccured)
  {
    Serial.println("Error occured");
    Serial.println("Waiting for red blink");
    delay(8000);
    Wire.beginTransmission(address);
    Wire.write(CLEAR_ERROR);
    // Wire.write(0x01);
    Wire.endTransmission();
    delay(10);
    return true;
  }
  return false;
}
