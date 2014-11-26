#include "game.h"

extern Adafruit_ST7735 tft;

short roundsToWin = 5;

void start(Paddle * RedPaddle, Paddle * BluePaddle, Ball * ActiveBall)
{
    tft.fillScreen(BLACK);

    RedPaddle->horzPosition = srcWidth /2 - RedPaddle->size/2;
    BluePaddle->horzPosition = srcWidth /2 - RedPaddle->size/2;

    drawPaddle(RedPaddle);
    drawPaddle(BluePaddle);

    initializeBall(ActiveBall);
}

void quit()
{
    tft.fillScreen(BLACK);
}

void finishRound(char result, short& redTotal, short& blueTotal)
{
    switch(result)
    {
        case 'r':
            redTotal++;
            break;
        case 'b':
            blueTotal++;
            break;
    }

    tft.fillScreen(0);
    tft.setCursor(srcWidth/2-(40), srcHeight/2);
    tft.setTextWrap(false);

    // winner screen
    if ((redTotal) >= roundsToWin)
    {
    	tft.setTextColor(WHITE);
    	tft.print("GAME FINISHED\n");
        tft.setCursor(srcWidth/2-(30), srcHeight/2 + 14);
    	tft.print("WINNER: ");
    	tft.setTextColor(RED);
    	tft.print("RED");

    	redTotal = blueTotal = 0;

    	// keep the frame until the user presses the button
    	while (digitalRead(SEL) == HIGH)
        {

        }

    	return;
    }
    else if ((blueTotal) >= roundsToWin)
    {
    	tft.setTextColor(WHITE);
    	tft.print("GAME FINISHED\n");
        tft.setCursor(srcWidth/2-(30), srcHeight/2 + 14);
    	tft.print("WINNER:");

    	tft.setTextColor(BLUE);
    	tft.print("BLUE");

    	redTotal = blueTotal = 0;

    	// keep the frame until the user presses the button
    	while (digitalRead(SEL) == HIGH)
        {

        }

    	return;
    }
    tft.setCursor(srcWidth/2 - 10, srcWidth/2);
  

    // blue result
    tft.setTextColor(BLUE);
    tft.print(blueTotal);

    tft.setTextColor(WHITE);
    tft.print(" - ");

    // red result
    tft.setTextColor(RED);
    tft.print(redTotal);
    // keep the frame until the user presses the button
    while (digitalRead(SEL) == HIGH)
    {

    }
}
