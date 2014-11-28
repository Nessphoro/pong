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
int speed = 16;

// displacement of the enemy joystick, read from the serial
int enemyDisp = 0;

// scores of each player
short redTotal, blueTotal;

// maximum velocity of the ball
int maxVelocity = 5;

long initialTime=0;




//read joystick signal
int joystickRead() 
{ 
    return (restPosition - analogRead(HORZ))/100; 
}



Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


// initialize paddles

Paddle BluePaddle = {
    paddleWidth, //size
    srcWidth/2 - paddleWidth/2, //position; place in middle of screen
    1, //player
    srcHeight-2-paddleHeight, //vertPosition;  2 pixels above the bottom
    BLUE,
    0}; // velocity

Paddle RedPaddle = {
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







bool isClient;

void setup()
{
    pinMode(SERVER_SEL,INPUT_PULLUP);
    pinMode(SEL, INPUT_PULLUP);
    digitalWrite(SEL, HIGH); //turn on internal resistor
    
    Serial.begin(9600);
    Serial3.begin(1000000);

    tft.initR(INITR_REDTAB);   // initialize a ST7735R chip, red tab
    tft.fillScreen(0);
    tft.setCursor(srcWidth/2 - 30, srcHeight/2);
    tft.print("Connecting");
    isClient = 0;
    isClient=digitalRead(SERVER_SEL)==LOW;
    tft.setCursor(srcWidth/2 - 35,srcHeight/2 + 20);
    if(isClient)
    {
        tft.setTextColor(BLUE);
        tft.print("You are blue");
    }
    else
    {
        tft.setTextColor(RED);
        tft.print("You are red");
    }


    if(!isClient)
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

    countdown();
    
    
    // read horizontal and vertical rest position of the joystick
    restPosition = analogRead(HORZ);

    redTotal = blueTotal = 0;

    initialTime = millis();



    start(&RedPaddle,&BluePaddle, &ActiveBall);
 
}

void loop()
{
    /* increase maximum ball velocity
     * by 5 pixels/s every 5 s*/
    long acc = ((millis() - initialTime));
    if (acc % 5000 == 0)
       maxVelocity += 5;


    // result for each ball
    int result = moveBall(&ActiveBall,&BluePaddle,&RedPaddle,maxVelocity);

    if (result)
    {
        finishRound(result, redTotal, blueTotal);
        start(&RedPaddle,&BluePaddle,&ActiveBall);
        initialTime = millis();
        
    }

    //exchange joystick data
   
    int jostickInput = joystickRead();
    sendLong3(jostickInput);
    while(Serial3.available() < 4);
    enemyDisp = readLong3();


    if (isClient)
    {
    	movePaddle(&BluePaddle,jostickInput);
    	movePaddle(&RedPaddle,enemyDisp);
    }
    else
    {
    	movePaddle(&RedPaddle, jostickInput);
    	movePaddle(&BluePaddle,enemyDisp);
    }


    drawPaddle(&RedPaddle);
    drawPaddle(&BluePaddle);



    delay(speed);
}
