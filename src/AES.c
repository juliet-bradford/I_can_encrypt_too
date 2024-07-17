#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned int Nb = 4;

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


uint32_t expanded[44] = { 
	0x2b7e1516, 0x28aed2a6, 0xabf71588, 0x09cf4f3c,
	0xa0fafe17, 0x88542cb1, 0x23a33939, 0x2a6c7605,
	0xf2c295f2, 0x7a96b943, 0x5935807a, 0x7359f67f,
	0x3d80477d, 0x4716fe3e, 0x1e237e44, 0x6d7a883b,
	0xef44a541, 0xa8525b7f, 0xb671253b, 0xdb0bad00,
	0xd4d1c6f8, 0x7c839d87, 0xcaf2b8bc, 0x11f915bc,
	0x6d88a37a, 0x110b3efd, 0xdbf98641, 0xca0093fd,
	0x4e54f70e, 0x5f5fc9f3, 0x84a64fb2, 0x4ea6dc4f,
	0xead27321, 0xb58dbad2, 0x312bf560, 0x7f8d292f,
	0xac7766f3, 0x19fadc21, 0x28d12941, 0x575c006e,
	0xd014f9a8, 0xc9ee2589, 0xe13f0cc8, 0xb6630ca6 
};



void printstate(uint8_t state[4][4]) {
	
	for (int i=0; i<4; ++i)
		for (int j=0; j<4; ++j)
			printf("%.2x", state[j][i] & 0xff);
	printf("\n");
}

void printfinal(uint8_t state[4][4], uint8_t final[16]) {

	for (int i=0; i<4; ++i)
		for (int j=0; j<4; ++j)
			final[(4*i)+j] = state[j][i];

	for (int i=0; i<16; ++i)
		printf("%.2x", final[i] & 0xff);
	printf("\n");
}

uint8_t ffAdd (uint8_t x, uint8_t y) {
	return x ^ y;
}

uint8_t xtime (uint8_t x) {
	if (x & 128) return (x << 1) ^ 27;
	else return x << 1;
}

uint8_t ffMultiply (uint8_t a, uint8_t b) {
	
	uint8_t sum = 0;
	for (int i=0; i<8; ++i) {
		if (a & (1 << i))
			sum = ffAdd(sum, b);

		b = xtime(b);
	}

	return sum;
}

uint8_t subByte (uint8_t a) {
	
	int i = (a & 0xf0) >> 4;
	int j = a & 0x0f;

	return Sbox[i][j];
}

uint8_t invSubByte (uint8_t a) {
	
	int i = (a & 0xf0) >> 4;
	int j = a & 0x0f;

	return InvSbox[i][j];
}

void subBytes (uint8_t state[4][4]) {
	
	for (int i=0; i<4; ++i)
		for (int j=0; j<4; ++j)
			state[i][j] = subByte(state[i][j]);
}

void invSubBytes (uint8_t state[4][4]) {
	
	for (int i=0; i<4; ++i)
		for (int j=0; j<4; ++j)
			state[i][j] = invSubByte(state[i][j]);
}

uint32_t subWord (uint32_t w) {
	
	uint32_t tempw = 0;
	for (int i=0; i<4; ++i) {
		uint8_t temp = (w >> (8*i)) & 0xff;
		temp = subByte(temp);
		tempw |= (temp << (8*i)) & (0xff << (8*i));
	}

	return tempw;
}

uint32_t rotWord (uint32_t w) {

	return (w << 8) | ((w >> 24) & 0xff);
}

void shiftRows (uint8_t state[4][4]) {
	
	for (int i=1; i<4; ++i) {
		uint32_t temp = (state[i][0] << 24) | ((state[i][1] << 16) & 0xff0000) |
			((state[i][2] << 8) & 0xff00) | ((state[i][3]) & 0xff);
		for (int j=0; j<i; ++j)
			temp = rotWord(temp);
		for (int j=0; j<4; ++j)
			state[i][j] = (temp >> (8 * (3-j))) & 0xff;
	}
}

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


void addRoundKey(uint8_t state[4][4], uint32_t *w, unsigned int round) {

	for (int i=round; i<round+4; ++i) {
		for (int j=0; j<4; ++j) {
			
			state[j][i%4] ^= (w[i] >> (8*(3-j))) & 0xff;
		}
	}
}


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


