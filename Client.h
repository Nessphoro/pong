//Code for the client part
#pragma once
#include "Utility.h"

//When it returns, client and server should both be at frame zero
void client()
{
	//Get Connect and get ACK
	while(true)
	{
		Serial.println("Connecting...");
		Serial3.write('C');
		Serial3.flush();

		bool cFlag=true;

		uint32_t startTime = millis();
		while(millis() - startTime < 1000)
		{
		 	if(Serial3.peek() == 'A')
		 	{
				Serial3.read(); // Header
				//And we're done
				cFlag = false;
				break;
		 	}
		 	else if(Serial3.peek()!= -1)
		 	{
		 		//Garbage
		 		Serial3.read();
		 		//Advance 
		 	}
		
		}

		//Clear the buffer
		while(Serial3.available())
		{
			Serial3.read();
		}

		if(!cFlag)
			break;
		else
		{
			Serial.println("Timeout.");
		}
	}
	Serial.println("Done negotiating.");
	while(Serial3.available())
	{
		Serial3.read();
	}
	delay(500);
		
	//Ok, we'have the server key now
	//ACK once again

	Serial3.write('A');
	delay(100);
	int32_t delayTime = 0, offset = 0;

	//Now, we must syncronize clocks
	for (int i = 0; i < 16; i++)
	{
		uint32_t start = micros();
		Serial3.write('S');
		Serial3.flush();
		while (Serial3.available() < 8)
		{
			
		}

		uint32_t end = micros();
		uint32_t serverStart = readLong3();
		uint32_t serverEnd = readLong3();

		delayTime += (end - start) - (serverEnd - serverStart);
		offset += ((serverStart - start) + (serverEnd - end)) / 2;
	}

	//Average
	delayTime /= 16;
	offset /= 16;
	//Ok client is now syncronized, now do the server
	for (int i = 0; i < 16; i++)
	{
		while (Serial3.read() != 'S')
		{

		}
		uint32_t serverStart = micros();
		uint32_t serverEnd = micros();
		sendLong3(serverStart);
		sendLong3(serverEnd);
		Serial3.flush();
	}

	Serial.print("Done syncronizing. Delay: ");
	Serial.print(delayTime);
	Serial.print(", Offset: ");
	Serial.println(offset);

	//Wait for random seed
	while (Serial3.available() < 5)
	{

	}

	Serial3.read();// should be 'R'
	uint32_t seed = readLong3();
	randomSeed(seed);

	while(Serial3.read() != 'G')
	{

	}

	//Sync game starts
	uint32_t timeToStart = 100 - (delayTime / 2) / 1000;
	delay(timeToStart);
}