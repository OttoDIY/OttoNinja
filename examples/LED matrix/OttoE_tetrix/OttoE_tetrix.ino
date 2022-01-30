// Game code is by Nathan Pryor
// with inspiration from the LoLShield_Tetris code by Aurélien Couderc
// http://benjamin.sonntag.fr/All-games-for-Arduino-Lol-Shield-Pong-Shoot-them-up-Tetris

// Button debounce code is from Adafruit:
// http://www.adafruit.com/blog/2009/10/20/example-code-for-multi-button-checker-with-debouncing/
/*****************************************************************************************/

#define DEBOUNCE 10  // button debouncer, how many ms to debounce, 5+ ms is usually plenty

// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
byte buttons[] = {14,15,16,17}; // the analog 0-5 pins are also known as 14-19
// analog 0 = rotate (bottom switch on joystick)
// analog 1 = move right (left switch on joystick)
// analog 2 = drop (top switch on joystick)
// analog 3 = move left (right switch on joystick)

// This handy macro lets us determine how big the array up above is, by checking the size
#define NUMBUTTONS sizeof(buttons)
// we will track if a button is just pressed, just released, or 'currently pressed' 
volatile byte pressed[NUMBUTTONS], justpressed[NUMBUTTONS], justreleased[NUMBUTTONS];

#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_8x16matrix matrixTop = Adafruit_8x16matrix();

int score; // the total score
int totalLines; // number of lines cleared
int level;  // the current level, should be totalLines/10
int stepCounter; // increments with every program loop. resets after the value of gravityTrigger is reached
int gravityTrigger; // the active piece will drop one step after this many steps of stepCounter;

/* the grid of pieces fixed on the playfield */
byte theGrid[] = {   
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000
    };

int activeShape; // the index of the currently active shape
int currentRotation=0; // the index of the current piece's rotation

int yOffset=0; // how far from the top the active shape is
int xOffset=0; // how far from the left the active shape is

/* Define a multidimensional array of the pieces in every rotation */
int shapes[7][4][4][2]={
        /*  square  */    {
           /* angle 0   */   {  {1,0}, {2,0}, {1,1}, {2,1} },
           /* angle 90  */   {  {1,0}, {2,0}, {1,1}, {2,1} },
           /* angle 180 */   {  {1,0}, {2,0}, {1,1}, {2,1} },
           /* angle 270 */   {  {1,0}, {2,0}, {1,1}, {2,1} } // I know it's redundant since they're all the same, but I like the consistency
                          },
  
        /*  bar  */        {
           /* angle 0   */   {  {0,1}, {1,1}, {2,1}, {3,1} },
           /* angle 90  */   {  {1,0}, {1,1}, {1,2}, {1,3} },
           /* angle 180 */   {  {0,1}, {1,1}, {2,1}, {3,1} },
           /* angle 270 */   {  {1,0}, {1,1}, {1,2}, {1,3} }
                          },  
        /*  Z  */       {
           /* angle 0   */   {  {0,0}, {1,0}, {1,1}, {2,1} },
           /* angle 90  */   {  {2,0}, {1,1}, {2,1}, {1,2} },
           /* angle 180 */   {  {0,0}, {1,0}, {1,1}, {2,1} },
           /* angle 270 */   {  {2,0}, {1,1}, {2,1}, {1,2} }
                          },   
        /*  S  */       {
           /* angle 0   */   {  {1,0}, {2,0}, {0,1}, {1,1} },
           /* angle 90  */   {  {0,0}, {0,1}, {1,1}, {1,2} },
           /* angle 180 */   {  {1,0}, {2,0}, {0,1}, {1,1} },
           /* angle 270 */   {  {0,0}, {0,1}, {1,1}, {1,2} }
                          },
        /*  L  */       {
           /* angle 0   */   {  {0,1}, {1,1}, {2,1}, {0,2} },
           /* angle 90  */   {  {1,0}, {1,1}, {1,2}, {2,2} },
           /* angle 180 */   {  {2,0}, {0,1}, {1,1}, {2,1} },
           /* angle 270 */   {  {0,0}, {1,0}, {1,1}, {1,2} }   
                          },
        /*  J  */       {
           /* angle 0   */   {  {0,1}, {1,1}, {2,1}, {2,2} },
           /* angle 90  */   {  {1,0}, {2,0}, {1,1}, {1,2} },
           /* angle 180 */   {  {0,0}, {0,1}, {1,1}, {2,1} },
           /* angle 270 */   {  {1,0}, {1,1}, {1,2}, {0,2} }                       
                          },
        /* T  */          {
           /* angle 0   */   {  {0,1}, {1,1}, {2,1}, {1,2} },
           /* angle 90  */   {  {1,0}, {1,1}, {2,1}, {1,2} },
           /* angle 180 */   {  {1,0}, {0,1}, {1,1}, {2,1} },
           /* angle 270 */   {  {1,0}, {0,1}, {1,1}, {1,2} } 
                          }         
                  };
  
