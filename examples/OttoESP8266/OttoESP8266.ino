//----------------------------------------------------------------
//  - Otto DIY Adapted for Wemos D1
//-----------------------------------------------------------------
#include <Servo.h>
#include <EEPROM.h>
#include <Otto.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

Otto Otto;  //This is Otto!
const char* ssid = "otto";
const char* password = "";
ESP8266WebServer server(80);

#define PIN_YL D3 //Left Leg
#define PIN_YR D4 //Right Foot
#define PIN_RL D7 //Left Leg
#define PIN_RR D8 //Right Foot
#define PIN_Buzzer D0
#define PIN_Sensor A0
#define trigPin D5       // Trigger
#define echoPin D6       // Echo

///////////////////////////////////////////////////////////////////
//-- Global VariaBLes -------------------------------------------//
long duration;
int distance;
boolean automatic = false;
String BL, BR, FL, FR;

///////////////////////////////////////////////////////////////////
//-- Setup ----------------l--------------------------------------//
///////////////////////////////////////////////////////////////////

void setup() {

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  bool load_calibration = true;
  readData();
  Serial.println(BL);
  Otto.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR, true, PIN_Sensor, PIN_Buzzer);
  if (BL.toInt() >= 100) {
    Otto.setTrims(0, 0, 0, 0);
  } else {
    Otto.setTrims(BL.toInt(), BR.toInt(), FL.toInt(), FR.toInt());
  }

  //Set webserver
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  server.begin();
  Serial.begin(115200);
  delay(500);
  server.on("/", handle_root);

  server.on("/1", []() {
    parseData("1");
    server.send(200, "text/plain", "FORWARD");
  });

  server.on("/2", []() {
    parseData("2");
    server.send(200, "text/plain", "RIGHT");
  });

  server.on("/3", []() {
    parseData("3");
    server.send(200, "text/plain", "LEFT");
  });

  server.on("/4", []() {
    parseData("4");
    server.send(200, "text/plain", "BACKWARD");
  });

  server.on("/5", []() {
    parseData("5");
    server.send(200, "text/plain", "BEND LEFT");
  });

  server.on("/6", []() {
    parseData("6");
    server.send(200, "text/plain", "BEND RIGHT");
  });

  server.on("/7", []() {
    parseData("7");
    server.send(200, "text/plain", "SHAKE LEFT");
  });

  server.on("/8", []() {
    parseData("8");
    server.send(200, "text/plain", "SHAKE RIGHT");
  });

  server.on("/9", []() {
    parseData("9");
    server.send(200, "text/plain", "UP DOWN");
  });

  server.on("/10", []() {
    parseData("10");
    server.send(200, "text/plain", "MOON WALKER");
  });

  server.on("/11", []() {
    parseData("11");
    server.send(200, "text/plain", "SHAKE RIGHT");
  });

  server.on("/12", []() {
    parseData("12");
    server.send(200, "text/plain", "SWING");
  });

  server.on("/13", []() {
    parseData("13");
    server.send(200, "text/plain", "TIPTOESWING");
  });

  server.on("/14", []() {
    parseData("14");
    server.send(200, "text/plain", "JITTER");
  });

  server.on("/15", []() {
    parseData("15");
    server.send(200, "text/plain", "ASC TURN");
  });

  server.on("/16", []() {
    parseData("16");
    server.send(200, "text/plain", "CRUSAITO");
  });

  server.on("/17", []() {
    parseData("17");
    server.send(200, "text/plain", "FLAP");
  });

  server.on("/18", []() {
    parseData("18");
    server.send(200, "text/plain", "RUN");
  });

  server.on("/19", []() {
    parseData("19");
    server.send(200, "text/plain", "AUTO");
  });

  server.on("/20", []() {
    parseData("20");
    server.send(200, "text/plain", "AUTO OFF");
  });

  server.on("/setting", []() {
    String a = server.arg("a");
    String b = server.arg("b");
    String c = server.arg("c");
    String d = server.arg("d");
    String e = server.arg("e");
    String f = server.arg("f");
    clearData();
    writeData(a, b, c, d, e, f);
    String content = "success";
    server.send(200, "text/html", content);
    ESP.restart();
  });

  server.on("/readsetting", []() {
  //Serial.println(BL + "," + BR + "," + FL + "," + FR + "," + LA + "," + RA);
    server.send(200, "text/plain", BL + "," + BR + "," + FL + "," + FR);

  });

  server.begin();
  Serial.println("HTTP server started");
  //Otto.sing(S_connection);
  Otto.home();
}

void handle_root() {
  server.send(200, "text/plain", "Otto is ready!!!");
  delay(100);
}

///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {
  server.handleClient();
  //Otto.home();
  if (automatic) { //If auto mode is on
    int dist = requestDistance();
    if (dist > 10 ) {
      Serial.print(dist);
      Otto.walk();
    } else {
      Otto.walk(6, 1000, BACKWARD);
      Otto.turn();
      Otto.turn();
      Otto.turn();
    }
  }
}

