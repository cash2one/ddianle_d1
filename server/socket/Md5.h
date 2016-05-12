#ifndef __MD5_H__
#define __MD5_H__

/* POINTER defines a generic pointer type */
typedef unsigned char *POINTER;

/* UINT2 defines a two byte word */
typedef unsigned short int UINT2;

/* UINT4 defines a four byte word */
typedef unsigned long int UINT4;

/* MD5 context. */
struct  MD5_CTX
{
	UINT4 state[4];                                   /* state (ABCD) */
	UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64];                         /* input buffer */
};

void MD5Init (MD5_CTX *);
void MD5Update (MD5_CTX *, const unsigned char *, unsigned int);
void MD5Final (unsigned char [16], MD5_CTX *);

//
int MDFile(const char *filename , unsigned char digest[16]);
void MDString (const char *str,unsigned char digest[16]);
void MDData(const char *data,int len,unsigned char digest[16]);

#endif