/********************************************************************************/  

void startGame(){
    gravityTrigger=20;                     // reset the trigger point at which the active piece will drop another step
    level=1;                               // reset the level
    score=0;                               // reset the score
    totalLines=0;                          // reset the line count
    for (int row=0; row<16; row++){        // reset the fixed-piece grid
      theGrid[row]=B00000000;
    }
    
    void launchNewShape();                 // kick off the first shape
}


/********************************************************************************/
// check whether the next move requested is valid
boolean checkNextMove(int nextRot , int nextXOffset, int nextYOffset){
  
  boolean isOK=true;  // assume the move is valid. we'll change isOK if it's not

  if (nextRot>3){nextRot=0;}; //rotate back to the original 0-index position if needed

  for (int thisPixel=0; thisPixel < 4; thisPixel++){  // Check all 4 pixels of the shape in play    
    // have we gone out of bounds left or right?
    if (shapes[activeShape][nextRot][thisPixel][0]+nextXOffset >(7)||shapes[activeShape][nextRot][thisPixel][0]+nextXOffset<0){
      isOK=false;
      break; //no need to check further
    }
    
    //have we hit the bottom of the grid?
    if (shapes[activeShape][nextRot][thisPixel][1]+nextYOffset >(15)){
        isOK=false;       
        break; //no need to check further
    }
    
    //have we collided with any other shapes?    
    int inverseCol=7-(shapes[activeShape][nextRot][thisPixel][0]+nextXOffset); // to make the next IF easier to read
    int theRow=(shapes[activeShape][nextRot][thisPixel][1])+nextYOffset;       // to make the next IF easier to read
    if(bitRead(theGrid[theRow],inverseCol)){
      isOK=false;
      break; //no need to check further
    }
  }
  return isOK;  
}

/********************************************************************************/
/* When movement is done, write the final placement of the active piece to the fixed grid */
void storeFinalPlacement(){
    for (int thisPixel=0; thisPixel < 4; thisPixel++){                            //run through all 4 pixels of the shape  
      int pixelX=shapes[activeShape][currentRotation][thisPixel][0] + xOffset;    //calculate its final, offset X position
      int pixelY=shapes[activeShape][currentRotation][thisPixel][1] + yOffset;    //calculate its final, offset Y position
      bitSet(theGrid[pixelY], 7-pixelX);                                          //flip the bit on in the grid in the corresponding location
    }
  }
  
/********************************************************************************/
/* Launch a new shape from the top of the screen */
void launchNewShape(){
  activeShape=random(6); // pick a random shape
  yOffset=0;             // reset yOffset so it comes from the top 
  xOffset=3;             // reset xOffset so it comes from the middle
  currentRotation=0;     // pieces start from the default rotation
  gravityTrigger=max(1,21-level);   // set the gravity appropriately. this is so that a drop doesn't carry to the next piece
  
  
  //can the piece be placed in the starting location? 
  //if not, the stack is full so end the game
  if (!checkNextMove(currentRotation, xOffset, yOffset)){
    endGame();
    displayScores();
    matrixTop.clear();

    matrixTop.writeDisplay();

    delay(5000);  //wait 5 seconds before restarting
    startGame();
  }        
}

/********************************************************************************/
void endGame(){
   byte blankGrid[16]= //create a blank grid to use for blinking
   {   
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000
    };
   for (int blinkCount=0; blinkCount<3; blinkCount++){
    //blink the fixed pieces 3 times by alternating between the blank grid and the played grid 
    // this section for blank grid
     matrixTop.clear();

     drawFixedPieces(blankGrid);
     matrixTop.writeDisplay();

     delay(150);
     // this section for played grid
     matrixTop.clear();

     drawFixedPieces(theGrid);
     matrixTop.writeDisplay();

     delay(150);
    }
}

