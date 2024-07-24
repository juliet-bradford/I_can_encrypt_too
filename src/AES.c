#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* number of columns comprising the state */
unsigned int Nb = 4;


/* the round constant word array */
uint32_t Rcon[] = { 
	0x00000000, 0x01000000, 0x02000000, 0x04000000, 
	0x08000000, 0x10000000, 0x20000000, 0x40000000, 
	0x80000000, 0x1B000000, 0x36000000, 0x6C000000, 
	0xD8000000, 0xAB000000, 0x4D000000, 0x9A000000, 
	0x2F000000, 0x5E000000, 0xBC000000, 0x63000000, 
	0xC6000000, 0x97000000, 0x35000000, 0x6A000000, 
	0xD4000000, 0xB3000000, 0x7D000000, 0xFA000000, 
	0xEF000000, 0xC5000000, 0x91000000, 0x39000000, 
	0x72000000, 0xE4000000, 0xD3000000, 0xBD000000, 
	0x61000000, 0xC2000000, 0x9F000000, 0x25000000, 
	0x4A000000, 0x94000000, 0x33000000, 0x66000000,
    0xCC000000, 0x83000000, 0x1D000000, 0x3A000000, 
	0x74000000, 0xE8000000, 0xCB000000, 0x8D000000
};


/* mapping array used by subBytes
* The specifics of the construction can be found in 
* FIPS 197 Advanced Encryption Standard Publication Section 5.1.1 
*/
uint8_t Sbox[16][16] = {
	{ 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76 },
	{ 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0 },
	{ 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15 },
	{ 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75 },
	{ 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84 },
	{ 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf },
	{ 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8 },
	{ 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2 },
	{ 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73 },
	{ 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb },
	{ 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79 },
	{ 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08 },
	{ 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a },
	{ 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e },
	{ 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf },
	{ 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }
};


/* mapping array used by invSubBytes
* The specifics of the construction can be found in 
* FIPS 197 Advanced Encryption Standard Publication Section 5.3.2 
*/
uint8_t InvSbox[16][16] = {
	{ 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb } ,
	{ 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb } ,
	{ 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e } ,
	{ 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25 } ,
	{ 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92 } ,
	{ 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84 } ,
	{ 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06 } ,
	{ 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b } ,
	{ 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73 } ,
	{ 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e } ,
	{ 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b } ,
	{ 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4 } ,
	{ 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f } ,
	{ 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef } ,
	{ 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61 } ,
	{ 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d }
};



/**
* @name ffAdd
* @brief finite field addition of 2 elements in GF(2^8).
* @param[in] x a 1 byte integer, interpreted as finite field polynomial.
* @param[in] y a 1 byte integer, interpreted as finite field polynomial.
* @return the two integers added together. 
* @note See FIPS 197 Advanced Encryption Standard Publication Section 4.1
*/
uint8_t ffAdd (uint8_t x, uint8_t y) {
	return x ^ y;
}


/**
* @name xtime
* @brief apply multiplication of the finite field polynomial b(x) by x modulo m(x)
* @param[in] x the finite field polynomial b(x)
* @return the polynomial, b(x) multiplied by x mod m(x)
* @note See FIPS 197 Advanced Encryption Standard Publication Section 4.2.1
*/
uint8_t xtime (uint8_t x) {

	/* note that 0x1b corresponds to the irreducible polynomial
	* specified in the AES standard: m(x) = x^8 + x^4 + x^3 + x + 1
	*
	* multiplication of the polynomial b(x) by x is equivalent to a left
	* shift by 1 and then subtraction by m(x) if the resulting polynomial 
	* is larger than m(x)
	*
	* which is applied if and only if x^8 is present in the resulting
	* polynomial
	*/
	
	if (x & 128) return (x << 1) ^ 0x1b; 
	else return x << 1;
}


/**
* @name ffMultiply
* @brief fast multiplication algorithm using xtime.
* @param[in] a the finite field polynomial a(x).
* @param[in] b the finite field polynomial b(x).
* @return the product of a and b
* @note See FIPS 197 Advanced Encryption Standard Publication Section 4.2.1
*/
uint8_t ffMultiply (uint8_t a, uint8_t b) {
	
	/* multiplication is applied bit wise, meaning b is multiplied 
	* by x, and if that bit is flipped in a then the resulting
	* product is added to our sum 
	*/

	uint8_t sum = 0;
	for (int i=0; i<8; ++i) {
		if (a & (1 << i))
			sum = ffAdd(sum, b);

		b = xtime(b);
	}

	return sum;
}


