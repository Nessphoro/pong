#include <Arduino.h>
#include "ball.h"


const int maxVelocity = 20;


extern Adafruit_ST7735 tft;

// boundaries of movement for the ball
const int leftEdge = 0;
const int rightEdge = srcWidth;
const int redCourt = 2 + paddleHeight; // as high as the red paddle
const int blueCourt = srcHeight - 2 - paddleHeight; // as high as the blue paddle


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

    Serial.print(" trailHPosition ");
    Serial.println(trailHPosition);
    Serial.print(" trailVPosition ");
    Serial.print(trailVPosition);
    Serial.print(" trailWidth ");
    Serial.println(trailWidth);
    Serial.print(" trailHeight ");
    Serial.println(trailHeight);
}


/* detect if the ball collided with a surface
 * and adjust its position and velocity accordingly*/
void bounce(Ball * ball, Paddle * red, Paddle * blue, char * winner)
{
    int paddleLeftEdge, paddleRightEdge;

    //coordinates of the top of the blue paddle
    //and bottom of the red paddle
    int blueTop = blue->vertPosition;
    int redBottom = red->vertPosition + paddleHeight;
    

    if (ball->horzPosition < leftEdge)
    {
	ball->horzPosition = leftEdge;
	ball->horzVelocity = (-(ball->horzVelocity));
    }
    else if (ball->horzPosition + ball->size > rightEdge)
    {
	ball->horzPosition = rightEdge - ball->size;
	ball->horzVelocity = (-(ball->horzVelocity));
    }

    *winner = 0;
    if (ball->vertPosition < redBottom)
    {
	paddleLeftEdge = red->horzPosition;
	paddleRightEdge = red->horzPosition + red->size;

	if (((ball->horzPosition + ball->size) > paddleLeftEdge) && (ball->horzPosition < paddleRightEdge))
	    // bounce if the ball hit the red paddle
	{
	    ball->vertPosition = redBottom;
	    ball->vertVelocity = (-(ball->vertVelocity));
	}
	else
	    *winner = 'b';
    }
    else if (ball->vertPosition + ball->size > blueTop)
    {
	paddleLeftEdge = blue->horzPosition;
	paddleRightEdge = blue->horzPosition + blue->size;

	if ((ball->horzPosition + ball->size > paddleLeftEdge) && (ball->horzPosition < paddleRightEdge))
	    // bounce if the ball hit the blue paddle
	{
	    Serial.println();
	    Serial.println("HIT PADDLE!");
	    Serial.println();
	    ball->vertPosition = blueTop - ball->size;
	    ball->vertVelocity = (-(ball->vertVelocity));
	}
	else
	    *winner = 'r';
    }
}

/* moves the ball
 * returns a character indicating the winner of the round
 * or 0 if there's no winner */
char moveBall(Ball * ball, Paddle * blue, Paddle * red)
{
    int oldHPosition = ball->horzPosition;
    int oldVPosition = ball->vertPosition;
    char winner;
    
    ball->horzPosition += ball->horzVelocity;
    ball->vertPosition += ball->vertVelocity;

    bounce(ball,red,blue,&winner); 

    eraseBallTrail(oldHPosition,oldVPosition,ball);

    drawBall(ball);

    return winner;
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

