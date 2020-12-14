/*
 AbsolutEncoder Test Script
 Autor:   Alex Krieg
 Version: 0.1.0
 Date:    14.12.2020

  Setup:
   Connect the I2C interface of the Test Arduino (which is running this programm) with the Encoder Print.
   For some tests you have to look at he LED and check if it behaves the right way.
 */


#include <Wire.h>
#include "print.h"

#include "tests/tests.h"

#define I2C_HIGH_SPEED_MODE



bool testResult = false;
unsigned int testPassCounter = 0;
unsigned int testFailCounter = 0;


//byte getError();


void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  #ifdef I2C_HIGH_SPEED_MODE
    // Fastest stable clock frequency I can get
    // physical max clk freq: 333kHz
    Wire.setClock(571428);
  #endif
  Serial.begin(115200);  // start serial for output

  
  delay(10);
    Wire.beginTransmission(address);
    Wire.write(CLEAR_ERROR);
    Wire.endTransmission();
  delay(10);
}

void loop() {
  const int tests = 8;
  bool resultList[tests];
  bool testsExecuted[tests];
  for(int i=0; i<tests; i++)
  {
    resultList[i] = true;
    testsExecuted[i] = false;
  }
  testResult = true;
 
  resultList[0] = test_readRelativeEncoderValue(); testsExecuted[0] = true;
  resultList[1] = test_idle(); testsExecuted[1] = true;
  resultList[2] = test_readRelativeEncoderValue_muchData(); testsExecuted[2] = true;
  resultList[3] = test_wrongRequest(); testsExecuted[3] = true;
  resultList[4] = test_spamRequest(); testsExecuted[4] = true;
  resultList[5] = test_readAllRegisters(); testsExecuted[5] = true;
  resultList[6] = test_setHomePos();  testsExecuted[6] = true;
  resultList[7] = test_checkRelativePos(); testsExecuted[7] = true;

 

  for(int i=0; i<tests; i++)
  {
    if(!resultList[i])
      testResult = false;
  }

  if(testResult)
  {
    testPassCounter++;
  }
  else
  {
    testFailCounter++;
  }
  println("-----------------------------------");
  consoleTabIn();
  println("Testresultat: ");
  
  for(int i=0; i<tests; i++)
  {
    if(testsExecuted[i])
    {
      print("Test: ");
      print(i);
      print("\t");
      println((resultList[i]?"PASS":"FAIL"));
    }
  }
  
  print("Aktueller Testdurchlauf bestanden: ");
  println((testResult?"PASS":"FAIL"));
  println("");
  print("Passes: ");
  println(testPassCounter);
  print("Fails: ");
  println(testFailCounter);
  consoleTabOut();
  println("-----------------------------------");
}
