# Otto Ninja Robot Arduino Libraries

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
![version](https://img.shields.io/badge/version-1.1-blue)

This repository have all the main [Otto Ninja robot](https://www.ottodiy.com/ninja) libraries for ESP compatible boards

Created by [Sebastian Coddington](https://github.com/SebastianCoddington)

:star: Star us on GitHub, it helps!

## Installation:

You will need [Arduino IDE in your computer](https://www.arduino.cc/en/software)

1. [Download the .zip Otto Ninja libraries here](https://github.com/OttoDIY/OttoNinja/archive/master.zip)
2. Open Arduino IDE and navigate to Sketch > Include Library > Add .ZIP Library.
3. Navigate to the .zip file's location, that you just downloaded and open it.
4. You will see in the bottom black area a message that it has been installed. 
5. To verify they are properly installed, go to Sketch > Include Library menu. You should now see the library at the bottom of the drop-down menu.

That means it is ready to use Otto example codes! you can find them in File > Examples > OttoNinja
[for more details or other ways to install libraries visit this link](https://www.arduino.cc/en/Guide/Libraries) 

For the LED matrix example codes you will need the [Adafruit LED Bacck pack library and the dependences](https://github.com/adafruit/Adafruit_LED_Backpack/archive/refs/heads/master.zip) just do the exact same installation steps.

For the OLED display example codes you will need the [U8g2 libraries](https://github.com/olikraus/u8g2/archive/refs/heads/master.zip)

Additionally you will need to [add the ESP8266 to your Arduino IDE](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/)

The board only flash the LED a little (this is the way ESP8266 boards work unless you program them to behave different)



## Uploading:

Firmware code for remote control of Ninja, select NodeMCU 1.0 as board. Connect the Ninja board with the USB cable to your computer, NO servos or anything else connected and we check if can upload code first.

1. [Upload this code](https://github.com/OttoDIY/OttoNinja/blob/master/examples/App/OttoNinja_APP/OttoNinja_APP.ino) 
2. [Download tp your phone](https://remotexy.com/en/download/)
2. Open the App and press + Add new device
3. Select Wifi point
4. Select "OTTO NINJA" visible from the options
6. The password is 12345678

(Make sure to have wifi disconnected from other networks) and power Ninja with battery all time.
with Otto Blockly  you do not need to deal with all the libraries and stuff

## Compatible Hardware

- Designed to work with ESP8266 boards but most of the code could be adapted to other microcontrollers

 ## How to Contribute:
Contributing to this software is warmly welcomed. There are 3 ways you can contribute to this project:
1. Test and if find a problem then post an issue.
2. Helps us solve the issues or other bugs.
3. Improve and optimize the curren libraries.
You can do this [basically by forking](https://help.github.com/en/articles/fork-a-repo), committing modifications and then [pulling requests](https://help.github.com/en/articles/about-pull-requests). 

Please add a change log and your contact into file header.

Thanks for your contribution and welcome to the [Otto DIY community](http://builders.ottodiy.com/)

## License CC-BY-SA
You can use all resources of Otto for free but Otto DIY Website must be included in any redistribution and remixes must keep the CC-BY-SA license, As you might seen with the original Otto DIY project the idea is that more people can have access, therefore if you copy or remix Otto you must also release under same open license, that means you must release all files also to the public.

<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br /><span xmlns:dct="http://purl.org/dc/terms/" property="dct:title">Otto DIY</span> by <a xmlns:cc="http://creativecommons.org/ns#"  property="cc:attributionName"> [www.ottodiy.com](http://www.ottodiy.com) </a> is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.

Otto DIY website must be included in any redistribution and remixes must keep the CC-BY-SA license.