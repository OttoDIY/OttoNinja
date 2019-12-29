/*


 */

#include <ESP32Servo.h>
int pin =2;
void setup() {
  Serial.begin(115200);

}

void loop() {
tone( pin,
      4186,// C
      500);// half a second
tone( pin,
      5274,// E
      500);// half a second
delay(500);



}
