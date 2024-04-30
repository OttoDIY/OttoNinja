// RemoteXYFunc.h
// This header file contains a set of utility functions for converting
// integer values to strings and extracting MAC address from a string.

#ifndef RemoteXYFunc_h
#define RemoteXYFunc_h

// Include inttypes.h for fixed-width integer types
#include <inttypes.h> 

// Convert an unsigned 16-bit integer to a string
// Input: i - the integer to convert, s - a character array to store the string
// Output: A pointer to the end of the character array
char* rxy_itos (uint16_t i, char* s) {
  uint8_t len=0;
  char *p=s+5; // Start from the end of the character array
  while (i) {
    *p--=i%10+'0'; // Extract the least significant digit and convert it to ASCII
    i/=10; // Remove the least significant digit
    len++; // Increment the length of the string
  }
  for (i=0; i<len; i++) *s++=*(++p); // Copy the digits to the beginning of the character array
  *s=0; // Null-terminate the string
  return s; // Return a pointer to the end of the character array
}

// Convert a hexadecimal digit character to an integer
uint8_t rxy_xctoi (char c) {
  uint8_t b;
  if (c<='9') b=c-'0'; // If the character is a digit, convert it to an integer
  else if (c<='f') b=c-'A'+10; // If the character is an uppercase letter, convert it to an integer
  else b=c-'a'+10; // If the character is a lowercase letter, convert it to an integer
  return b&0x0f; // Return the lower 4 bits of the integer
}

// Extract a MAC address from a string
// Input: s - a character array containing the MAC address, m - a pointer to a uint8\_t array to store the MAC address
void rxy_getMacAddr (const char* s, uint8_t* m) {
  uint8_t i, b;
  for (i=6; i>0; i--) {
    b=rxy_xctoi (*s++)<<4; // Extract the first hexadecimal digit and shift it to the upper 4 bits
    b|=rxy_xctoi (*s++); // Extract the second hexadecimal digit and add it to the lower 4 bits
    *m++=b; // Store the MAC address byte
    s++; // Move to the next byte in the input string
  }    
}



#endif //RemoteXYFunc_h
