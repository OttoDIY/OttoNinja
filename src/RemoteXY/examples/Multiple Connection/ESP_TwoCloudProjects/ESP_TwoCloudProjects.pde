/*
   Example: two cloud projects on one board, two tokens are used
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.1 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.5.1 or later version;
     - for iOS 1.4.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library
#define REMOTEXY__DEBUGLOG 

#if defined (ESP8266)
  #include <ESP8266WiFi.h>
#elif defined (ESP32)
  #include <WiFi.h>  
#endif

#include <RemoteXY.h>


// RemoteXY configurate  

/////////////////////////////////////////////////////////////
// First

#pragma pack(push, 1)
uint8_t const PROGMEM RemoteXY_CONF_PROGMEM_1[] =
  { 255,1,0,1,0,27,0,10,13,2,
  1,0,9,9,46,46,6,7,50,50,
  2,31,88,0,65,4,62,16,31,31,
  14,62,35,35 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t button_1; // =1 if button pressed, else =0 

    // output variables
  uint8_t led_1_r; // =0..255 LED Red brightness 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY_1;


/////////////////////////////////////////////////////////////
// Second

uint8_t const PROGMEM RemoteXY_CONF_PROGMEM_2[] =
  { 255,1,0,1,0,19,0,11,27,0,
  1,0,11,7,30,30,94,31,88,0,
  65,4,59,30,21,21 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t button_1; // =1 if button pressed, else =0 

    // output variables
  uint8_t led_1_r; // =0..255 LED Red brightness 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY_2;

#pragma pack(pop)



/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////


CRemoteXY *remotexy_1;
CRemoteXY *remotexy_2;

void setup() 
{
  
  CRemoteXYComm_WiFi * wifi =  new CRemoteXYComm_WiFi (
    "myHomeFiFi",                            // REMOTEXY_WIFI_SSID
    "myPass"                       // REMOTEXY_WIFI_PASSWORD
  );
  
  remotexy_1 = new CRemoteXY (RemoteXY_CONF_PROGMEM_1, &RemoteXY_1, ""); 
  remotexy_1->addConnection (new CRemoteXYConnectionCloud ( wifi, 
    "cloud.remotexy.com",                     // REMOTEXY_CLOUD_SERVER 
    6376,                                     // REMOTEXY_CLOUD_PORT
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"        // REMOTEXY_CLOUD_TOKEN
  ));
  
  remotexy_2 = new CRemoteXY (RemoteXY_CONF_PROGMEM_2, &RemoteXY_2, ""); 
  remotexy_2->addConnection (new CRemoteXYConnectionCloud ( wifi, 
    "cloud.remotexy.com",                     // REMOTEXY_CLOUD_SERVER 
    6376,                                     // REMOTEXY_CLOUD_PORT
    "YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY"        // REMOTEXY_CLOUD_TOKEN
  ));  
  
  // TODO you setup code
  
}

void loop() 
{ 
  remotexy_1->handler ();
  remotexy_2->handler ();
  
  if (RemoteXY_1.button_1)  RemoteXY_1.led_1_r = 255;
  else RemoteXY_1.led_1_r = 0;

  if (RemoteXY_2.button_1)  RemoteXY_2.led_1_r = 255;
  else RemoteXY_2.led_1_r = 0;

  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay() 


}
