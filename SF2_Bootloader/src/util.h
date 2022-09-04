#ifndef __UTIL_H__
#define __UTIL_H__

void delay_us(unsigned int time);
void delay_ms(unsigned int time);

int AsciiToHex(char c);
char HexStringToByte(short value );
short HexStringToShort(int value);

#endif /* __UTIL_H__ */
