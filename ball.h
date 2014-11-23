#include "paddle.h"
#include "screen.h"


typedef struct
{
    int size;
    int colour;
    // position of the top left corner of the ball
    int horzPosition;
    int vertPosition;
    // horizontal and vertical components of velocity (pixels/loop cycle)
    int horzVelocity;
    int vertVelocity;
} Ball;


void drawBall(Ball * ball);

void eraseBallTrail(int oldHPosition, int oldVPosition, Ball * ball);

char moveBall(Ball * ball, Paddle * player, Paddle * enemy);

void initializeBall(Ball * ball);
