
#pragma once

#include "../ssl_types.h"

#define SHA1_INIT_DATA_A 0x67452301
#define SHA1_INIT_DATA_B 0xEFCDAB89
#define SHA1_INIT_DATA_C 0x98BADCFE
#define SHA1_INIT_DATA_D 0x10325476
#define SHA1_INIT_DATA_E 0xC3D2E1F0

#define SHA1_ITERATIONS 80
#define SHA1_HASHLEN 40

#define SHA1_OPSZ 20

typedef struct
{
	u32 A;
	u32 B;
	u32 C;
	u32 D;
    u32 E;
} __attribute__ ((packed, aligned(sizeof(u32)))) sha1_ctx_t;

#define SHA1_F(b, c, d) (((b) & (c)) ^ (~(b) & (d)))
#define SHA1_G(b, c, d) ((b) ^ (c) ^ (d))
#define SHA1_H(b, c, d) (((b) & (c)) ^ ((b) & (d)) ^ ((c) & (d)))
#define SHA1_I(b, c, d) SHA1_G(b, c, d)

#define SHA1_KF 0x5A827999
#define SHA1_KG 0x6ED9EBA1
#define SHA1_KH 0x8F1BBCDC
#define SHA1_KI 0xCA62C1D6
