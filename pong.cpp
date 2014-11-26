#include <Arduino.h>

#include "paddle.h"
#include "screen.h"
#include "ball.h"
#include "Client.h"
#include "Server.h"
#include "game.h"


// rest position of the paddle
int restPosition;

// size of delay in the main loop
// determines speed of the game

// the game is currently slowed down for testing purposes
int speed = 500;

// displacement of the enemy joystick, read from the serial
int enemyDisp = 0;

// scores of each player
short redTotal, blueTotal;



//read joystick signal
int joystickRead() 
{ 
    return (restPosition - analogRead(HORZ))/100; 
}



Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


// initialize paddles

Paddle RedPaddle = {
    paddleWidth, //size
    srcWidth/2 - paddleWidth/2, //position; place in middle of screen
    1, //player
    srcHeight-2-paddleHeight, //vertPosition;  2 pixels above the bottom
    BLUE,
    0}; // velocity

Paddle BluePaddle = {
    paddleWidth, //size
    srcWidth/2 - paddleWidth/2, //position; place in middle of screen
    0, //not player
    2, // vertPosition; 2 pixels below the top
    RED,
    0}; // velocity

// create Ball object

Ball ActiveBall = {
    5, // size
    WHITE, // colour
    0, 0, 0, 0}; // position and velocity





bool client;

void setup()
{
    pinMode(SERVER_SEL,INPUT_PULLUP);
    pinMode(SEL, INPUT_PULLUP);
    digitalWrite(SEL, HIGH); //turn on internal resistor
    
    Serial.begin(9600);
    Serial3.begin(1000000);

    if(digitalRead(SERVER_SEL)==HIGH)
    {
        Serial.print("Server mode");
	   server();
    }
    else
    {
        Serial.print("Client mode");
	   client();
    }
    Serial.print("Hello world");
    //randomSeed(analogRead(2)); //seeding the random function with an unused pin

    tft.initR(INITR_REDTAB);   // initialize a ST7735R chip, red tab
    
    // read horizontal and vertical rest position of the joystick
    restPosition = analogRead(HORZ);
    client = digitalRead(SERVER_SEL) == LOW;

    redTotal = blueTotal = 0;

    start(&RedPaddle,&BluePaddle);
 
}

void loop()
{
    delay(speed);

    int result = moveBall(&ActiveBall,&BluePaddle,&RedPaddle);

    if (result)
    {
	finishRound(result, &redTotal, &blueTotal);
	start(&RedPaddle,&BluePaddle,&ActiveBall);
    }

    //exchange joystick data
    enemyDisp = readLong3();
    sendLong3(joystickRead());


    if (client)
    {
	movePaddle(&BluePaddle,joystickRead());
	movePaddle(&RedPaddle,enemyDisp);
    }
    else
    {
	movePaddle(&RedPaddle,joystickRead());
	movePaddle(&BluePaddle,enemyDisp);
    }


    drawPaddle(&RedPaddle);
    drawPaddle(&BluePaddle);

    if (digitalRead(SEL) == LOW)
	quit();

}
