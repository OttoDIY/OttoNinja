// TONES  ========================================== // Start by defining the relationship between
 //       note, period, &  frequency.
 #define  C     2100
 #define  D     1870 
 #define  E     1670
 #define  f     1580    // Does not seem to like capital F
 #define  G     1400 
 // Define a special note, 'R', to represent a rest
 #define  R     0
 // SETUP ============================================
 // Set up speaker on a PWM pin (digital 9, 10 or 11)
 int speakerOut = 13;
 // Do we want debugging on serial out? 1 for yes, 0 for no
 int DEBUG = 1;
 void setup() {
   pinMode(speakerOut, OUTPUT);
   if (DEBUG) {
     Serial.begin(9600); // Set serial out if we want debugging
   }
 }
 // MELODY and TIMING  =======================================
 //  melody[] is an array of notes, accompanied by beats[],
 //  which sets each note's relative length (higher #, longer note)
 int melody[] = {E, E, E,R,
 E, E, E,R,
 E, G, C, D, E, R,
 f, f, f,f, f, E, E,E, E, D ,D,E, D, R, G ,R,
 E, E, E,R,
 E, E, E,R,
 E, G, C, D, E, R,
 f, f, f,f, f, E, E, E,  G,G, f, D, C,R };
 int MAX_COUNT = sizeof(melody) / 2; // Melody length, for looping.
 // Set overall tempo
 long tempo = 5000;
 // Set length of pause between notes
 int pause = 150;
 // Loop variable to increase Rest length
 int rest_count = 100; //<-BLETCHEROUS HACK; See NOTES
 // Initialize core variables
 int tone_ = 0;
 int beat = 0;
 long duration  = 0;
 // PLAY TONE  ==============================================
 // Pulse the speaker to play a tone for a particular duration
 void playTone() {
   long elapsed_time = 0;
   if (tone_ > 0) { // if this isn't a Rest beat, while the tone has
     //  played less long than 'duration', pulse speaker HIGH and LOW
     while (elapsed_time < duration) {
       digitalWrite(speakerOut,HIGH);
       delayMicroseconds(tone_ / 2);
       // DOWN
       digitalWrite(speakerOut, LOW);
       delayMicroseconds(tone_ / 2);
       // Keep track of how long we pulsed
       elapsed_time += (tone_);
     }
   }
   else { // Rest beat; loop times delay
     for (int j = 0; j < rest_count; j++) { // See NOTE on rest_count
       delayMicroseconds(duration); 
     } 
   }                                
 }
 // LET THE WILD RUMPUS BEGIN =============================
 void loop() {
  for (int i=0; i<MAX_COUNT; i++) {
    tone_ = melody[i];
    beat = 50;

    duration = beat * tempo; // Set up timing

    playTone();
    // A pause between notes...
    delayMicroseconds(pause);
  }
 }