/**
* @name subByte
* @brief maps an element to another in the substitution table .
* @param[in] a 1 byte element. unaltered.
* @return the mapped element from Sbox.
*/
uint8_t subByte (uint8_t a) {
	
	// for i,j in (0,1,...,e,f)
	// a = 0xij -> Sbox[i][j]

	int i = (a & 0xf0) >> 4;
	int j = a & 0x0f;

	return Sbox[i][j];
}


/**
* @name invSubByte
* @brief inverse operation of subByte
* @param[in] a 1 byte element
* @return the mapped element from InvSbox
*/
uint8_t invSubByte (uint8_t a) {
	
	int i = (a & 0xf0) >> 4;
	int j = a & 0x0f;

	return InvSbox[i][j];
}


/**
* @name subBytes
* @brief wrapper that applies the subByte operation to each element of state.
* @param[out] state block of 16 bytes, mapped by subBytes.
* @return no return. parameter state is altered.
* @note See FIPS 197 Advanced Encryption Standard Publication Section 5.1.1
*/
void subBytes (uint8_t state[4][4]) {
	
	for (int i=0; i<4; ++i)
		for (int j=0; j<4; ++j)
			state[i][j] = subByte(state[i][j]);
}


/**
* @name invSubBytes
* @brief cooresponding wrapper for invSubByte.
* @param[out] state block of 16 bytes, mapped by subBytes.
* @return no return. parameter state is altered.
* @note See FIPS 197 Advanced Encryption Standard Publication Section 5.3.2
*/
void invSubBytes (uint8_t state[4][4]) {
	
	for (int i=0; i<4; ++i)
		for (int j=0; j<4; ++j)
			state[i][j] = invSubByte(state[i][j]);
}


/**
* @name subWord
* @brief applies subByte to each byte of word w.
* @param[in] w 4 byte word.
* @return subByted word returned.
*/
uint32_t subWord (uint32_t w) {
	
	uint32_t tempw = 0;
	for (int i=0; i<4; ++i) {
		uint8_t temp = (w >> (8*i)) & 0xff;
		temp = subByte(temp);
		tempw |= (temp << (8*i)) & (0xff << (8*i));
	}

	return tempw;
}


/**
* @name rotWord
* @brief rotates word w to the left by 1 byte.
* @param[in] w 4 byte word.
* @return the rotated word.
*/
uint32_t rotWord (uint32_t w) {

	// [a3,a2,a1,a0] -> [a2,a1,a0,a3]

	return (w << 8) | ((w >> 24) & 0xff);
}


/**
* @name shiftRows
* @brief cyclically rotates the rows of state.
* @param[out] state 16 byte block.
* @return no return. parameter state is altered.
* @note See FIPS 197 Advanced Encryption Standard Publication Section 5.1.2
*/
void shiftRows (uint8_t state[4][4]) {

	// Each row is rotated by i bytes, where i is which row it is
	
	for (int i=1; i<4; ++i) {
		uint32_t temp = (state[i][0] << 24) | ((state[i][1] << 16) & 0xff0000) |
			((state[i][2] << 8) & 0xff00) | ((state[i][3]) & 0xff);
		for (int j=0; j<i; ++j)
			temp = rotWord(temp);
		for (int j=0; j<4; ++j)
			state[i][j] = (temp >> (8 * (3-j))) & 0xff;
	}
}


/**
* @name invShiftRows
* @brief reverses the effect of shiftRows.
* @param[out] state 16 byte block.
* @return no return. parameter state is altered.
* @note See FIPS 197 Advanced Encryption Standard Publication Section 5.3.1
*/
void invShiftRows (uint8_t state[4][4]) {
	
	for (int i=1; i<4; ++i) {
		uint32_t temp = (state[i][0] << 24) | ((state[i][1] << 16) & 0xff0000) |
			((state[i][2] << 8) & 0xff00) | ((state[i][3]) & 0xff);
		for (int j=4-i; j>0; --j)
			temp = rotWord(temp);
		for (int j=0; j<4; ++j)
			state[i][j] = (temp >> (8 * (3-j))) & 0xff;
	}	
}


