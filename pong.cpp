#include <Arduino.h>

#include "paddle.h"
#include "screen.h"


// Define pins for the joystick
#define VERT 0
#define HORZ 1
#define SEL 9


// rest position of the paddle
int restPosition;

// size of delay in the main loop
// determines speed of the game
int speed = 25;



//read joystick signal
int joystickRead() 
{ 
    return (restPosition - analogRead(HORZ))/100; 
}



Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
// initialize paddles

Paddle PlayerPaddle = {
    paddleWidth, //size
    srcWidth/2 - paddleWidth/2, //position; place in middle of screen
    1, //player
    srcHeight-2-paddleHeight, //vertPosition;  2 pixels above the bottom
    BLUE};

Paddle EnemyPaddle = {
    paddleWidth, //size
    srcWidth/2 - paddleWidth/2, //position; place in middle of screen
    0, //not player
    2, // vertPosition; 2 pixels below the top
    RED};

void quit()
{
    tft.fillScreen(BLACK);
}


void setup()
{
    pinMode(VERT, INPUT);
    pinMode(HORZ, INPUT);
    pinMode(SEL, INPUT_PULLUP);
    digitalWrite(SEL, HIGH); //turn on internal resistor
    
    Serial.begin(9600);


    tft.initR(INITR_REDTAB);   // initialize a ST7735R chip, red tab
 
    tft.fillScreen(BLACK);

    drawPaddle(&PlayerPaddle);
    drawPaddle(&EnemyPaddle);


    // read horizontal and vertical rest position of the joystick
    restPosition = analogRead(HORZ);
}

void loop()
{
    delay(speed);

    movePaddle(&PlayerPaddle,joystickRead());
    Serial.println("joystickRead: ");
    Serial.println(joystickRead());
    Serial.println("position");
    Serial.println(PlayerPaddle.horzPosition);

    if (digitalRead(SEL) == LOW)
	quit();

}
