/*
    Library modified from: "SerialCommand.h" by Steven Cogswell http://awtfy.com
     -- Removed portion of the original library to not interfere with interruptions 
     -- (disable SoftwareSerial support, and thus don't have to use "#include <SoftwareSerial.h>" in the sketches)
*/



#ifndef OttoSerialCommand_h
#define OttoSerialCommand_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <string.h>
#include "Stream.h"

#if defined(ESP32)
#include "BluetoothSerial.h"

# if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
# error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
# endif
#endif

#define SERIALCOMMANDBUFFER 35  //16 after changed by me
#define MAXSERIALCOMMANDS	16
#define MAXDELIMETER 2

class OttoSerialCommand: public Stream
{
	public:
		OttoSerialCommand();      // Constructor
 
		// We need these to be a Stream (Serial)
		bool begin(String bluetoothName=String());
		void begin(int);
        int available(void);
        int peek(void);
        int read(void);
        size_t write(uint8_t c);
        size_t write(const uint8_t *buffer, size_t size);
        void flush();
//        void end(void);

		void clearBuffer();   // Sets the command buffer to all '\0' (nulls)
		char *next();         // returns pointer to next token found in command buffer (for getting arguments to commands)
		void readSerial();    // Main entry point.  
		void addCommand(const char *, void(*)());   // Add commands to processing dictionary
		void addDefaultHandler(void (*function)());    // A handler to call when no valid command received.
		bool isBluetooth(void);
	
	private:
		char inChar;          // A character read from the serial stream 
		char buffer[SERIALCOMMANDBUFFER];   // Buffer of stored characters while waiting for terminator character
		int  bufPos;                        // Current position in the buffer
		char delim[MAXDELIMETER];           // null-terminated list of character to be used as delimeters for tokenizing (default " ")
		char term;                          // Character that signals end of command (default '\r')
		char *token;                        // Returned token from the command buffer as returned by strtok_r
		char *last;                         // State variable used by strtok_r during processing
		typedef struct _callback {
			char command[SERIALCOMMANDBUFFER];
			void (*function)();
		} OttoSerialCommandCallback;            // Data structure to hold Command/Handler function key-value pairs
		int numCommand;
		OttoSerialCommandCallback CommandList[MAXSERIALCOMMANDS];   // Actual definition for command/handler array
		void (*defaultHandler)();           // Pointer to the default handler function 
		int usingOttoSoftwareSerial;        // Used as boolean to see if we're using OttoSoftwareSerial object or not
		bool usingBluetooth;
};

#endif //OttoSerialCommand_h
