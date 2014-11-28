#pragma once
#include "screen.h"
#include "paddle.h"
#include "ball.h"



void finishRound(char result, short &redTotal, short &blueTotal);

void start(Paddle * RedPaddle, Paddle * BluePaddle, Ball * ActiveBall);

void countdown();