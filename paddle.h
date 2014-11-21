#pragma once


// paddle specifications
const int paddleHeight = 6;
const int paddleWidth = 30;

typedef struct
{
    int size;

    // horizontal position of the left edge of the paddle
    int horzPosition;

    int player;

    int vertPosition; //vertical position

    int colour;
} Paddle;


void drawPaddle(Paddle * paddle);


void movePaddle(Paddle * paddle, int disp);

