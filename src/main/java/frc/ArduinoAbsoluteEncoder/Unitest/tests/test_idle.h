#include "utility.h"



bool test_idle()
{
	print("Starte Test: ");
	println("test_idle");
	consoleTabIn();
	int cycles = 800;
	
	println("Waiting for orange led to shine up");
	for(int cycle=0; cycle<cycles; cycle++)
	{
		delay(10);
		if(cycle % (cycles/10) == 0 || cycles < 10)
		{
			print("=");
		}
	}
	println(" done");
	consoleTabOut();
	println("OK");
	return true;
}