/**
* @name mixColumns
* @brief multiplication by a(x) = 3x^3 + x^2 + x + 2 modulo x^4 + 1 applied to each colomn in state.
* @param[out] state 16 byte block.
* @return no return. parameter state is altered.
* @note See FIPS 197 Advanced Encryption Standard Publication Section 5.1.3
*/
void mixColumns (uint8_t state[4][4]) {
	
	for (int j=0; j<4; ++j) {
		
		uint8_t temp[4];
		for (int i=0; i<4; ++i)
			temp[i] = state[i][j];

		for (int i=0; i<4; ++i)
			state[i][j] = ffMultiply(temp[i%4], 0x02) ^ ffMultiply(temp[(i+1)%4], 0x03) ^
				temp[(i+2)%4] ^ temp[(i+3)%4];
	}
}


/**
* @name invMixColumns
* @brief reverses the effect of mixColumns.
* @param[out] state 16 byte block.
* @return no return. parameter state is altered.
* @note See FIPS 197 Advanced Encryption Standard Publication Section 5.3.3
*/
void invMixColumns (uint8_t state[4][4]) {

	for (int j=0; j<4; ++j) {
		
		uint8_t temp[4];
		for (int i=0; i<4; ++i)
			temp[i] = state[i][j];

		for (int i=0; i<4; ++i)
			state[i][j] = ffMultiply(temp[i%4], 0x0e) ^ ffMultiply(temp[(i+1)%4], 0x0b) ^
				ffMultiply(temp[(i+2)%4], 0x0d) ^ ffMultiply(temp[(i+3)%4], 0x09);
	}
}


/**
* @name addRoundKey
* @brief adds round key to the state from the key schedule.
* @param[in] w key schedule, represented by word array.
* @param[in] round the current round of the cipher transformation.
* @param[out] state 16 byte block.
* @return no return. parameter state is altered.
* @note See FIPS 197 Advanced Encryption Standard Publication Section 5.1.4
*/
void addRoundKey(uint8_t state[4][4], uint32_t *w, unsigned int round) {

	for (int i=round; i<round+4; ++i) {
		for (int j=0; j<4; ++j) {
			
			state[j][i%4] ^= (w[i] >> (8*(3-j))) & 0xff;
		}
	}
}


/**
* @name keyExpansion
* @brief using the key, a key schedule is generated for use in the cipher.
* @param[in] key array of Nk words containing key.
* @param[in] Nk length of key in words.
* @param[out] w key schedule of Nb(Nr+1) words generated using key
* @return no return. parameter w is altered.
* @note See FIPS 197 Advanced Encryption Standard Publication Section 5.2 
*/
void keyExpansion (uint8_t *key, uint32_t *w, unsigned int Nk) {

	unsigned int Nr;
	if (Nk == 4) Nr = 10;
	else if (Nk == 6) Nr = 12;
	else Nr = 14;


	uint32_t temp;

	for (int i=0; i<Nk; ++i)
		w[i] = (key[4*i] << 24) | ((key[4*i+1] << 16) & 0xff0000) |
			((key[4*i+2] << 8) & 0xff00) | ((key[4*i+3]) & 0xff);

	for (int i=Nk; i<4*(Nr+1); ++i) {
		
		temp = w[i-1];

		if (i%Nk == 0) {
			temp = subWord(rotWord(temp)) ^ Rcon[i/Nk];
		} else if (Nk > 6 && i%Nk == 4) {
			temp = subWord(temp);
		}

		w[i] = w[i-Nk] ^ temp;
	}
}


/**
* @name fprintround
* @brief prints information about the current step of the current round of the cipher.
* @param[in] fout FILE pointer to output file.
* @param[in] state 16 byte block.
* @param[in] round unsigned int, current round of cipher.
* @param[in] name string containing the abbreviated step of this round.
* @return no return.
*/
void fprintround(FILE *fout, uint8_t state[4][4], unsigned int round, char *name) {
	fprintf(fout, "round[%2d].%-10s", round, name);

	for (int i=0; i<4; ++i)
		for (int j=0; j<4; ++j)
			fprintf(fout, "%.2x", state[j][i] & 0xff);
	fprintf(fout, "\n");
}


