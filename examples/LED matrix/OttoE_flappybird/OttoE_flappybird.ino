// Arduino Flappy Bird homage by augustzf@gmail.com
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
Adafruit_8x16matrix matrix = Adafruit_8x16matrix();

static const uint8_t PROGMEM
bird0[] =
{ 
  B00011000,
  B00111100,
  B00110100,
  B11111111,
  B10111111,
  B11011100,
  B01111100,
  B00111000 }
,
bird1[] =
{ 
  B00000000,
  B00011000,
  B00111100,
  B00110111,
  B11111111,
  B10011100,
  B11111100,
  B00111000 }
,
bird2[] =
{ 
  B00011000,
  B00111100,
  B00110100,
  B00111111,
  B01111111,
  B11011100,
  B10111100,
  B11111000 };

const byte vibration PROGMEM = A0; // vibration sensor
const int tapLevel PROGMEM = 512;

double initYVelocity = 6.5;
double gravity = -9.8;
long birdTime;
long previousBirdTime;
byte birdPos = 3;
byte tailPos = 3;
byte birdStart;
byte topWall;
byte bottomWall;
int wallDelay = 250;
byte wallGap = 5;
long wallTime;
int wallCount;
long previousWallTime;
byte wallPosition = 0;
boolean gameMode = false;
long frameTime;
long previousFrameTime;
byte frame = 0;
byte frameChange = 1;
byte lives = 2;

void setup() {

  pinMode(vibration, INPUT_PULLUP);

  matrix.begin(0x70);
  matrix.setRotation(1);
  matrix.setBrightness(3);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrix.setTextColor(LED_ON);
  matrix.clear();
  matrix.writeDisplay();
  previousBirdTime = millis();
  topWall = random(0,4);
  bottomWall = topWall + 4;
  wallPosition = 0;
  gameMode = false;
}

void loop() {
  matrix.clear();

  if(gameMode == false)
  {

    if(analogRead(vibration)< tapLevel)
    {
      gameMode = true;
    }

    frameTime = millis();

    if(frameTime - previousFrameTime > 250) {
      previousFrameTime = frameTime;   
      frame += frameChange;
      if(frame >= 2 || frame <= 0) frameChange *= -1;
    }

    if(frame == 0) matrix.drawBitmap(0, 0, bird0, 8, 8, LED_ON);
    else if(frame == 1) matrix.drawBitmap(0, 0, bird1, 8, 8, LED_ON);
    else if(frame == 2) matrix.drawBitmap(0, 0, bird2, 8, 8, LED_ON);
  } 

  else 
  {
    if(analogRead(vibration)<tapLevel)
    {
      previousBirdTime = millis();
      birdStart = birdPos;
    }

    drawWalls();
    drawBird();

    if(wallPosition == 14)
    {
      byte next = 15 - calculateNextY();
      if(next >= bottomWall || next <= topWall)
      {
        gameMode = false;

        int scoreInt = wallCount-1;

        const char* temp = "Game Over";
   
        for (int8_t x=15; x>=-1*9*5+1; x--)
        {
          matrix.clear();
          matrix.setCursor(x,0);
          matrix.print(temp);
          matrix.writeDisplay();
          delay(75);
        }
      }
      wallCount++;
    }

  }

  matrix.writeDisplay(); 
}

void drawWalls()
{
  wallTime = millis();

  if(wallPosition > 15) 
  {
    topWall = random(0,4);
    bottomWall = topWall + 4;
    wallPosition = 0;
  }

  matrix.drawLine(15-wallPosition, 0, 15-wallPosition, topWall, LED_ON); 

  matrix.drawLine(15-wallPosition, bottomWall, 15-wallPosition, 15, LED_ON); 

  if(wallTime - previousWallTime > wallDelay) {
    previousWallTime = wallTime;   
    wallPosition++;
  }
}

int calculateY()
{
  return (int)(birdStart+((birdTime/1000.0)*(initYVelocity+gravity*(birdTime/1000.0)/2.0)));
}

int calculateTail()
{
  return (int)(birdStart+(((birdTime-100)/1000.0)*(initYVelocity+gravity*((birdTime-100)/1000.0)/2.0)));
}

int calculateNextY()
{
  return (int)(birdStart+(((birdTime+100)/1000.0)*(initYVelocity+gravity*((birdTime+100)/1000.0)/2.0)));
}

void drawBird()
{
  birdTime = millis() - previousBirdTime;
  birdPos = calculateY();
  tailPos = calculateTail();
  if(calculateY() < 0) birdPos = 0;
  if(calculateTail() < 0) tailPos = 0;
  matrix.drawPixel(0, 7-tailPos, LED_ON); 
  matrix.drawPixel(1, 7-birdPos, LED_ON); 
  matrix.writeDisplay(); 
}
