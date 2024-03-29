
#pragma once

#include "../ssl_types.h"

#define MD5_INIT_DATA_A 0x67452301
#define MD5_INIT_DATA_B 0xefcdab89
#define MD5_INIT_DATA_C 0x98badcfe
#define MD5_INIT_DATA_D 0x10325476

#define MD5_ITERATIONS 64
#define MD5_HASHLEN 32

#define MD5_OPSZ 16

typedef struct
{
	u32 A;
	u32 B;
	u32 C;
	u32 D;
} __attribute__ ((packed, aligned(sizeof(u32)))) md5_ctx_t;

#define MD5_F(b, c, d) (((b) & (c)) | (~(b) & (d)))
#define MD5_G(b, c, d) (((b) & (d)) | ((c) & ~(d)))
#define MD5_H(b, c, d) ((b) ^ (c) ^ (d))
#define MD5_I(b, c, d) ((c) ^ ((b) | ~(d)))

#define CHUNK_INDEX_F(idx) (idx)
#define CHUNK_INDEX_G(idx) ((5 * (idx) + 1) % 16)
#define CHUNK_INDEX_H(idx) ((3 * (idx) + 5) % 16)
#define CHUNK_INDEX_I(idx) ((7 * (idx)) % 16)

/*  Binary integer part of the sines of 64 1st integers */
const u32 md5_sines[] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 
    0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340,
    0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 
    0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa,
    0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92,
    0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

const u32 md5_shift_amounts[] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};
