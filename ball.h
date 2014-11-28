#pragma once
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
    double horzVelocity;
    double vertVelocity;
} Ball;


void drawBall(Ball * ball);

void bounce(Ball * ball, Paddle * red, Paddle * blue, char * winner, int maxVelocity);

void eraseBallTrail(int oldHPosition, int oldVPosition, Ball * ball);

char moveBall(Ball * ball, Paddle * blue, Paddle * red, int maxVelocity);

void initializeBall(Ball * ball);
