#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
const char* ssid = "";
const char* pass =  "*";
#include <Servo.h>
#include <Oscillator.h>
#include <Otto.h>
Otto Otto;  //This is Otto!

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
}

void handleBACK() {
  Serial.println("BACK");
  String message;
  message += "{ \"success\": ";
  message += "1 }";
  server.send(200, "text/plain", message );
  Otto.walk(2, 1000, -1);
}

void handleRIGHT() {
  Serial.println("RIGHT");
  String message;
  message += "{ \"success\": ";
  message += "1 }";
  server.send(200, "text/plain", message );
  Otto.turn(2,1000,1);
}

void handleLEFT() {
  Serial.println("LEFT");
  String message;
  message += "{ \"success\": ";
  message += "1 }";
  server.send(200, "text/plain", message );
  Otto.turn(2,1000,-1);
}

void handleMOON() {
  Serial.println("Ooowwwww");
  String message;
  message += "{ \"success\": ";
  message += "1 }";
  server.send(200, "text/plain", message );
  Otto.moonwalker(3, 1000, 25, 1);
}

void handleHOME() {
  Serial.println("Ooowwwww");
  String message;
  message += "{ \"success\": ";
  message += "1 }";
  server.send(200, "text/plain", message );
  Otto.home();

}
void handleUPDOWN() {
  Serial.println("Ooowwwww");
  String message;
  message += "{ \"success\": ";
  message += "1 }";
  server.send(200, "text/plain", message );
  Otto.updown(1, 2000, 22);

}
void handleBEND() {
  Serial.println("Ooowwwww");
  String message;
  message += "{ \"success\": ";
  message += "1 }";
  server.send(200, "text/plain", message );
  Otto.bend (1, 2000, 1);
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/fwd", handleFWD);
  server.on("/back", handleBACK);
  server.on("/right", handleRIGHT);
  server.on("/left", handleLEFT);
  server.on("/moon", handleMOON);
  server.on("/home", handleHOME);
  server.on("/updown", handleUPDOWN);
  server.on("/bend", handleBEND);
  server.begin(); //Start the server
  Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, PIN_Sensor, PIN_Buzzer);
  Otto.home();
  delay(50);
}

void loop() {
  server.handleClient();
  delay(100);
}
