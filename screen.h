#ifndef SCREEN
#define SCREEN
#pragma once

#include <SPI.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library


#define SD_CS    5  // Chip select line for SD card
#define TFT_CS   6  // Chip select line for TFT display
#define TFT_DC   7  // Data/command line for TFT
#define TFT_RST  8  // Reset line for TFT (or connect to +5V)


//colour definitions

#define BLACK 0x0000
#define BLUE 0xF800
#define RED 0x001F
#define WHITE 0xFFFF

// Define pins for the joystick
#define VERT 0
#define HORZ 1
#define SEL 9
#define SERVER_SEL 13



//dimensions of the screen
const uint16_t srcWidth = 128; 
const uint16_t srcHeight =  160;

#endif
