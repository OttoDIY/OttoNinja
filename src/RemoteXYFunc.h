// RemoteXYFunc.h
// This header file contains a set of utility functions for converting
// integer values to strings and extracting MAC address from a string.

#ifndef RemoteXYFunc_h
#define RemoteXYFunc_h  // Include guard to prevent multiple inclusions

// Include inttypes.h for fixed-width integer types
#include <inttypes.h>

// Convert an unsigned 16-bit integer to a string
// Input: i - the integer to convert, s - a character array to store the string
// Output: A pointer to the end of the character array
char* rxy_itos (uint16_t i, char* s) {
  // Initialize local variables
  uint8_t len=0;
  char *p=s+5; // Start from the end of the character array

  // Convert the integer to a string
  while (i) {
    *p--=i%10+'0'; // Extract the least significant digit and convert it to ASCII
    i/=10; // Remove the least significant digit
    len++; // Increment the length of the string
  }

  // Copy the digits to the beginning of the character array and null-terminate the string
  for (i=0; i<len; i++) *s++=*(++p);
  *s=0;

  // Return a pointer to the end of the character array
  return s;
}

// Convert a hexadecimal digit character to an integer
uint8_t rxy_xctoi (char c) {
  uint8_t b;

  // Convert the character to an integer based on its ASCII value
  if (c<='9') b=c-'0';
  else if (c<='f') b=c-'A'+10;
  else b=c-'a'+10;

  // Return the lower 4 bits of the integer
  return b&0x0f;
}

// Extract a MAC address from a string
// Input: s - a character array containing the MAC address, m - a pointer to a uint8_t array to store the MAC address
void rxy_getMacAddr (const char* s, uint8_t* m) {
  uint8_t i, b;

  // Extract the MAC address from the input string and store it in the output array
  for (i=6; i>0; i--) {
    b=rxy_xctoi (*s++)<<4; // Extract the first hexadecimal digit and shift it to the upper 4 bits
    b|=rxy_xctoi (*s++); // Extract the second hexadecimal digit and add it to the lower 4 bits
    *m++=b; // Store the MAC address byte
    s++; // Move to the next byte in the input string
  }
}



#endif //RemoteXYFunc_h