/********************************************************************************/
/* Check for completed rows and clear them, then drop pieces above */
void dropFullRows(){
  byte theGridBlink[16]; // this will be a copy of the fixed-piece grid, but with full rows empty
                         // alternating between it and the actual fixed-piece grid will make the full rows blink
  int fullRows[4]; // to store the Y location of full rows. There can never be more than four at a time
  int fullRowCount=0;
  for (int row=0; row<16; row++){       // step through each row of the played grid
      if (theGrid[row]==B11111111){    // if the row is full then
        theGridBlink[row]=B00000000;   // set this row in the blinking version of the grid to empty.
        fullRows[fullRowCount]=row;    // record the position of the filled row
        fullRowCount++;                 // increment the count of full rows       
      }else{
        theGridBlink[row]=theGrid[row];// if it's not empty, duplicate the row from the played grid
      }
  }
  
 
  if (fullRowCount>0){                 // if there is a filled row, blink all filled rows 
    for (int blinkCount=0; blinkCount<3; blinkCount++){ 
    //blink the filled rows 3 times by alternating between the blink grid and the played grid 
    // this section for blink grid 
      matrixTop.clear();              // clear the top matrix
        // clear the bottom matrix
      drawFixedPieces(theGridBlink);  // draw the blink version of the grid
      matrixTop.writeDisplay();       // update the top matrix
    // update the bottom matrix
      delay(150);
    // this section for played grid
      matrixTop.clear();              // clear the top matrix
         // clear the bottom matrix
      drawFixedPieces(theGrid);       // draw the blink version of the grid
      matrixTop.writeDisplay();       // update the top matrix
  // update the bottom matrix
      delay(150);
    }
    
    //  remove the filled rows and drop them
    for (int i=0; i<fullRowCount; i++){                       // we only need to do this for as many filled rows as there are      
      for (int copyRow=fullRows[i]; copyRow>0; copyRow--){    // for every row above the filled row
          theGrid[copyRow]=theGrid[copyRow-1];                // copy the row above into the one below
        }
        theGrid[0]=B00000000;                                 // now that everything is bumped down by one, fill the top row with blanks  
      }
      
    
    totalLines=totalLines+fullRowCount;    //  add the filled rows to the total line count
  
    //  add points to the score
   int thePoints; 
    switch(fullRowCount){
      case 1:  // Single = 100 x Level
       thePoints=100 * level;
       break;
      case 2:  // Double = 300 x Level
       thePoints=300 * level;
       break;
      case 3:  // Triple = 500 x Level
       thePoints=500 * level;
       break;
      case 4:  // Tetris = 800 x Level
       thePoints=800 * level; 
       break;
    }
    
    score=score+thePoints;  // add the points to the total score
    
    
    //increment the level every 10 lines
    if (totalLines%10==0){
      level++;
      gravityTrigger--; // and with every level, decrease the trigger value at which stepCounter will drop the piece. This makes it faster with every level 
    }
  }
}

/*********************************************************************************/
void displayScores(){
  matrixTop.clear();
  matrixTop.writeDisplay();
  matrixTop.setTextSize(1);
  matrixTop.setTextWrap(false);  // we don't want text to wrap, so it scrolls nicely
//  matrixTop.setTextColor(LED_ON);
  
  
  String levelString=String(level); // convert the level integer to a string
  String pointString=String(score); // convert the score integer to a string
  
  String theLevel=String("Level " + levelString);   // build a string containing the word "level" + the level value
  String theScore=String("Points " + pointString);  // build a string containing the word "points" + the point value
  
  int theLevelLength = 38 + (7 * levelString.length()); // estimating the pixel width of the string
  int theScoreLength = 44 + (7 * pointString.length()); // estimating the pixel width of the string
  
  for (int8_t x=8; x>= -(theLevelLength); x--) {       // start offscreen and scroll the level from right to left
    matrixTop.clear();
    matrixTop.setCursor(x,0);
    matrixTop.print(theLevel);
    matrixTop.writeDisplay();
    delay(50);
  }
   
}

/*********************************************************************************/
void drawFixedPieces(byte gridToDraw[16]){
  
    for (int row=0; row<16; row++){               // run through every row
      for (int col=0; col <8; col++){            // and every column(bit)
        if (bitRead(gridToDraw[row],7-col)){     // if the bit is 1, then
            if (row<16){
               matrixTop.drawPixel(col, row, LED_ON);  // turn that pixel on
            }else{
//               matrixBottom.drawPixel(col, row-8, LED_ON);  // turn that pixel on
            }
          
      }
    }
  } 
}

/********************************************************************************/
void drawActivePiece(){
 for (int thisPixel=0; thisPixel < 4; thisPixel++){                  //run through all 4 pixels of the shape
    int pixelX=shapes[activeShape][currentRotation][thisPixel][0];   //get the X coordinate
    int pixelY=shapes[activeShape][currentRotation][thisPixel][1]+yOffset;   //get the Y coordinate
    if (pixelY<16){
      matrixTop.drawPixel(pixelX + xOffset, pixelY, LED_ON); //draw each pixel, offset by the current location of the shape 
    }else {
//matrixBottom.drawPixel(pixelX + xOffset, pixelY-8, LED_ON); //draw each pixel, offset by the current location of the shape
    } 
  } 
}