/**
* @name cipher
* @brief function that alters the block according to the AES cipher.
* @param[in] w key schedule sized to Nb(Nr+1).
* @param[in] Nr number of rounds in cipher.
* @param[in] verbose boolean, if true step by step output is printed.
* @param[in] fout FILE pointer that output is directed to if verbose.
* @param[out] state the 16 byte block.
* @return no return. parameter state is altered.
* @note See FIPS 197 Advanced Encryption Standard Publication Section 5.1
*/
void cipher (uint8_t state[4][4], uint32_t *w, unsigned int Nr, bool verbose, FILE *fout) {
	
	unsigned int round = 0;
	
	if (verbose) {
		fprintf(fout, "CIPHER (ENCRYPT):\n");
		fprintround(fout, state, round, "input");
	}

	addRoundKey(state, w, round);

	if (verbose) {
		fprintf(fout, "round[%2d].k_sch     ", round);
		for (int i=4*round; i<(4*round)+4; ++i)
			fprintf(fout, "%.8x", w[i]);
		fprintf(fout, "\n");
	}

	for (round=1; round<Nr; ++round) {
		
		if (verbose)
			fprintround(fout, state, round, "start");
		
		subBytes(state);

		if (verbose)
			fprintround(fout, state, round, "s_box");

		shiftRows(state);

		if (verbose)
			fprintround(fout, state, round, "s_row");
		

		mixColumns(state);

		if (verbose)
			fprintround(fout, state, round, "m_col");

		addRoundKey(state, w, 4*round);

		if (verbose) {
			fprintf(fout, "round[%2d].k_sch     ", round);
			for (int i=4*round; i<(4*round)+4; ++i)
				fprintf(fout, "%.8x", w[i]);
			fprintf(fout, "\n");
		}
	}

	if (verbose)
		fprintround(fout, state, round, "start");

	subBytes(state);

	if (verbose)
		fprintround(fout, state, round, "s_box");

	shiftRows(state);
		
	if (verbose)
		fprintround(fout, state, round, "s_row");
	
	addRoundKey(state, w, 4*round);
		
	if (verbose) {
		fprintf(fout, "round[%2d].k_sch     ", round);
		for (int i=4*round; i<(4*round)+4; ++i)
			fprintf(fout, "%.8x", w[i]);
		fprintf(fout, "\n");
	}

	if (verbose) 
		fprintround(fout, state, round, "output");

}


/**
* @name invCipher
* @brief function that alters the block according to the AES inverse cipher.
* @param[in] w key schedule sized to Nb(Nr+1).
* @param[in] Nr number of rounds in cipher.
* @param[in] verbose boolean, if true step by step output is printed.
* @param[in] fout FILE pointer that output is directed to if verbose.
* @param[out] state the 16 byte block.
* @return no return. parameter state is altered.
* @note See FIPS 197 Advanced Encryption Standard Publication Section 5.3
*/
void invCipher (uint8_t state[4][4], uint32_t *w, unsigned int Nr, bool verbose, FILE *fout) {

	int round = Nr;
	
	if (verbose) {
		fprintf(fout, "INVERSE CIPHER (DECRYPT):\n");
		fprintround(fout, state, Nr-round, "iinput");
	}

	addRoundKey(state, w, 4*round);

	if (verbose) {	
		fprintf(fout, "round[%2d].ik_sch    ", Nr-round);
		for (int i=4*round; i<(4*round)+4; ++i)
			fprintf(fout, "%.8x", w[i]);
		fprintf(fout, "\n");
	}

	for (round=Nr-1; round>0; --round) {

		if (verbose)
			fprintround(fout, state, Nr-round, "istart");

		
		invShiftRows(state);

		if (verbose)
			fprintround(fout, state, Nr-round, "is_row");


		invSubBytes(state);

		if (verbose)
			fprintround(fout, state, Nr-round, "is_box");


		addRoundKey(state, w, 4*round);

		if (verbose) {
			fprintf(fout, "round[%2d].ik_sch    ", Nr-round);
			for (int i=4*round; i<(4*round)+4; ++i)
				fprintf(fout, "%.8x", w[i]);
			fprintf(fout, "\n");
			
		}

		if (verbose)
			fprintround(fout, state, Nr-round, "ik_add"); 

		invMixColumns(state);
	}

	if (verbose)
		fprintround(fout, state, Nr-round, "istart");

	invShiftRows(state);

	if (verbose)
		fprintround(fout, state, Nr-round, "is_row");

	invSubBytes(state);

	if (verbose)
		fprintround(fout, state, Nr-round, "is_box");

	addRoundKey(state, w, 0);

	if (verbose) {
		fprintf(fout, "round[%2d].ik_sch    ", Nr-round);
		for (int i=4*round; i<(4*round)+4; ++i)
			fprintf(fout, "%.8x", w[i]);
		fprintf(fout, "\n");
	}

	if (verbose) 
		fprintround(fout, state, Nr-round, "ioutput");

}

