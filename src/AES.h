#ifndef AES_H
#define AES_H

void encryptBlock (uint8_t[4][4], uint8_t[4][4], uint32_t *, unsigned int, bool, FILE *);
void decryptBlock (uint8_t[4][4], uint8_t[4][4], uint32_t *, unsigned int, bool, FILE *);

#endif