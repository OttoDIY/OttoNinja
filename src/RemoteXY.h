/*
RemoteXY Library - Remote device control
version 3.1.7

This library allows you to control an Arduino-compatible device from a
smartphone or tablet using the RemoteXY service. It supports various
communication modes, including HardwareSerial, SoftwareSerial, ESP8266
AT commands, WiFi, Ethernet, and Bluetooth.

For more information, visit http://remotexy.com

This library is free software; you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.
*/

#ifndef _REMOTEXY_H_
#define _REMOTEXY_H_

#include <inttypes.h>
#include <Arduino.h>
#include "RemoteXYDebugLog.h"

using std::uint16_t;
using std::uint32_t;

#if REMOTEXY_MAX_CLIENTS < 1
#define REMOTEXY_MAX_CLIENTS 4
#endif

class CRemoteXY {
public:
    CRemoteXY(const PROGMEM RemoteXY_CONF_PROGMEM& conf, RemoteXY_Api* api, const char* access_password, CRemoteXYStream* stream);
    ~CRemoteXY();
    bool isConnected();
    void handler();

private:
    const PROGMEM RemoteXY_CONF_PROGMEM& conf_;
    RemoteXY_Api* api_;
    const char* access_password_;
    CRemoteXYStream* stream_;
};

// Stream classes
#include "RemoteXYStream_HardSerial.h"
#include "RemoteXYStream_SoftSerial.h"
#include "RemoteXYStream_CDCSerial.h"
#include "RemoteXYStream_BluetoothSerial.h"
#include "RemoteXYStream_BLEDevice.h"
#include "RemoteXYStream_BLEPeripheral.h"

// Communication classes
#include "RemoteXYComm_WiFi.h"
#include "RemoteXYComm_Ethernet.h"
#include "RemoteXYComm_ESP8266.h"

// Connection classes
#include "RemoteXYConnectionServer.h"
#include "RemoteXYConnectionCloud.h"

// API
inline bool RemoteXY_isConnected() {
    return remotexy->isConnected();
}

// Initialize the RemoteXY object
void RemoteXY_begin(const PROGMEM RemoteXY_CONF_PROGMEM& conf, RemoteXY_Api* api, const char* access_password, CRemoteXYStream* stream) {
    remotexy = new CRemoteXY(conf, api, access_password, stream);
}

#endif // _REMOTEXY_H_