/**
* @name getNk
* @brief function that returns the key length based on the encryption standard
* @param[in] standard encryption standard as string.
* @return unsigned int. length of key in words.
*/
unsigned int getNk(char standard[8]) {

	if (strcmp(standard, "aes-128") == 0) 
		return 4;
	else if (strcmp(standard, "aes-192") == 0)
		return 6;
	else if (strcmp(standard, "aes-256") == 0)
		return 8;
	else {
		fprintf(stderr, "ERROR: Encryption standard not identified `%s`.\nUse argument: aes[128|192|256]\n", standard);
		exit(EXIT_FAILURE);
	}
}

/**
* @name encryptBlock
* @brief wrapper for encryption cipher and key expansion.
* @param[in] key uint32_t[Nk]. cipher key.
* @param[in] Nk length of key according to encryption specification.
* @param[in] verbose boolean for detailed output.
* @param[in] fout file that detailed output is directed to.
* @param[out] state 16 byte block.
* @return no return. parameter state is altered.
*/
void encryptBlock(uint8_t state[4][4], uint8_t *key, unsigned int Nk, bool verbose, FILE *fout) {

	unsigned int Nr = Nk + 6;

	uint32_t ekey[Nb*(Nr+1)];
	keyExpansion(key, ekey, Nk);
	cipher(state, ekey, Nr, verbose, fout);
}

/**
* @name decryptBlock
* @brief wrapper for decryption cipher and key expansion.
* @param[in] key uint32_t[Nk]. cipher key.
* @param[in] Nk length of key according to encryption specification.
* @param[in] verbose boolean for detailed output.
* @param[in] fout file that detailed output is directed to.
* @param[out] state 16 byte block.
* @return no return. parameter state is altered.
*/
void decryptBlock(uint8_t state[4][4], uint8_t *key, unsigned int Nk, bool verbose, FILE *fout) {

	unsigned int Nr = Nk + 6;

	uint32_t ekey[Nb*(Nr+1)];
	keyExpansion(key, ekey, Nk);
	invCipher(state, ekey, Nr, verbose, fout);
}



