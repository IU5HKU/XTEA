// XTEA.cpp : Defines the entry point for the console application.
//

#pragma once

#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINNT 0x0600     // Change this to the appropriate value to target other versions of Windows.
#endif

#include <stdio.h>
#include <tchar.h>
#include <Shlwapi.h>

//this is necessary for using XTEA in 64bit systems
#include <cstdint>

//////////////////////////////////////////////////////////////////////////
/* take 64 bits of data in v[0] and v[1] and 128 bits of key[0] - key[3] */
/* numrounds usually is 64 but can be 32 as well						 */
_inline void encipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
	unsigned int i;
	uint32_t v0=v[0], v1=v[1], sum=0, delta=0x9E3779B9;
	for (i=0; i < num_rounds; i++) {
		v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
		sum += delta;
		v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
	}
	v[0]=v0; v[1]=v1;
}

//////////////////////////////////////////////////////////////////////////
_inline void decipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
	unsigned int i;
	uint32_t v0=v[0], v1=v[1], delta=0x9E3779B9, sum=delta*num_rounds;
	for (i=0; i < num_rounds ; i++) {
		v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
		sum -= delta;
		v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
	}
	v[0]=v0; v[1]=v1;
}

//////////////////////////////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{
	//128bit default key
	uint32_t key[4]={0xDC529BD3,0xDEADBEEF,0xC0FFEE33,0x29211663};
	uint32_t tmp;
	//default numrounds
	uint32_t numrounds=64;
	//default data to be encrypted/decrypted
	uint32_t lzma_data[2]={0x6C000080,0x0088546D};

	// the first bytes of the INETHDS12 maincode chunk
	// uint32_t encoded_data[2]={0x93FFFF80,0xFF801CA2};
	
	if(argc==5){
		StrToInt64Ex(argv[1],STIF_SUPPORT_HEX,(LONGLONG*)&key[0]);
		StrToInt64Ex(argv[2],STIF_SUPPORT_HEX,(LONGLONG*)&key[2]);
		StrToInt64Ex(argv[3],STIF_SUPPORT_HEX,(LONGLONG*)&lzma_data[0]);
		numrounds=StrToIntA(argv[4]);
		
		//adjust the key endianess
		tmp=key[1];
		key[1]=key[0];
		key[0]=tmp;
		tmp=key[2];
		key[2]=key[3];
		key[3]=tmp;

		//adjust data endianess
		tmp=lzma_data[0];
		lzma_data[0]=lzma_data[1];
		lzma_data[1]=tmp;
	}
	
	printf("------------------------------------------------------------------\n");
	printf("ORIGINAL DATA    : 0x%08X%08X\n",lzma_data[0],lzma_data[1]);
	printf("128bit KEY is    : 0x%08X%08X%08X%08X\n",key[0],key[1],key[2],key[3]);
	printf("------------------------------------------------------------------\n");

	encipher(numrounds,lzma_data,(uint32_t*)&key); 
	printf("CRYPT WITH XTEA  : 0x%08X%08X\n",lzma_data[0],lzma_data[1]);
	
	decipher(numrounds,lzma_data,(uint32_t*)&key);
	printf("DECRYPT WITH XTEA: 0x%08X%08X\n",lzma_data[0],lzma_data[1]);
	printf("DONE.\n");

	return 0;
}
