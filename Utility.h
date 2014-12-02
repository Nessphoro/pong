//Contains code that is generally useful across the code base
#pragma once
#include <Arduino.h>

//Send a long on Serial3
void sendLong3(uint32_t l);

//Get a long on Serial3
uint32_t readLong3();