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
int speed = 20;

// displacement of the enemy joystick, read from the serial
int enemyDisp = 0;

// scores of each player
short redTotal, blueTotal;

// maximum velocity of the ball
int maxVelocity = 5;

long initialTime=0;
int numBalls=1;

//accumulator for the main loop
long acc=0;



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

Ball InitialBall = {
    5, // size
    WHITE, // colour
    0, 0, 0, 0}; // position and velocity

Ball ActiveBall[3];







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

    // initialize ball vector
    for (int i=0; i<3; ++i)
        ActiveBall[i] = InitialBall;
    
    
    // read horizontal and vertical rest position of the joystick
    restPosition = analogRead(HORZ);

    redTotal = blueTotal = 0;

    initialTime = millis();

    numBalls = 1;

    start(&RedPaddle,&BluePaddle, &ActiveBall[0]);
 
}

void loop()
{
    /* increase maximum ball velocity
     * by 5 pixels/s every 5 s*/
    acc = ((millis() - initialTime));
   if (acc % 5000 == 0)
       maxVelocity += 5;

    delay(speed);

    // result for each ball
    int result[3];

    for (int i=0; i<numBalls; ++i)
        result[i] = moveBall(&ActiveBall[i],&BluePaddle,&RedPaddle,maxVelocity);

    for (int i=0; i<numBalls; ++i)
        if (result[i])
        {
            finishRound(result[i], redTotal, blueTotal);
            start(&RedPaddle,&BluePaddle,&ActiveBall[0]);
            initialTime = millis();
            numBalls = 1;
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

    if (digitalRead(SEL) == LOW)
	quit();

}
