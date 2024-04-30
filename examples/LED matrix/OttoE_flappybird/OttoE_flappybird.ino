// Constants
Adafruit_8x16matrix matrix = Adafruit_8x16matrix(); // LED matrix object
const uint8_t PROGMEM bird0[] = {...}; // Array of bytes representing the first bird frame
const uint8_t PROGMEM bird1[] = {...}; // Array of bytes representing the second bird frame
const uint8_t PROGMEM bird2[] = {...}; // Array of bytes representing the third bird frame
const byte vibration = A0; // Vibration sensor pin
const int tapLevel = 512; // Tap level threshold


// Variables
double initYVelocity = 6.5; // Initial vertical velocity of the bird
double gravity = -9.8; // Gravity constant
long birdTime; // Time elapsed since the bird's last movement
long previousBirdTime; // Time elapsed since the game started
byte birdPos = 3; // Current vertical position of the bird
byte tailPos = 3; // Current vertical position of the bird's tail
byte birdStart; // Initial vertical position of the bird
byte topWall; // Top wall position
byte bottomWall; // Bottom wall position
int wallDelay = 250; // Time delay between wall movements
byte wallGap = 5; // Gap between walls
long wallTime; // Time elapsed since the last wall movement
int wallCount; // Number of walls passed
long previousWallTime; // Time elapsed since the game started
byte wallPosition = 0; // Current horizontal position of the walls
boolean gameMode = false; // Game mode flag
long frameTime; // Time elapsed since the last frame update
long previousFrameTime; // Time elapsed since the game started
byte frame = 0; // Current frame number
byte frameChange = 1; // Frame change direction
byte lives = 2; // Number of lives


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
  topWall = random(0,4);
  bottomWall = topWall + 4;
  wallPosition = 0;
  gameMode = false;
}


void loop() {
  matrix.clear();

  if(gameMode == false)
  {
    // Wait for a tap to start the game
    if(analogRead(vibration)< tapLevel)
    {
      gameMode = true;
    }

    // Update the frame every 250 milliseconds
    frameTime = millis();
    if(frameTime - previousFrameTime > 250) {
      previousFrameTime = frameTime;
      frame += frameChange;
      if(frame >= 2 || frame <= 0) frameChange *= -1;
    }

    // Display the bird's initial frame
    if(frame == 0) matrix.drawBitmap(0, 0, bird0, 8, 8, LED_ON);
    else if(frame == 1) matrix.drawBitmap(0, 0, bird1, 8, 8, LED_ON);
    else if(frame == 2) matrix.drawBitmap(0, 0, bird2, 8, 8, LED_ON);
  } 

  else 
  {
    // Handle tap input and bird movement
    if(analogRead(vibration)<tapLevel)
    {
      previousBirdTime = millis();
      birdStart = birdPos;
    }

    // Draw the walls and the bird
    drawWalls();
    drawBird();

    // Check for game over
    if(wallPosition == 14)
    {
      byte next = 15 - calculateNextY();
      if(next >= bottomWall || next <= topWall)
      {
        gameMode = false;

        // Display game over message
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


// Draw the walls on the LED matrix
void drawWalls()
{
  wallTime = millis();

  // Move to the next set of walls when the current set reaches the end
  if(wallPosition > 15) 
  {
    topWall = random(0,4);
    bottomWall = topWall + 4;
    wallPosition = 0;
  }

  // Draw the top and bottom walls
  matrix.drawLine(15-wallPosition, 0, 15-wallPosition, topWall, LED_ON); 
  matrix.drawLine(15-wallPosition, bottomWall, 15-wallPosition, 15, LED_ON); 

  // Move the walls to the right every wallDelay milliseconds
  if(wallTime - previousWallTime > wallDelay) {
    previousWallTime = wallTime;   
    wallPosition++;
  }
}

// Calculate the bird's vertical position based on the elapsed time
int calculateY()
{
  return (int)(birdStart+((birdTime/1000.0)*(initYVelocity+gravity*(birdTime/1000.0)/2.0)));
}

// Calculate the bird's tail position based on the elapsed time
int calculateTail()
{
  return (int)(birdStart+(((birdTime-100)/1000.0)*(initYVelocity+gravity*((birdTime-100)/1000.0)/2.0)));
}

// Calculate the bird's next vertical position based on the elapsed time
int calculateNextY()
{
  return (int)(birdStart+(((birdTime+100)/1000.0)*(initYVelocity+gravity*((birdTime+100)/1000.0)/2.0)));
}

// Draw the bird on the LED matrix
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
