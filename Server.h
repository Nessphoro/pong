//Code for the server part
#pragma once
#include "Utility.h"

//When it returns, client and server should both be at frame zero
void server()
{
	uint32_t resetTime=0;

	bool connectionExpected = false;
	//Wait for connection
	while(true)
	{
		if(!connectionExpected)
		{
			//is there a connection on the wire?
			if(Serial3.peek() == 'C')
			{	
				Serial.read();
				Serial.println("Initiating connection");
				Serial3.write('A');

				Serial3.flush();

				//Now we are expecting an ACK, too
				connectionExpected=true;

				//And make sure we'll reset
				resetTime=millis();
				
			}
			else if(Serial3.peek() != -1)
			{
				//No, but there is garbage on the line
				Serial3.read(); //Purge the cache
			}
		}
		else
		{
			if(Serial3.peek() == 'A')
			{
				Serial.println("Found last ACK");
				//And here is the confirmation
				Serial3.read();
				break; //And break
			}
			else if(Serial3.peek() != -1)
			{

				Serial3.read();

			}

			if(millis() - resetTime > 1000)
			{
				connectionExpected=false; //We were abandonned

				//Purge everything
				while(Serial3.available())
					Serial3.read();
			}
		}
	}

	Serial.println("Syncronizing");
	int32_t delayTime = 0, offset = 0;

	//Syncronize the client
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

	//Sync ourselves
	for (int i = 0; i < 16; i++)
	{
		uint32_t start = micros();
		Serial3.write('S');
		Serial3.flush();
		while (Serial3.available() < 8)
		{

		}

		uint32_t end = micros();
		uing32_t serverStart = readLong3();
		uint32_t serverEnd = readLong3();

		delayTime += (end - start) - (serverEnd - serverStart);
		offset += ((serverStart - start) + (serverEnd - end)) / 2;
	}

	//Average
	delay /= 16;
	offset /= 16;

	Serial.print("Done syncronizing. Delay: ");
	Serial.print(delayTime);
	Serial.print(", Offset: ");
	Serial.println(offset);

	//Send "start game", game will start in 100 000 micro seconds
	Serial3.write('G');
	delay(100);
}
