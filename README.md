# Otto Ninja Robot Arduino Libraries

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
![version](https://img.shields.io/badge/version-1.1-blue)

This repository has the main [Otto Ninja robot](https://www.ottodiy.com/ninja) libraries for ESP compatible boards.

## Compatible Hardware

- Designed to work with ESP8266 boards, but most of the code could be adapted to other microcontrollers.

You will need to [add the ESP8266 to your Arduino IDE](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/)

:star: Star us on GitHub; it helps!

## Installation:

You will need the [Arduino IDE on your computer](https://www.arduino.cc/en/software)

1. [Download Otto Ninja libraries here](https://github.com/OttoDIY/OttoNinja/archive/master.zip)
2. Open the Arduino IDE and navigate to Sketch > Include Library > Add .ZIP Library...
3. Navigate to .zip file location that you just downloaded and open it.
4. You will see in the bottom black area a message that it has been installed. 
5. To verify they are properly installed, go to Sketch > Include Library menu. You should now see the library at the bottom of the drop-down menu.

## Example:

You can find codes in File > Examples > OttoNinja.

*For more details or other ways to [install libraries, visit this link](https://www.arduino.cc/en/Guide/Libraries) 

*For the LED matrix, you will need the [Adafruit LED Backpack library and all the dependencies](https://github.com/adafruit/Adafruit_LED_Backpack/archive/refs/heads/master.zip) just do the exact same installation steps.

*For the OLED display, you will need the [U8g2 libraries](https://github.com/olikraus/u8g2/archive/refs/heads/master.zip)

## Uploading:

For the firmware code to remote control Ninja via the [Remote XY mobile App](https://remotexy.com), select NodeMCU 1.0 as the board.

1. Connect the ESP board using the USB cable to your computer.

2. [Upload this code](https://github.com/OttoDIY/OttoNinja/blob/master/examples/App/OttoNinja_APP/OttoNinja_APP.ino) 
3. [Download the app to your phone](https://remotexy.com/en/download/)
4. Open the app and press + Add new device.
5. Select the WiFi point.
6. Select "OTTO NINJA" from the options.
7. The password is 12345678.

(Make sure to have wifi disconnected from other networks) and power the robot with a battery all the time.

## How to Contribute:
Contributing to this software is warmly welcomed.
1. Test it, and if you find any problems, then post an issue.
2. Help us solve the issues or other bugs.
3. Improve and optimize the current libraries.
You can do this [basically by forking](https://help.github.com/en/articles/fork-a-repo), committing modifications, and then [pull a request](https://help.github.com/en/articles/about-pull-requests). 

Thanks for your contribution, and welcome to the [Otto DIY community](https://www.ottodiy.com/community)

## License: CC-BY-SA
You can use all resources of Otto for free, but Otto DIY website must be included in any redistribution, and remixes must keep the CC-BY-SA license. In open source, the idea is that more people can have access; therefore, if you copy or remix Otto, you must also release it under the same open license, which means you must also release all files to the public.

<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br /><span xmlns:dct="http://purl.org/dc/terms/" property="dct:title">Otto DIY</span> by <a xmlns:cc="http://creativecommons.org/ns#"  property="cc:attributionName"> [www.ottodiy.com](http://www.ottodiy.com) </a> is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.

Thanks to [Sebastian Coddington](https://github.com/SebastianCoddington) for this project.
