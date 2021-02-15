/*
    Library modified from: "SerialCommand.h" by Steven Cogswell http://awtfy.com
     -- Removed portion of the original library to not interfere with interruptions 
     -- (disable SoftwareSerial support, and thus don't have to use "#include <SoftwareSerial.h>" in the sketches)
*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "OttoSerialCommand.h"

#include <string.h>

#if defined(ESP32)
BluetoothSerial SerialBT;
#endif

// Constructor makes sure some things are set. 
OttoSerialCommand::OttoSerialCommand()
{
	strncpy(delim," ",MAXDELIMETER);  // strtok_r needs a null-terminated string
	term='\r';   // return character, default terminator for commands
	numCommand=0;    // Number of callback handlers installed
	clearBuffer();
}

//
// Initialize the command buffer being processed to all null characters
//
void OttoSerialCommand::clearBuffer()
{
	for (int i=0; i<SERIALCOMMANDBUFFER; i++) 
	{
		buffer[i]='\0';
	}
	bufPos=0; 
}

// Retrieve the next token ("word" or "argument") from the Command buffer.  
// returns a NULL if no more tokens exist.   
char *OttoSerialCommand::next() 
{
	char *nextToken;
	nextToken = strtok_r(NULL, delim, &last); 
	return nextToken; 
}


// Stream functions 

// begin for bluetooth (ESP32)

bool OttoSerialCommand::begin(String bluetoothName)
{
	bool results = false;
	
#if defined(ESP32)
	if (bluetoothName.length())
	{
		// seems we were passed a name, print it out
		Serial.print("We want to start bluetooth ");
		Serial.print(bluetoothName);
		Serial.println("");
		
		// start the bluetooth SPP
		results = SerialBT.begin(bluetoothName);
		usingBluetooth = true;
	}
	else
#endif
	{
		usingBluetooth = false;
	}

	return results;
}

// begin for a real serial port with a baud rate
void OttoSerialCommand::begin(int baud)
{
	Serial.begin(baud);
	usingBluetooth = false;
}

bool OttoSerialCommand::isBluetooth(void)
{
	if (usingBluetooth)
	{
#if defined(ESP32)
		if (SerialBT.hasClient())
		{
			return true;
		}
		else
		{
			return false;
		}
#else
		return false;
#endif
	}
	return false;
}

int OttoSerialCommand::available()
{
#if defined(ESP32)
	if (isBluetooth())
	{
		return SerialBT.available();
	}
	else
#endif
	return Serial.available();
}

int OttoSerialCommand::read()
{
#if defined(ESP32)
	if (isBluetooth())
	{
		return SerialBT.read();
	}
	else
#endif
	return Serial.read();
}

int OttoSerialCommand::peek()
{
#if defined(ESP32)
	if (isBluetooth())
	{
		return SerialBT.peek();
	}
	else
#endif
	return Serial.peek();
}

size_t OttoSerialCommand::write(uint8_t c)
{
#if defined(ESP32)
	if (isBluetooth())
	{
		return SerialBT.write(c);
	}
	else
#endif
	return Serial.write(c);
}

size_t OttoSerialCommand::write(const uint8_t *buffer, size_t size)
{
#if defined(ESP32)
	if (isBluetooth())
	{
		return SerialBT.write(buffer, size);
	}
	else
#endif
	return Serial.write(buffer, size);
}

void OttoSerialCommand::flush()
{
#if defined(ESP32)
	if (isBluetooth())
	{
		// currently there is a bug in SerialBT.flush, it works without it
		//SerialBT.flush();
	}
	else
#endif
	Serial.flush();
}

// This checks the Serial stream for characters, and assembles them into a buffer.  
// When the terminator character (default '\r') is seen, it starts parsing the 
// buffer for a prefix command, and calls handlers setup by addCommand() member
void OttoSerialCommand::readSerial() 
{
	bool onlyOneCommand = true;
	
	// If we're using the Hardware port, check it.   Otherwise check the user-created OttoSoftwareSerial Port
	while ((available() > 0)&&(onlyOneCommand==true))
	{
		int i; 
		boolean matched; 
		
		inChar=read();   // Read single available character, there may be more waiting
		
		if (inChar==term) {     // Check for the terminator (default '\r') meaning end of command

			onlyOneCommand=false; //
			
			bufPos=0;           // Reset to start of buffer
			token = strtok_r(buffer,delim,&last);   // Search for command at start of buffer
			if (token == NULL) return; 
			matched=false; 
			for (i=0; i<numCommand; i++) {
				
				// Compare the found command against the list of known commands for a match
				if (strncmp(token,CommandList[i].command,SERIALCOMMANDBUFFER) == 0) 
				{
					// Execute the stored handler function for the command
					(*CommandList[i].function)(); 
					clearBuffer(); 
					matched=true; 
					break; 
				}
			}
			if (matched==false) {
				(*defaultHandler)(); 
				clearBuffer(); 
			}

		}
		if (isprint(inChar))   // Only printable characters into the buffer
		{
			buffer[bufPos++]=inChar;   // Put character into buffer
			buffer[bufPos]='\0';  // Null terminate
			if (bufPos > SERIALCOMMANDBUFFER-1) bufPos=0; // wrap buffer around if full  
		}
	}
}

// Adds a "command" and a handler function to the list of available commands.  
// This is used for matching a found token in the buffer, and gives the pointer
// to the handler function to deal with it. 
void OttoSerialCommand::addCommand(const char *command, void (*function)())
{
	if (numCommand < MAXSERIALCOMMANDS) {
				
		strncpy(CommandList[numCommand].command,command,SERIALCOMMANDBUFFER); 
		CommandList[numCommand].function = function; 
		numCommand++; 
	} 
}

// This sets up a handler to be called in the event that the receveived command string
// isn't in the list of things with handlers.
void OttoSerialCommand::addDefaultHandler(void (*function)())
{
	defaultHandler = function;
}
