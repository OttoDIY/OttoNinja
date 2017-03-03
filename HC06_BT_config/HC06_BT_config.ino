/* 
	http://giltesa.com/?p=11738
	09/08/2012
	(CC) BY-NC-SA: giltesa.com
Modified by Pablo García pabloeweb@gmail.com for Otto DIY robot
ConfigBluetooth HC-06 from Arduino.
Change name, password and baud using serial port, with no user interaction. 
1.- Upload this code to Arduino nano (no bluetooth module connected)
2.- unplug USB
3.- Connect HC-06
4.- Power arduino from external power.
5.- Wait for the Arduino´s onboard led to start blinking
6.- Disconnect power. Everything is done.

After upload code you have 10 seconds to connect the module
BT ---- Arduino
TX ----> RX
RX <---- TX

Once the LED off configuration will start and at the end LED will blink
After this you can pair your module
*/
// Options:
	int ArduLED=13;				//Internal Arduino´s LED
	char ssid[10]		= "Zowi";	// Name for Bluetooth.
	char baudios		 = '8';		   // 1=>1200 baudios, 2=>2400, 3=>4800, 4=>9600 (default), 5=>19200, 6=>38400, 7=>57600, 8=>115200
	char password[10]	= "1234";		// Password for pairing

void setup()
{
	Serial.begin(9600);		//9600bauds is the deafult baudrate for these modules.
					//if it´s not working try changing this baudrate to match your HC-06 initial setup
	
	// Waiting time (10 seconds) onboard LED is ON:
		pinMode(ArduLED,OUTPUT);
		digitalWrite(ArduLED,HIGH);
		delay(10000);
		digitalWrite(ArduLED,LOW);
	
Serial.print("AT"); delay(1000); // Now configuration start

Serial.print("AT+NAME"); Serial.print(ssid); delay(1000);   // Change Name of BT

Serial.print("AT+BAUD"); Serial.print(baudios); delay(1000);    // Change Baud

Serial.print("AT+PIN"); Serial.print(password); delay(1000);	    // Change Password
}

void loop()
{
	// After programing bluetooth, onboard LED will Blink.
	digitalWrite(ArduLED, !digitalRead(ArduLED));
	delay(500);
}
