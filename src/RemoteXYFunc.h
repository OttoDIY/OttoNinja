// RemoteXYFunc.h
// This header file contains a set of utility functions for converting
// integer values to strings and extracting MAC address from a string.

#ifndef RemoteXYFunc_h
#define RemoteXYFunc_h  // Include guard to prevent multiple inclusions

// Include standard headers
#include <inttypes.h>

// Convert an unsigned 16-bit integer to a string
// Input: i - the integer to convert, s - a character array to store the string
// Output: A pointer to the end of the character array
static char* rxy_itos(uint16_t i, char* s) {
  // Initialize local variables
  uint8_t len = 0;
  char* p = s + 5; // Start from the end of the character array

  // Convert the integer to a string
  while (i) {
    *p-- = i % 10 + '0'; // Extract the least significant digit and convert it to ASCII
    i /= 10; // Remove the least significant digit
    len++; // Increment the length of the string
  }

  // Copy the digits to the beginning of the character array and null-terminate the string
  for (uint8_t j = 0; j < len; j++) *s++ = *(++p);
  *s = 0;

  // Return a pointer to the end of the character array
  return s;
}

// Convert a hexadecimal digit character to an integer
uint8_t rxy_xctoi(char c) {
  uint8_t b;

  // Convert the character to an integer based on its ASCII value
  if (c <= '9') b = c - '0';
  else if (c <= 'f') b = c - 'A' + 10;
  else b = c - 'a' + 10;

  // Return the lower 4 bits of the integer
  return b & 0x0f;
}

// Extract a MAC address from a string
// Input: s - a character array containing the MAC address, m - a pointer to a uint8_t array to store the MAC address
// Output: true if the MAC address was extracted successfully, false otherwise
static bool rxy_getMacAddr(const char* s, uint8_t* m) {
  uint8_t i, b;

  // Check that the input string has the correct format
  if (strlen(s) != 17 || strspn(s, "0123456789ABCDEFabcdef:") != 17) {
    return false;
  }

  // Extract the MAC address from the input string and store it in the output array
  for (i = 6; i > 0; i--) {
    b = rxy_xctoi(*s++) << 4; // Extract the first hexadecimal digit and shift it to the upper 4 bits
    b |= rxy_xctoi(*s++); // Extract the second hexadecimal digit and add it to the lower 4 bits
    *m++ = b; // Store the MAC address byte
    s++; // Move to the next byte in the input string
  }

  // Return true if the MAC address was extracted successfully
  return true;
}

// Test function to verify the functionality of the rxy_itos and rxy_getMacAddr functions
void test_RemoteXYFunc() {
  uint16_t i = 12345;
  char s[6];
  rxy_itos(i, s);
  printf("rxy_itos(%u) = %s\n", i, s);

  const char* macStr = "00:11:22:33:44:55";
  uint8_t mac[6];
  if (rxy_getMacAddr(macStr, mac)) {
    printf("rxy_getMacAddr(%s) = {%02x, %02x, %02x, %02x, %02x, %02x}\n",
           macStr, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  } else {
    printf("rxy_getMacAddr(%s) failed\n", macStr);
  }
}

#endif //RemoteXYFunc_h
