Library version: 3.1.6

This code is the library for Arduino IDE. The library is part of the RemoteXY project. 
RemoteXY allows you to control your device using an Android or iOS smartphone. There are Bluetooth, WiFi, Ethernet, USB OTG or Cloud server can be used for connection.

For more details and make GUI go to website http://remotexy.com/


History:
   version 2.3.1    
     - Support the device access password;
     - Support the cloud server as beta test;
     - Fixed a bug where the length of variables more than 255;
     - Fixed a bug where ESP module reboot and device did not see it;
     - Fixed a bug where the connection was filed and the device 
         did not see it and reconnection is impossible 
   version 2.3.4  
     - Fixed a bug where the length of all input variables more than 256;
     - Fixed a bug where millis() overflow in 50 days;
     - Fixed some bugs;
   version 2.3.5  
     - Fixed some bugs;
   version 2.4.1
     - support ESP32 WiFi and Bluetooth   
   version 2.4.2
     - Fixed some bugs;
   version 2.4.3
     - Fixed some bugs;
   version 2.4.4
     - Fixed ESP32 BLE bugs;
   version 3.1.1
     - Full update the library: multiple connections, set input variables from board and more;
   version 3.1.3
     - Multiple connection for cloud, fixed some bugs;
   version 3.1.4
     - fixed some bugs;
   version 3.1.5
     - add NRF5XXX and BLEPeripheral.h library as Beta, need define REMOTEXY_MODE__NRFCORE_BLEPERIPHERAL;
   version 3.1.6
     - fixed some bugs;
   version 3.1.7
     - add CDC Serial for Leonardo, Micro, ..;     