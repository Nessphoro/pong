#include "game.h"

extern Adafruit_ST7735 tft;

const int roundsToWin = 5;

void start(Paddle * RedPaddle, Paddle * BluePaddle, Ball * ActiveBall)
{
    tft.fillScreen(BLACK);

    drawPaddle(RedPaddle);
    drawPaddle(BluePaddle);

    initializeBall(ActiveBall);
}

void quit()
{
    tft.fillScreen(BLACK);
}

void finishRound(char result, short * redTotal, short * blueTotal)
{
    tft.fillScreen(0);
    tft.setCursor(srcWidth/2-3, srcHeight/2);
    tft.setTextWrap(false);

    // winner screen
    if (*redTotal >= roundsToWin)
    {
	tft.setTextColor(WHITE);
	tft.print("GAME FINISHED\n");
	tft.print("WINNER:\n");
	tft.setTextColor(RED);
	tft.print("RED");

	*redTotal = *blueTotal = 0;

	// keep the frame until the user presses the button
	while (digitalRead(SEL) == HIGH)
	    ;

	return;
    }
    else if (*blueTotal >= roundsToWin);
    {
	tft.setTextColor(WHITE);
	tft.print("GAME FINISHED\n");
	tft.print("WINNER:\n");
	tft.setTextColor(BLUE);
	tft.print("BLUE");

	*redTotal = *blueTotal = 0;

	// keep the frame until the user presses the button
	while (digitalRead(SEL) == HIGH)
	    ;

	return;
    }



    // blue result
    tft.setTextColor(BLUE);
    tft.print("%d",blueTotal);

    tft.setTextColor(WHITE);
    tft.print(" - ");

    // red result
    tft.setTextColor(RED);
    tft.print("%d",redTotal);
    // keep the frame until the user presses the button
    while (digitalRead(SEL) == HIGH)
	;
}
