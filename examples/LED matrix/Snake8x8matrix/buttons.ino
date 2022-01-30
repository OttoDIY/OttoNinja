int* buttonPin;
int* buttonState;

void initButtons(int pinNumbers[], int length){
  Serial.println("initButtons");
  // Copy PIN numbers
  buttonPin = new int[length];
  for(int i=0; i<length; i++){
    buttonPin[i] = pinNumbers[i];
  }
  // Initialize button state
  buttonState = new int[length];
  for(int i=0; i<length; i++){
    buttonState[i] = LOW;
  }
  
  for(int i=0; i<length; i++)
  {
    Serial.print(buttonPin[i]);
    Serial.print(buttonState[i]);
  }
  
}

boolean buttonClicked(int index){
  // current state
  int state = digitalRead(buttonPin[index]);
  boolean buttonClick = false;
  // button has pressed
  if(buttonState[index] == HIGH){
    if(state == LOW){
      buttonClick = true;
    }
  }
  buttonState[index] = state;
  return buttonClick;
}
