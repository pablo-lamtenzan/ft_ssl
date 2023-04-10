
#pragma once

#include <ssl_types.h>

#define VROTL32(vec, amount) (((vec) << (amount)) | (vec) >> (32 - (amount)))
#define VRPTR32(vec, amount) (((vec) >> (amount)) | (vec) << (32 - (amount)))
#define VROTL64(vec, amount) (((vec) << (amount)) | (vec) >> (64 - (amount)))
#define VRPTR64(vec, amount) (((vec) >> (amount)) | (vec) << (64 - (amount)))

/// https://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte
#define BSWAP8(x) (\
    (u8)(((u8)(x) * 0x0202020202ULL & 0x010884422010ULL) % 0x3ff) \
)
#define BSWAP32(x) ( \
    (((((x) << 8) & 0xFF00FF00) | (((x) >> 8) & 0xFF00FF)) << 16) \
    | (((((x) << 8) & 0xFF00FF00) | (((x) >> 8) & 0xFF00FF)) >> 16) \
)
#define BSWAP64(x) (x)

result_t	u32_to_str(u32* vectors, u64 alloc_size, u64 size);
result_t    u64_to_str(u64* vectors, u64 alloc_size, u64 size);

///TODO:
// The algorithm inverts neigbours inverting first blocks of 1, them of 2, and finally of 4
/* ABCDEFGH -> BADCFEHG -> DCBAHGFE -> HGFEDCBA */
// unsigned long       swap_u64bits(unsigned long target)
// {
//     target = ((target << 8) & 0xff00ff00ff00ff00ull) | ((target >> 8) & 0xff00ff00ff00ffull);
//     target = ((target << 16) & 0xffff0000ffff0000ull) | ((target >> 16) & 0xffff0000ffffull);
//     return ((target << 32) | (target >> 32));
// }
