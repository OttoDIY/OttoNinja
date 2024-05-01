// Define a struct for storing button information
struct Button {
  int pinNumber;   // Pin number to which the button is connected
  int state;       // Current state of the button (LOW or HIGH)
};

// Initialize button pins and states
void initButtons(Button buttons[], int length) {
  Serial.println("initButtons");    // Print a debug message for initialization
  
  // Initialize button states
  for(int i=0; i<length; i++){
    buttons[i].state = LOW;  // Initialize all button states to LOW
  }
  
  // Print button pin numbers and states for debugging
  for(int i=0; i<length; i++)
  {
    Serial.print(buttons[i].pinNumber);  // Print button pin number
    Serial.print(buttons[i].state);      // Print button state
  }
  
}

/**
 * Check if a button is clicked
 * @param buttons An array of button structs
 * @param index The index of the button to check
 * @return true if the button is clicked, false otherwise
 */
boolean buttonClicked(Button buttons[], int index) {
  // current state
  int state = digitalRead(buttons[index].pinNumber);  // Read the current state of the button
  
  boolean buttonClick = false;  // Initialize buttonClick to false
  
  // button has pressed
  if(buttons[index].state == HIGH){  // If the button was previously pressed
    if(state == LOW){  // And it is now released
      buttonClick = true;  // The button is clicked
    }
  }
  
  buttons[index].state = state;  // Update the button state
  
  return buttonClick;  // Return the buttonClick value
}

