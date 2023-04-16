
#pragma once

#include "../ssl_types.h"

void		md5_init(void* const vctx);
void		md5_update(void* const vctx, u8* const msg);
result_t	md5_final(void* const vctx, u8* const chunk_msg, u64 chunk_len, u64 total_len);

void		sha0_init(void* const vctx);
void		sha0_update(void* const vctx, u8* const msg);
result_t	sha0_final(void* const vctx, u8* const chunk_msg, u64 chunk_len, u64 total_len);

void		sha1_init(void* const vctx);
void		sha1_update(void* const vctx, u8* const msg);
result_t	sha1_final(void* const vctx, u8* const chunk_msg, u64 chunk_len, u64 total_len);

void		sha224_init(void* const vctx);
void		sha224_update(void* const vctx, u8* const msg);
result_t	sha224_final(void* const vctx, u8* const chunk_msg, u64 chunk_len, u64 total_len);

void		sha256_init(void* const vctx);
void		sha256_update(void* const vctx, u8* const msg);
result_t	sha256_final(void* const vctx, u8* const chunk_msg, u64 chunk_len, u64 total_len);

void		sha384_init(void* const vctx);
void		sha384_update(void* const vctx, u8* const msg);
result_t	sha384_final(void* const vctx, u8* const chunk_msg, u64 chunk_len, u64 total_len);

void		sha512_init(void* const vctx);
void		sha512_update(void* const vctx, u8* const msg);
result_t	sha512_final(void* const vctx, u8* const chunk_msg, u64 chunk_len, u64 total_len);
