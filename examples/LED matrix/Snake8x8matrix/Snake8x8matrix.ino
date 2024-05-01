// Snake on 8x8 Matrix
// 2013-06-15 JorgVisch
//
// This is a simple implementation of the classic Snake game using an 8x8 LED matrix
// and buttons for controlling the snake's movement.

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

// Button pin definitions
const int buttonRightPin = 14;
const int buttonLeftPin = 16;

// Game constants
const int RIGHT_BUTTON = 0;
const int LEFT_BUTTON = 1;
const int TOP = 0;
const int RIGHT = 1;
const int BOTTOM = 2;
const int LEFT = 3;
const int MAX_SNAKE_LENGTH = 10;
const int MATRIX_ADDRESS = 0x70;

// Variables
Adafruit_8x8matrix matrix = Adafruit_8x8matrix();  // Display
int direction = TOP;                               // direction of movement
int snakeX[MAX_SNAKE_LENGTH];                      // X-coordinates of snake
int snakeY[MAX_SNAKE_LENGTH];                      // Y-coordinates of snake
int snakeLength = 1;                               // nr of parts of snake
boolean buttonRead = false;                        // is button already read in this loop
unsigned long prevTime = 0;                        // for gamedelay (ms)
unsigned long delayTime = 500;                     // Game step in ms

int fruitX, fruitY;
unsigned long fruitPrevTime = 0;
unsigned long fruitBlinkTime = 1000 / 250;
int fruitLed = LED_ON;

void setup() {
  Serial.begin(9600);
  Serial.println("Snake is started");

  // Init buttons
  int buttonpins[] = {buttonRightPin, buttonLeftPin};
  if (initButtons(buttonpins, 2) != 0) {
    Serial.println("Error initializing buttons");
    return;
  }

  // Init matrix
  if (initMatrix(MATRIX_ADDRESS) != 0) {
    Serial.println("Error initializing matrix");
    return;
  }

  // Init snake
  snakeX[0] = 4;
  snakeY[0] = 7;
  for (int i = 1; i < MAX_SNAKE_LENGTH; i++) {
    snakeX[i] = snakeY[i] = -1;
  }
  makeFruit();
}

void loop() {
  checkButtons();
  unsigned long currentTime = millis();
  if (currentTime - prevTime >= delayTime) {
    nextStep();
    buttonRead = false;
    prevTime = currentTime;
  }
  clearMatrix();
  drawSnake();
  drawFruit();
  matrix.writeDisplay();
}

// Check for button clicks and update the direction variable accordingly
void checkButtons() {
  if (!buttonRead) {
    int currentDirection = direction;
    if (buttonClicked(LEFT_BUTTON)) {
      direction--;
      if (direction < 0) {
        direction = LEFT;
      }
    } else if (buttonClicked(RIGHT_BUTTON)) {
      direction++;
      if (direction > 3) {
        direction = TOP;
      }
    }
    buttonRead = (currentDirection != direction);
  }
}

// Draw the snake on the LED matrix
void drawSnake() {
  for (int i = 0; i < snakeLength; i++) {
    drawPixel(snakeX[i], snakeY[i], LED_ON);
  }
}

// Draw the fruit on the LED matrix
void drawFruit() {
  if (inPlayField(fruitX, fruitY)) {
    unsigned long currenttime = millis();
    if (currenttime - fruitPrevTime >= fruitBlinkTime) {
      fruitLed = (fruitLed == LED_ON) ? LED_OFF : LED_ON;
      fruitPrevTime = currenttime;
    }
    drawPixel(fruitX, fruitY, fruitLed);
  }
}

// Check if the given coordinates are within the playfield
boolean inPlayField(int x, int y) {
  return (x >= 0) && (x < 8) && (y >= 0) && (y < 8);
}

// Move the snake and check for collisions
void nextStep() {
  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }
  switch (direction) {
    case TOP:
      snakeY[0] = snakeY[0] - 1;
      break;
    case RIGHT:
      snakeX[0] = snakeX[0] + 1;
      break;
    case BOTTOM:
      snakeY[0] = snakeY[0] + 1;
      break;
    case LEFT:
      snakeX[0] = snakeX[0] - 1;
      break;
  }
  if (checkCollision() || checkGameOver()) {
    gameOver();
  } else if (checkEatFruit()) {
    updateFruit();
    snakeLength++;
  }
}

// Generate a new fruit at a random position that is not part of the snake
void makeFruit() {
  int x, y;
  x = random(0, 8);
  y = random(0, 8);
  while (isPartOfSnake(x, y)) {
    x = random(0, 8);
    y = random(0, 8);
  }
  fruitX = x;
  fruitY = y;
}

// Check if the given coordinates are part of the snake
boolean isPartOfSnake(int x, int y) {
  for (int i = 0; i < snakeLength - 1; i++) {
    if ((x == snakeX[i]) && (y == snakeY[i])) {
      return true;
    }
  }
  return false;
}

// Check if the snake has collided with itself
boolean checkCollision() {
  for (int i = 1; i < snakeLength; i++) {
    if ((snakeX[0] == snakeX[i]) && (snakeY[0] == snakeY[i])) {
      return true;
    }
  }
  return false;
}

// Check if the snake has eaten the fruit
boolean checkEatFruit() {
  if ((snakeX[0] == fruitX) && (snakeY[0] == fruitY)) {
    return true;
  }
  return false;
}

// Check if the game is over
boolean checkGameOver() {
  if ((snakeX[0] < 0) || (snakeX[0] >= 8) || (snakeY[0] < 0) || (snakeY[0] >= 8)) {
    return true;
  }
  return false;
}

// Game over sequence
void gameOver() {
  // Add game over sequence here
}

// Initialize the matrix
int initMatrix(int address) {
  Wire.begin();
  matrix.begin(address);
  matrix.clear();
  matrix.writeDisplay();
  return 0;
}

// Clear the matrix
void clearMatrix() {
  matrix.clear();
}

// Draw a pixel on the matrix
void drawPixel(int x, int y, int led) {
  matrix.drawPixel(x, y, led);
}

// Initialize the buttons
int initButtons(int *buttonpins, int numButtons) {
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonpins[i], INPUT_PULLUP);
  }
  return 0;
}

// Check if a button has been clicked
boolean buttonClicked(int button) {
  if (digitalRead(button) == LOW) {
    delay(50);
    if (digitalRead(button) == LOW) {
      return true;
    }
  }
  return false;
}
