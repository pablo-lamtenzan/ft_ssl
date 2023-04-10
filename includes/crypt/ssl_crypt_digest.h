
#pragma once

#include "../ssl_types.h"
#include "../ssl_error.h"

#include <stdio.h>

#define MD5_HASH_SIZE 16
#define SHA256_HASH_SIZE 32
#define SHA512_HASH_SIZE 64

#define DIGEST_HASH_SZMAX SHA512_HASH_SIZE

void			md5_init(void* const ctx);
void			md5_update(void* const vctx, u8* const msg);
result_t	    md5_final(void* const vctx, u8* const chunk_msg, u64 chunk_len, u64 total_len);

result_t		sha256(void* const ctx);
result_t		sha512(void* const ctx);
