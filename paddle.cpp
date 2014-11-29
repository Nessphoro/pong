
#include "paddle.h"
#include "screen.h"

extern Adafruit_ST7735 tft;


// boundaries of paddle movement
int minPosition = 5;
int maxPosition = srcWidth - 5;

void drawPaddle(Paddle * paddle)
{

    tft.fillRect(paddle->horzPosition,paddle->vertPosition,paddle->size,paddleHeight,paddle->colour);
}

void movePaddle(Paddle * paddle, int disp)
{
    int paddleMaxPosition = maxPosition - paddle->size;
    paddle->horzVelocity = disp;

    // make sure paddle doesn't move beyond edges of the screen
    if ((paddle->horzPosition + disp) <= minPosition)
    {
        disp = minPosition - (paddle->horzPosition);
        paddle->horzVelocity = 0;
    }
    else if ((paddle->horzPosition + disp) >= paddleMaxPosition)
    {
        disp = paddleMaxPosition - (paddle->horzPosition);
        paddle->horzVelocity = 0;
    }

    // do nothing if there's no displacement
    if (disp == 0)
        return;


    // move and re-draw paddle
    paddle->horzPosition += disp;
    drawPaddle(paddle);

    // erase paddle trail

    int trailX; // position of the trail
    int trailLength; // length of trail

    if (disp < 0) // moved left
    {
        trailX = (paddle->horzPosition) + (paddle->size); // right side
        trailLength = (-disp); // abs value of displacement
    }
    else // moved right
    {
        trailX = (paddle->horzPosition) - disp; // left side
        trailLength = (disp); // displacement
    }

    tft.fillRect(trailX,paddle->vertPosition,trailLength,paddleHeight,BLACK);
}
