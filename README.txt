Multiplayer Pong  

Pavlo Malynin - 1418796
Vitor Mendonca - 1401866

Setup
------------------------------------------------------
The game demands 2 Arduinos, 2 Breadboards, 2 Adafruit LCD screens 
and 2 Joysticks.

The equipment should be wired as follows:

Arduino GND to Breadboard -
Arduino 5V to Breadboard +
Arduino 1 TX3 to Arduino 2 RX3
Arduino 2 TX3 to Arduino 1 RX3

Choose one of the Arduinos to be in
Client mode (Blue paddle) and connect its
Pin 13 to GND

Joysticks:
    GND to BB negative bus
    VERT to Arduino A0
    HORZ to Arduino A1
    SEL to Arduino D9
    VCC to BB positive bus
LCD screens:
    GND to BB GND bus
    VCC to BB positive bus
    RESET to Pin 8
    D/C (Data/Command) to Pin 7
    CARD_CS (Card Chip Select) to Pin 5
    TFT_CS (TFT/screen Chip Select) to Pin 6
    MOSI (Master Out Slave In) to Pin 51
    SCK (Clock) to Pin 52
    MISO (Master In Slave Out) to 50
    LITE (Backlite) to BB positive bus    


Instalation
------------------------------------------------------

De-compress the tar file into your folder of choice. Connect one
of your Arduinos and run "make upload" on your terminal. 
Connect the other one on a different USB port and repeat the process.

Gameplay
------------------------------------------------------

As the game starts, the game will anounce what is the colour
of your paddle and start to connect. Upon successful connection, the game will
display a countdown on both screens and the game will start. 

When one of the paddles misses the ball, the player loses the round and a
score is displayed on both screen. The game will resume when both player press
the joystick. The first player to win 5 rounds wins the game and the winner is
anounced on the screens. 

When a moving paddle hits the ball, it transfers some of its velocity to the
ball, so use that as a strategy. 

In each round, every 5 seconds, the ball's maximum velocity increases, to keep rounds from
lasting too long.


File layout
------------------------------------------------------

ball.cpp - Ball physics
paddle.cpp - Paddle specification and behaviour
game.cpp - Game scorekeeping and score screens
Client.cpp - Networking functions for the client
Server.cpp - Networking functions for the server
Utility.cpp - Networking functions common to server and client
screen.h - Constant definitions for the screen, such as size
