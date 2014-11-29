#include <Arduino.h>
#include "ball.h"


const int initialVelocity = 5;

// percentage of the paddle's
// velocity that will be transferred
// to the ball
const int percentage = 30;


extern Adafruit_ST7735 tft;

// boundaries of movement for the ball
const int leftEdge = 0;
const int rightEdge = srcWidth;

void drawBall(Ball * ball)
{
    //the ball is a square with side "ball->size"
    tft.fillRect(ball->horzPosition, ball->vertPosition,
	    ball->size, ball->size, 
	    ball->colour);
}

/* erase the ball trail by painting a black square
 * on the screen where the ball used to be*/
void eraseBallTrail(int oldHPosition, int oldVPosition, Ball * ball)
{
    tft.fillRect(oldHPosition,oldVPosition,ball->size,ball->size,BLACK);
}


/* detect if the ball collided with a surface
 * and adjust its position and velocity accordingly*/
void bounce(Ball * ball, Paddle * red, Paddle * blue, char * winner, int maxVelocity)
{
    int paddleLeftEdge, paddleRightEdge;

    //coordinates of the top of the blue paddle
    //and bottom of the red paddle
    int blueTop = blue->vertPosition;
    int redBottom = red->vertPosition + paddleHeight;


    // verify if ball hit one of the side edge of the screen
    if (ball->horzPosition < leftEdge)
    {
    	ball->horzPosition = leftEdge;
    	ball->horzVelocity = (-(ball->horzVelocity));
    }
    else if ((ball->horzPosition + ball->size )> rightEdge)
    {
    	ball->horzPosition = rightEdge - ball->size;
    	ball->horzVelocity = (-(ball->horzVelocity));
    }

    // verify if the ball reached the top or bottom
    // of the screen and change the winner variable
    // accordingly
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
    	    // transfer some of the paddle's velocity to the ball
    	    ball->horzVelocity += red->horzVelocity * percentage /100;
    	}
    	else
    	    *winner = 'b'; // Winner is the blue paddle
    }
    else if (ball->vertPosition + ball->size > blueTop)
    {
    	paddleLeftEdge = blue->horzPosition;
    	paddleRightEdge = blue->horzPosition + blue->size;

    	if ((ball->horzPosition + ball->size > paddleLeftEdge) && (ball->horzPosition < paddleRightEdge))
    	    // bounce if the ball hit the blue paddle
    	{
    	    ball->vertPosition = blueTop - ball->size;
    	    ball->vertVelocity = (-(ball->vertVelocity));
    	    // transfer some of the paddle's velocity to the ball
    	    ball->horzVelocity += blue->horzVelocity * percentage /100;
    	}
    	else
    	    *winner = 'r'; // Winner is the red paddle
    }

    // make sure that the speed (length of velocity vector) is not greater than MaxVelocity
    // find speed from the 2 velocity components using Pythagoras
    double totalVelocity = ball->horzVelocity *ball->horzVelocity  + ball->vertVelocity*ball->vertVelocity;
    if(totalVelocity > (maxVelocity * maxVelocity))
    {
        double length = sqrt(totalVelocity);
        length = ((double) maxVelocity) / length;
        ball->horzVelocity *= length;
        ball->vertVelocity *= length;

    }

}

/* moves the ball
 * returns a character indicating the winner of the round
 * or 0 if there's no winner */
char moveBall(Ball * ball, Paddle * blue, Paddle * red,int maxVelocity)
{
    // save previous position of the ball to erase the trail
    int oldHPosition = ball->horzPosition;
    int oldVPosition = ball->vertPosition;

    char winner;
    
    // change the ball's position
    // according to its velocity vectors
    ball->horzPosition += ball->horzVelocity;
    ball->vertPosition += ball->vertVelocity;

    // verify if the ball hit something
    // or someone won the round
    bounce(ball,red,blue,&winner,maxVelocity); 

    eraseBallTrail(oldHPosition,oldVPosition,ball);

    drawBall(ball);

    return winner;
}

void initializeBall(Ball * ball)
{
    // place ball in the middle of the screen
    ball->horzPosition = srcWidth/2 - ball->size/2;
    ball->vertPosition = srcHeight/2 - ball->size/2; 

    // initial horz velocity will range from -(initialVelocity/2) to +(initialVelocity/2)
    ball->horzVelocity = initialVelocity/2 - random(initialVelocity);

    //initial vert velocity is always (initialVelocity/2) towards the blue player (down)
    ball->vertVelocity = (initialVelocity/2);

    drawBall(ball);
}

