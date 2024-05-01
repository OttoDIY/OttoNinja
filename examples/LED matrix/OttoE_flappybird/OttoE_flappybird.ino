// Constants
const uint8_t PROGMEM bird0[] = {...}; // Array of bytes representing the first bird frame
const uint8_t PROGMEM bird1[] = {...}; // Array of bytes representing the second bird frame
const uint8_t PROGMEM bird2[] = {...}; // Array of bytes representing the third bird frame
const byte vibration = A0; // Vibration sensor pin
const int tapLevel = 512; // Tap level threshold
const int topWallPos = 0;
const int bottomWallPos = 15;
const int wallGap = 5;
const int wallDelay = 250; // Time delay between wall movements in milliseconds
const int numLives = 3;
const int gameOverMessageLength = 14; // Length of the "Game Over" message in characters
const int gameOverMessageDelay = 75; // Delay between each character of the "Game Over" message in milliseconds

// Variables
double initYVelocity = 6.5; // Initial vertical velocity of the bird in pixels per frame
double gravity = -9.8; // Gravity constant in pixels per frame^2
long birdTime; // Time elapsed since the bird's last movement in milliseconds
long previousBirdTime; // Time elapsed since the game started in milliseconds
byte birdPos = 3; // Current vertical position of the bird in pixels
byte tailPos = 3; // Current vertical position of the bird's tail in pixels
byte topWall; // Top wall position in pixels
byte bottomWall; // Bottom wall position in pixels
int wallDelayTime; // Time delay between wall movements in milliseconds
int wallCount; // Number of walls passed
long previousWallTime; // Time elapsed since the game started in milliseconds
byte wallPosition = 0; // Current horizontal position of the walls in pixels
boolean gameMode = false; // Game mode flag
long frameTime; // Time elapsed since the last frame update in milliseconds
byte frame = 0; // Current frame number
byte frameChange = 1; // Frame change direction
byte lives = numLives; // Number of lives

void setup() {
  // Initialize the vibration sensor pin as an input
  pinMode(vibration, INPUT_PULLUP);

  // Initialize the LED matrix
  matrix.begin(0x70);
  matrix.setRotation(1);
  matrix.setBrightness(3);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  matrix.clear();
  matrix.writeDisplay();

  // Initialize game variables
  previousBirdTime = millis();
  topWall = random(topWallPos, bottomWallPos - wallGap);
  bottomWall = topWall + wallGap;
  wallPosition = 0;
  gameMode = false;
  wallDelayTime = wallDelay;
  wallCount = 0;
}

void loop() {
  if (gameMode) {
    gameUpdate();
  } else {
    gameStart();
  }
}

void gameStart() {
  matrix.clear();

  // Wait for a tap to start the game
  if (analogRead(vibration) < tapLevel) {
    gameMode = true;
  }

  // Update the frame every 250 milliseconds
  frameTime = millis();
  if (frameTime - previousFrameTime > 250) {
    previousFrameTime = frameTime;
    frame += frameChange;
    if (frame >= 2 || frame <= 0) frameChange *= -1;
  }

  // Display the bird's initial frame
  if (frame == 0) matrix.drawBitmap(0, 0, bird0, 8, 8, LED_ON);
  else if (frame == 1) matrix.drawBitmap(0, 0, bird1, 8, 8, LED_ON);
  else if (frame == 2) matrix.drawBitmap(0, 0, bird2, 8, 8, LED_ON);

  matrix.writeDisplay();
}

void gameUpdate() {
  matrix.clear();

  // Handle tap input and bird movement
  if (analogRead(vibration) < tapLevel) {
    previousBirdTime = millis();
  }

  // Draw the walls and the bird
  drawWalls();
  drawBird();

  // Check for game over
  if (wallPosition == 14) {
    if (checkCollision()) {
      gameOver();
    } else {
      wallCount++;
      wallDelayTime = min(wallDelayTime + 10, wallDelay);
    }
  }

  matrix.writeDisplay();
}

void drawWalls() {
  // Move to the next set of walls when the current set reaches the end
  if (wallPosition > 15) {
    topWall = random(topWallPos, bottomWallPos - wallGap);
    bottomWall = topWall + wallGap;
    wallPosition = 0;
  }

  // Draw the top and bottom walls
  matrix.drawLine(15 - wallPosition, 0, 15 - wallPosition, topWall, LED_ON);
  matrix.drawLine(15 - wallPosition, bottomWall, 15 - wallPosition, 15, LED_ON);

  // Move the walls to the right every wallDelay milliseconds
  if (millis() - previousWallTime > wallDelayTime) {
    previousWallTime = millis();
    wallPosition++;
  }
}

int calculateY(double yVelocity, long elapsedTime) {
  return (int)(birdPos + (yVelocity + gravity * yVelocity / 2.0) * elapsedTime / 1000.0);
}

void drawBirdTail() {
  matrix.drawPixel(1, 7 - tailPos, LED_ON);
}

int calculateNextY(double yVelocity, long elapsedTime) {
  return (int)(birdPos + (yVelocity + gravity * yVelocity / 2.0) * (elapsedTime + 100) / 1000.0);
}

boolean checkCollision() {
  if (birdPos < 0 || birdPos > 15) return true; // Check if the bird is out of bounds vertically
  if (tailPos < 0 || tailPos > 15) return true; // Check if the bird's tail is out of bounds vertically
  if (calculateNextY(initYVelocity, 100) >= bottomWall) return true; // Check if the bird will collide with the bottom wall
  if (calculateNextY(initYVelocity, 100) <= topWall) return true; // Check if the bird will collide with the top wall
  return false;
}

void gameOver() {
  gameMode = false;
  lives--;

  // Display game over message
  const char* gameOverMessage = "Game Over";
  for (int8_t x = 15; x >= -1 * gameOverMessageLength * 5 + 1; x--) {
    matrix.clear();
    matrix.setCursor(x, 0);
    matrix.print(gameOverMessage);
    matrix.writeDisplay();
    delay(gameOverMessageDelay);
  }

  if (lives == 0) {
    // Display game over message
    const char* gameOverMessage = "Game Over";
    for (int8_t x = 15; x >= -1 * gameOverMessageLength * 5 + 1; x--) {
      matrix.clear();
      matrix.setCursor(x, 0);
      matrix.print(gameOverMessage);
      matrix.writeDisplay();
      delay(gameOverMessageDelay);
    }
  } else {
    // Reset game variables
    previousBirdTime = millis();
    topWall = random(topWallPos, bottomWallPos - wallGap);
    bottomWall = topWall + wallGap;
    wallPosition = 0;
    frame = 0;
    frameChange = 1;
  }
}

void updateGameVariables() {
  birdTime = millis() - previousBirdTime;
  birdPos = calculateY(initYVelocity, birdTime);
  tailPos = calculateY(initYVelocity, birdTime - 100);
  if (birdPos < 0) birdPos = 0;
  if (tailPos < 0) tailPos = 0;
}

