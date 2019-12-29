#include <TimerFreeTone.h>

#define TONE_PIN 10 // Pin you have speaker/piezo connected to (be sure to include a 100 ohm resistor).

// Melody (liberated from the toneMelody Arduino example sketch by Tom Igoe).
int melody[] = { 262, 196, 196, 220, 196, 0, 247, 262 };
int duration[] = { 250, 125, 125, 250, 250, 250, 250, 250 };

void setup() {}

void loop() {
  for (int thisNote = 0; thisNote < 8; thisNote++) { // Loop through the notes in the array.
    TimerFreeTone(TONE_PIN, melody[thisNote], duration[thisNote]); // Play melody[thisNote] for duration[thisNote].
    delay(50); // Short delay between notes.
  }
  while(1); // Stop (so it doesn't repeat forever driving you crazy--you're welcome).
}