void parseData(String data) {
  switch (data.toInt()) {
    case 1: // Up
      //Otto.sing(S_OhOoh);
      Otto.walk();
      Otto.home();
      Serial.println("FORWARD");
      break;
    case 2: // RIGHT
      //Otto.sing(S_OhOoh);
      Otto.turn(2, 2000, RIGHT);
      Otto.home();
      Serial.println("RIGHT");
      break;
    case 3: // LEFT
      //Otto.sing(S_OhOoh);
      Otto.turn(2, 2000, LEFT);
      Otto.home();
      Serial.println("LEFT");
      break;
    case 4: // BACK
      //Otto.sing(S_OhOoh);
      Otto.walk(1, 1000, BACKWARD);
      Otto.home();
      Serial.println("BACKWARD");
      break;
    case 5: // BEND LEFT
      //Otto.sing(S_OhOoh);
      Otto.bend();
      Otto.home();
      Serial.println("BEND LEFT");
      break;
    case 6: // BEND RIGHT
      //Otto.sing(S_OhOoh);
      Otto.bend(1, 1400, RIGHT);
      Otto.home();
      Serial.println("BEND RIGHT");
      break;
    case 7: // SHAKE LEFT
      //Otto.sing(S_OhOoh);
      Otto.shakeLeg(1, 2000, LEFT);
      Otto.home();
      Serial.println("SHAKE LEFT");
      break;
    case 8: // SHAKE RIGHT
      //Otto.sing(S_OhOoh);
      Otto.shakeLeg(1, 2000, RIGHT);
      Otto.home();
      Serial.println("SHAKE RIGHT");
      break;
    case 9: // UPDOWN MEDIUM
      //Otto.sing(S_OhOoh);
      Otto.updown();
      Otto.home();
      Serial.println("UPDOWN");
      break;
    case 10: // MOON WALKER
      //Otto.sing(S_OhOoh);
      Otto.moonwalker(5, 900, 20, LEFT);
      Otto.home();
      Serial.println("MOON WALKER");
      break;
    case 11: // JUMP
      //Otto.sing(S_OhOoh);
      Otto.jump();
      Otto.home();
      Serial.println("JUMP");
      break;
    case 12: // SWING
      //Otto.sing(S_OhOoh);
      Otto.swing();
      Otto.home();
      Serial.println("SWING");
      break;
    case 13: // TIPTOESWING
      //Otto.sing(S_OhOoh);
      Otto.tiptoeSwing();
      Otto.home();
      Serial.println("TIPTOESWING");
      break;
    case 14: // JITTER
      //Otto.sing(S_OhOoh);
      Otto.jitter();
      Otto.home();
      Serial.println("JITTER");
      break;
    case 15: // ASC TURN
      //Otto.sing(S_OhOoh);
      Otto.ascendingTurn();
      Otto.home();
      Serial.println("ASC TURN");
      break;
    case 16: // CRUSAITO
      //Otto.sing(S_OhOoh);
      Otto.crusaito();
      Otto.home();
      Serial.println("CRUSAITO");
      break;
    case 17: // FLAP
      //Otto.sing(S_OhOoh);
      Otto.flapping();
      Otto.home();
      Serial.println("FLAP");
      break;
    case 18:
      //Otto.sing(S_OhOoh);
      Otto.walk(15, 300, FORWARD);
      Otto.home();
      Serial.println("RUN");
      break;
    case 19:
      //Otto.sing(S_OhOoh);
      automatic = true;
      Serial.println("AUTO");
      break;
    case 20:
      //Otto.sing(S_OhOoh);
      automatic = false;
      Serial.println("AUTO STOP");
      break;
    case 21:
      Otto.sing(S_OhOoh);
      Otto.home();
      Serial.println("HANDWAVE");
      break;
    case 22:
      Otto.sing(S_OhOoh);
      Otto.home();
      Serial.println("HANDS UP");
      break;
    case 23:
      Otto.sing(S_OhOoh);
      Otto.home();
      Serial.println("HANDS UP DOWN");
      break;
    default:
      Serial.println("HOME");
      Otto.home();
      break;
  }
}

int requestDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  delay(1000);
  return distance;
}

void readData() { //Read all servos trim from EEPROM
  EEPROM.begin(512);
  BL = ""; BR = ""; FL = ""; FR = "";
  Serial.println("Reading From EEPROM..");
  for (int i = 0; i < 3; i++) {
    BL += char(EEPROM.read(i));
  }
  for (int i = 3; i < 6; i++) {
    BR += char(EEPROM.read(i));
  }
  for (int i = 6; i < 9; i++) {
    FL += char(EEPROM.read(i));
  }
  for (int i = 9; i < 12; i++) {
    FR += char(EEPROM.read(i));
  }

  Serial.println("READING DATA");
  Serial.println(BL + "," + BR + "," + FL + "," + FR);
  EEPROM.end();
}

void clearData() { //Clear EEPROM before write
  EEPROM.begin(512);
  Serial.println("Clearing EEPROM ");
  for (int i = 0; i < 512; i++) {
    Serial.print(".");
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  EEPROM.end();
}

void writeData(String a, String b, String c, String d, String e, String f) {
  Serial.println("");
  Serial.println("Writing EEPROM");
  Serial.println(a + "," + b + "," + c + "," + d + "," + e + "," + f);
  clearData();
  EEPROM.begin(512);
  Serial.println("Writing to EEPROM...");
  for (int i = 0; i < 3; i++) {
    EEPROM.write(i, a[i]);
  }
  for (int i = 3; i < 6; i++) {
    EEPROM.write(i, b[i - 3]);
    Serial.println(b[i]);
  }
  for (int i = 6; i < 9; i++) {
    EEPROM.write(i, c[i - 6]);
    Serial.println(c[i]);
  }
  for (int i = 9; i < 12; i++) {
    EEPROM.write(i, d[i - 9]);
    Serial.println(d[i]);
  }
  for (int i = 12; i < 15; i++) {
    EEPROM.write(i, e[i - 12]);
    Serial.println(e[i]);
  }
  for (int i = 15; i < 18; i++) {
    EEPROM.write(i, f[i - 15]);
    Serial.println(f[i]);
  }
  EEPROM.commit();
  EEPROM.end();
  Serial.println("Write Successfull");
}