void cipher (uint8_t in[4][4], uint8_t out[4][4], uint32_t *w, unsigned int Nr, bool verbose) {
	
	uint8_t state[4][4];
	for (int i=0; i<4; ++i)
		for (int j=0; j<4; ++j)
			state[i][j] = in[i][j];

	unsigned int round = 0;
	
	if (verbose) {
		printf("CIPHER (ENCRYPT):\n");
		printf("round[%2d].input     ", round);
		printstate(state);
	}

	addRoundKey(state, w, round);

	if (verbose) {
		printf("round[%2d].k_sch     ", round);
		for (int i=4*round; i<(4*round)+4; ++i)
			printf("%.8x", w[i]);
		printf("\n");
	}

	for (round=1; round<Nr; ++round) {
		
		if (verbose) {		
			printf("round[%2d].start     ", round);
			printstate(state);
		}
		
		subBytes(state);

		if (verbose) {
			printf("round[%2d].s_box     ", round);
			printstate(state);
		}

		shiftRows(state);

		if (verbose) {
			printf("round[%2d].s_row     ", round);
			printstate(state);
		}

		mixColumns(state);

		if (verbose) {
			printf("round[%2d].m_col     ", round);
			printstate(state);
		}

		addRoundKey(state, w, 4*round);

		if (verbose) {
			printf("round[%2d].k_sch     ", round);
			for (int i=4*round; i<(4*round)+4; ++i)
				printf("%.8x", w[i]);
			printf("\n");
		}
	}

	if (verbose) {	
		printf("round[%2d].start     ", round);
		printstate(state);
	}

	subBytes(state);

	if (verbose) {
		printf("round[%2d].s_box     ", round);
		printstate(state);
	}

	shiftRows(state);
		
	if (verbose) {
		printf("round[%2d].s_row     ", round);
		printstate(state);
	}
	
	addRoundKey(state, w, 4*round);
		
	if (verbose) {
		printf("round[%2d].k_sch     ", round);
		for (int i=4*round; i<(4*round)+4; ++i)
			printf("%.8x", w[i]);
		printf("\n");
	}

	for (int i=0; i<4; ++i)
		for (int j=0; j<4; ++j)
			out[i][j] = state[i][j];
}


void invCipher (uint8_t in[4][4], uint8_t out[4][4], uint32_t *w, unsigned int Nr, bool verbose) {
	
	uint8_t state[4][4];
	for (int i=0; i<4; ++i)
		for (int j=0; j<4; ++j)
			state[i][j] = in[i][j];

	int round = Nr;
	
	if (verbose) {
		printf("\nINVERSE CIPHER (DECRYPT):\n");
		printf("round[%2d].iinput    ", Nr-round);
		printstate(state);
	}

	addRoundKey(state, w, 4*round);

	if (verbose) {	
		printf("round[%2d].ik_sch    ", Nr-round);
		for (int i=4*round; i<(4*round)+4; ++i)
			printf("%.8x", w[i]);
		printf("\n");
	}

	for (round=Nr-1; round>0; --round) {

		if (verbose) {
			printf("round[%2d].istart    ", Nr-round);
			printstate(state);
		}

		invShiftRows(state);

		if (verbose) {
			printf("round[%2d].is_row    ", Nr-round);
			printstate(state);
		}

		invSubBytes(state);

		if (verbose) {
			printf("round[%2d].is_box    ", Nr-round);
			printstate(state);
		}

		addRoundKey(state, w, 4*round);

		if (verbose) {
			printf("round[%2d].ik_sch    ", Nr-round);
			for (int i=4*round; i<(4*round)+4; ++i)
				printf("%.8x", w[i]);
			printf("\n");
			
			printf("round[%2d].ik_add    ", Nr-round);
			printstate(state);
		}
		
		invMixColumns(state);
	}

	if (verbose) {
		printf("round[%2d].istart    ", Nr-round);
		printstate(state);
	}

	invShiftRows(state);

	if (verbose) {
		printf("round[%2d].is_row    ", Nr-round);
		printstate(state);
	}

	invSubBytes(state);

	if (verbose) {
		printf("round[%2d].is_box    ", Nr-round);
		printstate(state);
	}

	addRoundKey(state, w, 0);

	if (verbose) {
		printf("round[%2d].ik_sch    ", Nr-round);
		for (int i=4*round; i<(4*round)+4; ++i)
			printf("%.8x", w[i]);
		printf("\n");
	}

	for (int i=0; i<4; ++i)
		for (int j=0; j<4; ++j)
			out[i][j] = state[i][j];
}

