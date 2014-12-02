#include "Utility.h"

//Send a long on Serial3
void sendLong3(uint32_t l)
{
	Serial3.write((byte) l);
	Serial3.write((byte) (l >> 8));
	Serial3.write((byte) (l >> 16));
	Serial3.write((byte) (l >> 24));
}

//Get a long on Serial3
uint32_t readLong3()
{
	uint32_t accumulator = 0;
	accumulator |= Serial3.read();
	accumulator |= Serial3.read() << 8;
	accumulator |= Serial3.read() << 16;
	accumulator |= Serial3.read() << 24;

	return accumulator;
}