int main (unsigned int argc, char** argv) {

	/* A bunch of command line parsing garbage */

	if (argc == 1) {
		fprintf(stderr, "ERROR: Improper number of arguments.\nTry: ./AES --help\n");
		exit(EXIT_FAILURE);
	}
	else if (strcmp(argv[1], "--help") == 0) {
		fprintf(stdout, 
		"Welcome to (an unofficial replication of) AES: the Advanced Encryption Standard (also known as Rijndael).\n" 
		"This is meant purely for educational purposes and should absolutely not be used on anything that matters.\n\n" 
		
		"USAGE INFORMATION: ./AES -s aes-[128|192|256] [-d] [-fin filename] [-fkey filename] [-fout filename] [-verbose]\n\n" 
		
		"Encryption Standard (-s aes-[128|192|256]): Determines what encryption standard is used. Must be used with key of cooresponding size.\n\n" 
		
		"Decrypt or Encrypt ([-d]): Determines if the input block is to be encrypted or decrypted. If not present encryption is used.\n\n" 
		
		"Input File ([-fin filename]): An optional file containing the input block. If none is given stdin is used.\n\n" 
		
		"Key File ([-fkey filename]): An optional file containing the key. If none is given stdin is used.\n\n" 
		
		"Output File ([-fout filename]): An optional file for the output block. If none is given then stdout is used.\n\n" 
		
		"Verbose [-verbose]: This option changes the output from the altered block in binary to the hex values of the block as\n" 
		"	the block is encrypted/decrypted. The meaning of this output can be garnered from reading Appendix C of the\n" 
		"	FIPS 197 Advanced Encryption Standard Publication.\n\n\n"


		"EXAMPLES INFORMATION:\n"
		"Examples taken from the afformentioned Appendix C have been lovingly added to the examples/verbose_examples file.\n"
		"	Additionally, the plain text block used in those examples, and the corresponding encrypted blocks, can all be\n"
		"	found in examples/plaintext. Finally, the keys used in Appendix C to encrypt the original block can be found in\n" 
		"	examples/keys.\n\n\n"
		
		"EXAMPLE USAGE:\n"
		"If you wanted to, for example, encrypt a file called plain.txt using a 128 byte keyfile named k128.key using the AES-128\n"
		"	encryption standard and output that encrypted block to a file named plain128.txt.enc as binary, then you could use the\n"
		"	following command:\n\n"
		
		"	./AES -s aes-128 -fin plain.txt -fkey k128.key -fout plain128.txt.enc\n\n\n"
		
		"And to decrypt that block into stdout and redirect the binary into a file one could use:\n\n"
		
		"	./AES -s aes-128 -d -fin plain128.txt.enc -fkey k128.key > decrypted_block\n");

		return 0;
	}
	else if (argc == 2) {
		fprintf(stderr, "ERROR: Improper number of arguments.\nTry: ./AES --help\n");
		exit(EXIT_FAILURE);
	}

	bool verbose = false, encrypt = true;
	char standard[8] = "", *finName = NULL, *fkeyName = NULL, *foutName = NULL;
	for (unsigned int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-s") == 0) {
			if (i+1 < argc && strlen(argv[++i]) == 7) strncpy(standard, argv[i], 7);
			else {
				fprintf(stderr, "ERROR: Encryption standard not identified.\nUse argument: -s aes-[128|192|256]\n");
				exit(EXIT_FAILURE);
			}
		}
		else if (strcmp(argv[i], "-d") == 0) {
			encrypt = false;
		}
		else if (strcmp(argv[i], "-fin") == 0) {
			if (i+1 < argc && argv[++i][0] != '-') {
				finName = malloc((strlen(argv[i])+1) * sizeof(char));
				strncpy(finName, argv[i], strlen(argv[i]));
			}  
			else {
				fprintf(stderr, "ERROR: Input file name not found.\n");
				exit(EXIT_FAILURE);
			}
		}
		else if (strcmp(argv[i], "-fkey") == 0) {
			if (i+1 < argc && argv[++i][0] != '-') {
				fkeyName = malloc((strlen(argv[i])+1) * sizeof(char));
				strncpy(fkeyName, argv[i], strlen(argv[i]));
			}  
			else {
				fprintf(stderr, "ERROR: Key file name not found.\n");
				exit(EXIT_FAILURE);
			}
		}
		else if (strcmp(argv[i], "-fout") == 0) {
			if (i+1 < argc && argv[++i][0] != '-') {
				foutName = malloc((strlen(argv[i])+1) * sizeof(char));
				strncpy(foutName, argv[i], strlen(argv[i]));
			}  
			else {
				fprintf(stderr, "ERROR: Output file name not found.\n");
				exit(EXIT_FAILURE);
			}
		}
		else if (strcmp(argv[i], "-verbose") == 0) verbose = true;
		else {
			fprintf(stderr, "ERROR: Unrecognized command line argument `%s`\n", argv[i]);
			exit(EXIT_FAILURE);
		}
	}
	if (strcmp(standard, "") == 0) {
		fprintf(stderr, "ERROR: No Encryption standard given.\nTry ./AES --help\n");
		exit(EXIT_FAILURE);
	}

	
	/* reading input data from file into block, only the first 16 bytes are used. */
	/* if input is less than 16 bytes, we extend to 16 bytes with zeroes. */
	/* if no file is given we use standard input. */

	uint8_t in[Nb*4];

	FILE *fin;
	if (finName != NULL) {
		fin = fopen(finName, "r");
		if (fin == NULL) {
			fprintf(stderr, "ERROR: Input file not found\n");
			exit(EXIT_FAILURE);
		}

		unsigned int bytes_read = fread(in, sizeof(uint8_t), Nb*4, fin);
		if (bytes_read < Nb*4)
			for (unsigned int i = bytes_read; i < Nb*4; ++i) 
				in[i] = 0;

		if (fclose(fin) != 0) {
			fprintf(stderr, "ERROR: Unable to close input file.\n");
			exit(1);
		}
	}
	else {
		printf("No input file found.\nInput block as hex(16 bytes):\n");
		for (unsigned int i = 0; i < Nb*4; ++i) {
			char str[3];
			scanf("%s", str);
			in[i] = strtoul(str, NULL, 16);
		}
	}

	
	/* reading key data from file. length of key is Nk, found in AES specifications. */

	unsigned int Nk = getNk(standard);
	uint8_t key[Nk*4];

	if (fkeyName != NULL) {
		FILE *fkeyin = fopen(fkeyName, "r");
		if (fkeyin == NULL) {
			fprintf(stderr, "ERROR: Key file not found\n");
			exit(EXIT_FAILURE);
		}

		unsigned int bytes_read = fread(key, sizeof(uint8_t), Nk*4, fkeyin);
		if (bytes_read < Nk*4) {
			fprintf(stderr, "ERROR: Key file is not Nk bytes long.\n");
			exit(EXIT_FAILURE);
		}

		if (fclose(fkeyin) != 0) {
			fprintf(stderr, "ERROR: Unable to close key file.\n");
			exit(1);
		}
	}
	else {
		printf("No key file found.\nInput key as hex(%d bytes):\n", Nk*4);
		for (unsigned int i = 0; i < Nk*4; ++i) {
			char str[3];
			scanf("%s", str);
			key[i] = strtoul(str, NULL, 16);
		}
	}
	
	/* input is read into 4x4 block for conformity to cipher */

	uint8_t statein[4][4]; //, stateout[4][4];

	for (int i=0; i<4; ++i)
		for (int j=0; j<4; ++j) {
			statein[j][i] = in[(4*i)+j];
			//stateout[i][j] = 0x00;
		}


	/* defining output file, using stdout if no file given */

	FILE *fout;
	if (foutName != NULL) {
		fout = fopen(foutName, "w");
		if (fout == NULL) {
			fprintf(stderr, "ERROR: Output file unable to open.\n");
			exit(EXIT_FAILURE);
		}
	}
	else
		fout = stdout;


	if (verbose) {
		switch (Nk) {
			case 4:
				fprintf(fout, "AES-128 ");
				break;
			case 6:
				fprintf(fout, "AES-192 ");
				break;
			case 8:
				fprintf(fout, "AES-256 ");
				break;
			default:
				fprintf(stderr, "May god have mercy on your soul.\n");
				exit(1);
		}
		fprintf(fout, "(Nk=%d, Nr=%d)\n\n", Nk, Nk+6);
		fprintf(fout, "PLAINTEXT:          ");
		for (unsigned int i = 0; i < Nb*4; ++i)
			fprintf(fout, "%.2x",in[i]);
		fprintf(fout, "\n");
		fprintf(fout, "KEY:                ");
		for (unsigned int i = 0; i < Nk*4; ++i)
			fprintf(fout, "%.2x",key[i]);
		fprintf(fout, "\n\n");
	}

	
	/* encryption of decryption based on command line */

	if (encrypt) encryptBlock(statein, key, Nk, verbose, fout);
	else decryptBlock(statein, key, Nk, verbose, fout);

	
	/* makes fwrite work in one line */
	
	uint8_t out[Nb*4];
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			out[(i*4)+j] = statein[j][i];


	/* write binary to file and close file if needed */

	if (!verbose) fwrite(out, sizeof(uint8_t), Nb*4, fout);

	if (fout != stdout && fclose(fout) != 0) {
		fprintf(stderr, "ERROR: Unable to close output file.\n");
		exit(1);
	}
	
	return 0;

}