void encryptBlock(uint8_t in[4][4], uint8_t out[4][4], uint8_t *key, char standard[7], bool verbose) {

	unsigned int Nk, Nr;
	if (strcmp(standard, "aes-128") == 0) 
		Nk = 4;
	else if (strcmp(standard, "aes-192") == 0)
		Nk = 6;
	else if (strcmp(standard, "aes-256") == 0)
		Nk = 8;
	else {
		;
	}
	Nr = Nk + 6;

	uint32_t ekey[Nb*(Nr+1)];
	keyExpansion(key, ekey, Nk);
	cipher(in, out, ekey, Nr, verbose);

}

void decryptBlock(uint8_t in[4][4], uint8_t out[4][4], uint8_t *key, char standard[7], bool verbose) {

	unsigned int Nk, Nr;
	if (strcmp(standard, "aes-128") == 0) 
		Nk = 4;
	else if (strcmp(standard, "aes-192") == 0)
		Nk = 6;
	else if (strcmp(standard, "aes-256") == 0)
		Nk = 8;
	else {
		;
	}
	Nr = Nk + 6;

	uint32_t ekey[Nb*(Nr+1)];
	keyExpansion(key, ekey, Nk);
	invCipher(out, in, ekey, Nk, verbose);
}

int main (int argc, char** argv) {

	bool verbose = false;
	char standard[7] = "aes-128";

	uint8_t in[16] = { 
		0x00, 0x11, 0x22, 0x33, 
		0x44, 0x55, 0x66, 0x77, 
		0x88, 0x99, 0xaa, 0xbb, 
		0xcc, 0xdd, 0xee, 0xff 
		};

    uint8_t out[16] = { 
		0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00 
		};
	
	uint8_t statein[4][4], stateout[4][4];

	for (int i=0; i<4; ++i)
		for (int j=0; j<4; ++j) {
			statein[j][i] = in[(4*i)+j];
			stateout[i][j] = 0x00;
		}
	

	
	printf("C.1   AES-128 (Nk=4, Nr=10)\n\n");
	printf("PLAINTEXT:          00112233445566778899aabbccddeeff\n");
	printf("KEY:                000102030405060708090a0b0c0d0e0f\n");
	printf("\n");

	uint8_t key128[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
		0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

	encryptBlock(statein, stateout, key128, standard, verbose);

	printf("round[10].output    ");
	printfinal(stateout, out);

	decryptBlock(stateout, statein, key128, standard, verbose);
	
	printf("round[10].ioutput   ");
	printfinal(statein, in);

	
	
	printf("\nC.2   AES-192 (Nk=6, Nr=12)\n\n");
	printf("PLAINTEXT:          00112233445566778899aabbccddeeff\n");
	printf("KEY:                000102030405060708090a0b0c0d0e0f1011121314151617\n");
	printf("\n");
	
	uint8_t key192[24] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 
		0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16 , 0x17 };

	strncpy(standard, "aes-192", 7);
	encryptBlock(statein, stateout, key192, standard, verbose);

	printf("round[12].output    ");
	printfinal(stateout, out);

	decryptBlock(stateout, statein, key192, standard, verbose);
	
	printf("round[12].ioutput   ");
	printfinal(statein, in);
	


	printf("\nC.3   AES-256 (Nk=8, Nr=14)\n\n");
	printf("PLAINTEXT:          00112233445566778899aabbccddeeff\n");
	printf("KEY:                000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f\n");
	printf("\n");

	uint8_t key256[32] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 
		0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 
		0x1b, 0x1c, 0x1d, 0x1e, 0x1f };

	strncpy(standard, "aes-256", 7);
	encryptBlock(statein, stateout, key256, standard, verbose);

	printf("round[14].output    ");
	printfinal(stateout, out);

	decryptBlock(stateout, statein, key256, standard, verbose);
	
	printf("round[14].ioutput   ");
	printfinal(statein, in);

	
	return 0;
}