/********************************************************************************/
void check_switches()
{
  static byte previousstate[NUMBUTTONS];
  static byte currentstate[NUMBUTTONS];
  static long lasttime;
  byte index;

  if (millis() < lasttime) {
     // we wrapped around, lets just try again
     lasttime = millis();
  }

  if ((lasttime + DEBOUNCE) > millis()) {
    // not enough time has passed to debounce
    return;
  }
  // ok we have waited DEBOUNCE milliseconds, lets reset the timer
  lasttime = millis();

  for (index = 0; index < NUMBUTTONS; index++) {

    currentstate[index] = digitalRead(buttons[index]);   // read the button
    if (currentstate[index] == previousstate[index]) {
      if ((pressed[index] == LOW) && (currentstate[index] == LOW)) {
          // just pressed
          justpressed[index] = 1;
      }
      else if ((pressed[index] == HIGH) && (currentstate[index] == HIGH)) {
          // just released
          justreleased[index] = 1;
      }
      pressed[index] = !currentstate[index];  // remember, digital HIGH means NOT pressed
    }
    //Serial.println(pressed[index], DEC);
    previousstate[index] = currentstate[index];   // keep a running tally of the buttons
  }
}
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
/********************************************************************************/
void setup()   
{   
  
 stepCounter=0;
 Serial.begin(9600); 
 
  matrixTop.begin(0x70);             // initialize the top matrix

  
  matrixTop.setRotation(4);          // set the rotation of the top matrix

  
  matrixTop.setBrightness(1);        // set the brightness of the top matrix

  
   
    // Make input & enable pull-up resistors on switch pins
  byte i;
  for (i=0; i< NUMBUTTONS; i++) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
  }
  
    // Run timer2 interrupt every 15 ms 
  TCCR2A = 0;
  TCCR2B = 1<<CS22 | 1<<CS21 | 1<<CS20;

  //Timer2 Overflow Interrupt Enable
  TIMSK2 |= 1<<TOIE2;
  
 startGame();
}

SIGNAL(TIMER2_OVF_vect) {
  check_switches();
}
/********************************************************************************/
void loop()                   
{  
  for (byte i = 0; i < NUMBUTTONS; i++) {
    
    //The left, right, and rotate switches are triggered once then need to be released and re-triggered. 
    //They can't be held down for continuous movement or rotation
    
    if (justpressed[i]) {
      justpressed[i] = 0;     
      switch (i){
        case 0: // rotate
          if (checkNextMove(currentRotation+1, xOffset, yOffset)){
             currentRotation++;
             if (currentRotation>3){currentRotation=0;};
          }; 
          break;
        case 1: // move right
          if (checkNextMove(currentRotation, xOffset+1, yOffset)){
            xOffset++;
          };
          break;
          
        case 2: // accelerate the drop speed
          gravityTrigger=min(3, gravityTrigger/2); // increase the speed of the drop to the lowest of either 3 or 1/2 the current speed
                                                     // it's probably not the method used in the real game, but it was a quick solution 
          break;
          
        case 3: // move left
          if (checkNextMove(currentRotation, xOffset-1, yOffset)){
            xOffset--;  
          }
          break;
      } 
    }
    
    if (justreleased[2]) {                          // when the down button is released
      gravityTrigger=max(1,21-level);               // reset the drop speed to its normal speed for that level
      justreleased[2]=0; 
    }
  
     
     //drop the piece one step every N ticks. N=gravityTrigger
     
      if(stepCounter>gravityTrigger){              // if the program has looped more times than the gravityTrigger then
        stepCounter=0;                             // reset the loop counter
        
        if (checkNextMove(currentRotation, xOffset, yOffset+1)){  // check to see if the piece can be dropped a level
          yOffset++;                                              // if so, then do it
          
          }else{                                                  // if not,
            storeFinalPlacement();                                // record the final location of that block
            dropFullRows();                                       // clear out any full rows and drop the ones above
            launchNewShape();                                     // drop a new shape from the top
          }
      }
      stepCounter++;    // increment the count of the number of program loops


     matrixTop.clear();            // clear the top matrix

     drawFixedPieces(theGrid);     // draw the fixed pieces in their locations
     drawActivePiece();            // draw the active piece

     matrixTop.writeDisplay();     // update the top LEDs with the new info

     
     delay(30);                    // wait 30ms before looping again


}
}
