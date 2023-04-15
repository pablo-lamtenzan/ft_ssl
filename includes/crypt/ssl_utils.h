
#pragma once

#include "../ssl_types.h"

#define VROTL32(vec, amount) (((vec) << (amount)) | (vec) >> (32 - (amount)))
#define VROTR32(vec, amount) (((vec) >> (amount)) | (vec) << (32 - (amount)))

#define VROTL64(vec, amount) (((vec) << (amount)) | (vec) >> (64 - (amount)))
#define VROTR64(vec, amount) (((vec) >> (amount)) | (vec) << (64 - (amount)))

#define BSWAP8(x) (((x) * 0x0202020202ULL & 0x010884422010ULL) % 1023)
#define BSWAP32(x) (swap_u32bits(x))
#define BSWAP64(x) (swap_u64bits(x))

result_t	u32_to_str(u32* vectors, u64 alloc_size, u64 size, bool bswap);
result_t	u64_to_str(u64* vectors, u64 alloc_size, u64 size, bool bswap);
u64			swap_u64bits(u64 target);
u32			swap_u32bits(u32 target);
