#include "game.h"

extern Adafruit_ST7735 tft;

short roundsToWin = 5;

// initialize round
// place paddles in the middle and initialize ball
void start(Paddle * RedPaddle, Paddle * BluePaddle, Ball * ActiveBall)
{
    tft.fillScreen(BLACK);

    RedPaddle->horzPosition = srcWidth /2 - RedPaddle->size/2;
    BluePaddle->horzPosition = srcWidth /2 - RedPaddle->size/2;

    drawPaddle(RedPaddle);
    drawPaddle(BluePaddle);

    initializeBall(ActiveBall);
}

// shows a countdown on the screen to the start of the game
void countdown()
{
    for(int i=0;i<5;i++)
    {
        tft.fillScreen(BLACK);
        tft.setTextSize(4);
        tft.setCursor(srcWidth/2 - 10, srcHeight/2-10);
        tft.print(5-i);
        delay(1000);
    }
    tft.setTextSize(1);
}

/* end round, display score
 * and declare a winner if
 * there is one*/
void finishRound(char result, short& redTotal, short& blueTotal)
{
    // increment score of the winner
    switch(result)
    {
        case 'r':
            redTotal++;
            break;
        case 'b':
            blueTotal++;
            break;
    }

    // set the screen to black
    // and place text cursor in the middle
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

        // reset scores
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

        // reset scores
    	redTotal = blueTotal = 0;

    	// keep the frame until the user presses the button
    	while (digitalRead(SEL) == HIGH)
        {

        }

    	return;
    }

    //place text cursor in the middle
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
