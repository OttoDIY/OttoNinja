#ifndef RemoteXYFunc_h
#define RemoteXYFunc_h

#include <inttypes.h> 


char* rxy_itos (uint16_t i, char* s) {
  uint8_t len=0;
  char *p=s+5;
  while (i) {
    *p--=i%10+'0';
    i/=10;
    len++;
  }
  for (i=0; i<len; i++) *s++=*(++p);
  *s=0;
  return s;  
}

uint8_t rxy_xctoi (char c) {
  uint8_t b;
  if (c<='9') b=c-'0';
  else if (c<='f') b=c-'A'+10; 
  else b=c-'a'+10;
  return b&0x0f;
}

void rxy_getMacAddr (const char* s, uint8_t* m) {
  uint8_t i, b;
  for (i=6; i>0; i--) {
    b=rxy_xctoi (*s++)<<4;
    b|=rxy_xctoi (*s++);
    *m++=b;
    s++;
  }    
}



#endif //RemoteXYFunc_h