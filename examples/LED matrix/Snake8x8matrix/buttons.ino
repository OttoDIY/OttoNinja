// Define an integer pointer for storing button pin numbers
int* buttonPin;

// Define an integer pointer for storing button states
int* buttonState;

/**
 * Initialize button pins and states
 * @param pinNumbers An array of button pin numbers
 * @param length The number of buttons
 */
void initButtons(int pinNumbers[], int length){
  Serial.println("initButtons");
  // Copy PIN numbers
  buttonPin = new int[length]; // Allocate memory for buttonPin array
  for(int i=0; i<length; i++){
    buttonPin[i] = pinNumbers[i]; // Copy pinNumbers array elements to buttonPin array
  }
  // Initialize button state
  buttonState = new int[length]; // Allocate memory for buttonState array
  for(int i=0; i<length; i++){
    buttonState[i] = LOW; // Initialize all button states to LOW
  }
  
  // Print button pin numbers and states for debugging
  for(int i=0; i<length; i++)
  {
    Serial.print(buttonPin[i]);
    Serial.print(buttonState[i]);
  }
  
}

/**
 * Check if a button is clicked
 * @param index The index of the button to check
 * @return true if the button is clicked, false otherwise
 */
boolean buttonClicked(int index){
  // current state
  int state = digitalRead(buttonPin[index]); // Read the current state of the button
  boolean buttonClick = false; // Initialize buttonClick to false
  // button has pressed
  if(buttonState[index] == HIGH){ // If the button was previously pressed
    if(state == LOW){ // And it is now released
      buttonClick = true; // The button is clicked
    }
  }
  buttonState[index] = state; // Update the button state
  return buttonClick; // Return the buttonClick value
}

