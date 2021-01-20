#ifndef PRINT_H
#define PRINT_H

#include "Arduino.h"

static byte consoleDepth = 0;
static bool printsEnabled = true;
static bool tabsEnable    = true;
void disablePrints();
void enablePrints();
void consoleTabIn();
void consoleTabOut();
void printTaps();

template<typename T>
void print(const T &msg);

template<typename T>
void println(const T &msg);

void disablePrints()
{
	printsEnabled = false;
}
void enablePrints()
{
	printsEnabled = true;
}
void consoleTabIn()
{
	if(consoleDepth < 255)
		consoleDepth++;
}
void consoleTabOut()
{
	if(consoleDepth > 0)
		consoleDepth--;
}
void printTaps()
{
	if(!printsEnabled || !tabsEnable)
		return;
	for(byte i=0; i<consoleDepth; i++)
	{
		Serial.print(" ");
	}
}

template<typename T>
void print(const T &msg)
{
	if(!printsEnabled)
		return;
	printTaps();
	Serial.print(msg);
	tabsEnable = false;
}

template<typename T>
void println(const T &msg)
{
	if(!printsEnabled)
		return;
	printTaps();
	Serial.println(msg);
	tabsEnable = true;
}
#endif