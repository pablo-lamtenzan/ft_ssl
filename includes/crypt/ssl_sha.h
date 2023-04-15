
#pragma once

#include "../ssl_types.h"
#include "ssl_utils.h"

#define SHA_CH(e, f, g) (((e) & (f)) ^ (~(e) & (g)))
#define SHA_MA(a, b, c) (((a) & (b)) ^ ((a) & (c)) ^ ((b) & (c)))

#define SHA_F32(x) (VROTR32(x, 2) ^ VROTR32(x, 13) ^ VROTR32(x, 22))
#define SHA_G32(x) (VROTR32(x, 6) ^ VROTR32(x, 11) ^ VROTR32(x, 25))
#define SHA_H32(x) (VROTR32(x, 7) ^ VROTR32(x, 18) ^ ((x) >> 3))
#define SHA_I32(x) (VROTR32(x, 17) ^ VROTR32(x, 19) ^ ((x) >> 10))

#define SHA_F64(x) (VROTR64(x, 28) ^ VROTR64(x, 34) ^ VROTR64(x, 39))
#define SHA_G64(x) (VROTR64(x, 14) ^ VROTR64(x, 18) ^ VROTR64(x, 41))
#define SHA_H64(x) (VROTR64(x, 1) ^ VROTR64(x, 8) ^ ((x) >> 7))
#define SHA_I64(x) (VROTR64(x, 19) ^ VROTR64(x, 61) ^ ((x) >> 6))
