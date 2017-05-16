/*
ConfigBluetooth HC-05 from Arduino.
Change name, password and baud using serial port with AT commands, only configurate one time
After upload the sketch connect your module, the LED should blinking every 2 seconds.
If LED is blinking too fast, you need to disconnect and connect again but this time pressing the reset little button of the BT module
Then you can open your serial (Both NL & CR 9600 baud) and should appear and "OK" message means is ready to recieve AT commands. (AT+NAME=)
After setup you can pair your module
HC-05 GND --- Arduino GND Pin
HC-05 VCC (5V) --- Arduino 5V
HC-05 TX --- Arduino Pin 10 (soft RX)
HC-05 RX --- Arduino Pin11 (soft TX)
HC-05 Key (PIN 34) --- Arduino Pin 9
*/
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11); // RX | TX

void setup()
{
  pinMode(9, OUTPUT);  // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode
  digitalWrite(9, HIGH);
  Serial.begin(9600);
  Serial.println("Enter AT commands:");
  BTSerial.begin(38400);  // HC-05 default speed in AT command more
}

void loop()
{

  // Keep reading from HC-05 and send to Arduino Serial Monitor
  if (BTSerial.available())
    Serial.write(BTSerial.read());

  // Keep reading from Arduino Serial Monitor and send to HC-05
  if (Serial.available())
    BTSerial.write(Serial.read());
}
