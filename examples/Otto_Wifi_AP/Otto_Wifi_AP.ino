#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
//----------------------------------------------------------------
//-- Basic Otto´s setup
//-- You can add functions, gestures or sounds in loop
//-- https://github.com/OttoDIY
//-- CC BY SA (http://ottodiy.com)
//-----------------------------------------------------------------
#include <Servo.h>
#include <Oscillator.h>
#include <Otto.h>
Otto Otto;  //This is Otto!
//---------------------------------------------------------
#define PIN_YL D3 //Left Leg
#define PIN_YR D4 //Right Foot
#define PIN_RL D7 //Left Leg
#define PIN_RR D8 //Right Foot
#define PIN_Buzzer D0
#define PIN_Sensor A0

bool obstacleDetected = false;
int distance = 0;

void handleFWD() {
  Serial.println("FWD");
  String message;
  message += "{ \"success\": ";
  message += "1 }";
  server.send(200, "text/plain", message );
  Otto.walk(2, 1000, 1); //2 steps FORWARD
  Otto.home();
}
void handleBACK() {
  Serial.println("BACK");
  String message;
  message += "{ \"success\": ";
  message += "1 }";
  server.send(200, "text/plain", message );
  Otto.walk(2, 1000, -1);
  Otto.home();
}
void handleRIGHT() {
  Serial.println("RIGHT");
  String message;
  message += "{ \"success\": ";
  message += "1 }";
  server.send(200, "text/plain", message );
  Otto.turn(2, 1000, 1);
  Otto.home();
}
void handleLEFT() {
  Serial.println("LEFT");
  String message;
  message += "{ \"success\": ";
  message += "1 }";
  server.send(200, "text/plain", message );
  Otto.turn(2, 1000, -1);
  Otto.home();
}
void handleMOON() {
  Serial.println("Ooowwwww");
  String message;
  message += "{ \"success\": ";
  message += "1 }";
  server.send(200, "text/plain", message );
  Otto.moonwalker(3, 1000, 25, 1);
  Otto.home();
}
void handleFLAP() {
  Serial.println("Flap");
  String message;
  message += "{ \"success\": ";
  message += "1 }";
  server.send(200, "text/plain", message );
  Otto.flapping(3, 1000, 25, 1);
  Otto.home();
}

void handleHOME() {
  Serial.println("Home");
  String message;
  message += "{ \"success\": ";
  message += "1 }";
  server.send(200, "text/plain", message );
  Otto.home();

}
void handleUPDOWN() {
  Serial.println("UpDown");
  String message;
  message += "{ \"success\": ";
  message += "1 }";
  server.send(200, "text/plain", message );
  Otto.updown(1, 2000, 22);
  Otto.home();
}
void handleBEND() {
  Serial.println("Bend");
  String message;
  message += "{ \"success\": ";
  message += "1 }";
  server.send(200, "text/plain", message );
  Otto.bend (1, 2000, 1);
  Otto.home();
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting: Please connect to DIYODE-AP");

  WiFi.mode(WIFI_AP);
  WiFi.softAP("DIYODE-AP", "1234");
  server.begin(); // Start the HTTP Server
  Serial.print("Please connect to http://192.168.4.1/");

  server.on("/fwd", handleFWD);
  server.on("/back", handleBACK);
  server.on("/right", handleRIGHT);
  server.on("/left", handleLEFT);
  server.on("/moon", handleMOON);
  server.on("/flap", handleFLAP);
  server.on("/home", handleHOME);
  server.on("/updown", handleUPDOWN);
  server.on("/bend", handleBEND);
  server.begin(); //Start the server

  Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, PIN_Sensor, PIN_Buzzer);
  Otto.sing(S_connection); //Otto wake up!
  Otto.home();
  delay(50);
}
void loop() {

  server.handleClient();
  delay(100);

}
