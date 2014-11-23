#include <Arduino.h>
#include "ball.h"


const int maxVelocity = 20;


extern Adafruit_ST7735 tft;


void drawBall(Ball * ball)
{
    tft.fillRect(ball->horzPosition, ball->vertPosition,
	    ball->size, ball->size, 
	    ball->colour);
}

void eraseBallTrail(int oldHPosition, int oldVPosition, Ball * ball)
{
    // dimensions of the trail
    int trailHPosition, trailVPosition, trailWidth, trailHeight;

    /* if either of the velocity components is greater than the ball size,
     * the trail and the ball don't overlap */
    if ( (ball->vertVelocity >= ball->size) || (ball->horzVelocity >= ball->size) )
	tft.fillRect(oldHPosition,oldVPosition,ball->size,ball->size,BLACK);

    /* if the ball is going down, the trail
     * is partially above it */
    if ( ball->vertVelocity > 0 )
    {
	trailHPosition = oldHPosition;
	trailVPosition = oldVPosition;
	trailWidth = ball->size;
	trailHeight = ball->vertPosition - oldVPosition; //height = vertical displacement

	tft.fillRect(trailHPosition,trailVPosition,trailWidth,trailHeight,BLACK);
    }
    /* if the ball is going up, the trail is partially below it */
    else if ( ball->vertVelocity < 0 )
    {
	trailHPosition = oldHPosition;
	trailVPosition = ball->vertPosition + ball->size; //immediately below the ball
	trailWidth = ball->size;
	trailHeight = oldVPosition - ball->vertPosition; //height = vertical displacement

	tft.fillRect(trailHPosition,trailVPosition,trailWidth,trailHeight,BLACK);
    }

    /* if the ball is going to the right, the trail
     * is partially to the left of it */
    if ( ball->horzVelocity > 0 )
    {
	trailHPosition = oldHPosition;
	trailVPosition = oldVPosition;
	trailWidth = ball->horzPosition - oldHPosition; //width = horizontal displacement
	trailHeight = ball->size;

	tft.fillRect(trailHPosition,trailVPosition,trailWidth,trailHeight,BLACK);
    }
    /* if the ball is going to the left, the trail is partially to the right of it */
    else if ( ball->horzVelocity < 0 )
    {
	trailHPosition = ball->horzPosition + ball->size; //immediately  to the right the ball
	trailVPosition = oldVPosition;
	trailWidth = oldHPosition - ball->horzPosition; //width = horizontal displacement
	trailHeight = ball->size;

	tft.fillRect(trailHPosition,trailVPosition,trailWidth,trailHeight,BLACK);
    }
}


char moveBall(Ball * ball, Paddle * player, Paddle * enemy)
{
    int oldHPosition = ball->horzPosition;
    int oldVPosition = ball->vertPosition;

    ball->horzPosition += ball->horzVelocity;
    ball->vertPosition += ball->vertVelocity;

    eraseBallTrail(oldHPosition,oldVPosition,ball);

    drawBall(ball);

    return 1;
}

void initializeBall(Ball * ball)
{
    // place ball in the middle of the screen
    ball->horzPosition = srcWidth/2 - ball->size/2;
    ball->vertPosition = srcHeight/2 - ball->size/2; 

    // initial horz velocity will range from -(maxVelocity/2) to +(maxVelocity/2)
    ball->horzVelocity = maxVelocity/2 - random(maxVelocity);

    //initial vert velocity is always (maxVelocity/2) towards the blue player (down)
    ball->vertVelocity = (maxVelocity/2);

    drawBall(ball);
}

