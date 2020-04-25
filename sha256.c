#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define padding_length(s,n)(memset(s+63,n,sizeof(char)))


void sha256_words(unsigned char[],unsigned int[]);
void derive_words(unsigned int[]);
void compress_sha256(unsigned int []);
unsigned int rotate_right(unsigned int,int );


unsigned int hash[8]={
			0x6a09e667,
			0xbb67ae85,
			0x3c6ef372,
			0xa54ff53a,
			0x510e527f,
			0x9b05688c,
			0x1f83d9ab,
			0x5be0cd19
			
			}; //hashes


unsigned int k[64]={
   0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
   0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
   0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
   0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
   0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
   0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
   0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
   0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
		}; //constants



int main()
{
	unsigned char s[64]="abc"; //enter your string here
	unsigned int words[64];
	char final_hash[64];
	int i,length,j=0;
	
	length=strlen(s)*8;
	
	memset(s+strlen(s),0x80 ,sizeof(char)); //padding 1
	
	padding_length(s,length); //padding the length of the string

	
	sha256_words(s,words);
	derive_words(words);
	compress_sha256(words);
	
	
	for(i=0;i<8;i++) //converting final hash into a string
	{

		sprintf(&final_hash[j],"%x",hash[i]);
		j=j+8;
	}
	printf("%s\n",final_hash);

	
	
	return 0;
}
	
void sha256_words(unsigned char s[],unsigned int words[])
{
	int i,j=0;
	for(i=0;i<16;++i)
	{

		words[i]=(s[j]<<24) | (s[j+1]<<16) | (s[j+2]<<8) |(s[j+3]); //converting it from char to int 
		
		j=j+4;
	}

}
void derive_words(unsigned int words[])
{
	int i;
	unsigned int s0,s1;
	
	for(i=16;i<64;i++)
	{
		s0 = (rotate_right(words[i-15],7)) ^ (rotate_right(words[i-15],18)) ^ (words[i-15]>>3);
		
		s1 = (rotate_right(words[i-2],17)) ^ (rotate_right(words[i-2],19))  ^ (words[i-2]>>10);
		
		words[i] = words[i-16] + s0 + words[i-7] + s1;
	}
}
void compress_sha256(unsigned words[])
{
	unsigned int a,b,c,d,e,f,g,h,i,s1,s0,ch,temp1,temp2,maj;
	
	a = hash[0];
	b = hash[1];
	c = hash[2];
	d = hash[3];
	e = hash[4];
	f = hash[5];
	g = hash[6];
	h = hash[7];


	for(i=0;i<64;i++)
	{
		s1 = (rotate_right(e,6)) ^ (rotate_right(e,11)) ^ (rotate_right(e,25));
	        
		ch = (e & f) ^ ((!e) & g);
		
		temp1 = h + s1 + ch + k[i] + words[i];
		
		s0 = (rotate_right(a,2)) ^ (rotate_right(a,13))	^ (rotate_right(a,22));
		
		maj = (a & b) ^ (a & c) ^ (b & c);
		
		temp2 = s0 + maj;
		
		h = g;
		g = f;
		f = e;
		e = d + temp1;
		d = c;
		c = b;
		b = a;
		a = temp1 + temp2;
	}

	hash[0] = hash[0] + a;
	hash[1] = hash[1] + b;
	hash[2] = hash[2] + c;
	hash[3] = hash[3] + d;
	hash[4] = hash[4] + e;
	hash[5] = hash[5] + f;
	hash[6] = hash[6] + g;
	hash[7] = hash[7] + h;


}

unsigned int  rotate_right(unsigned int word,int bits)
{
	return (word>>bits | word<<(32-bits));	
}


