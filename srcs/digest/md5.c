
#include <crypt/ssl_crypt_digest.h>
#include <crypt/ssl_md5.h>
#include <crypt/ssl_utils.h>
#include <ssl_engine.h>

void	md5_init(void* const vctx)
{
	md5_ctx_t* ctx = (md5_ctx_t*)vctx;

	*ctx = (md5_ctx_t){
		.A = INIT_DATA_A,
		.B = INIT_DATA_B,
		.C = INIT_DATA_C,
		.D = INIT_DATA_D
	};
}

static u32 md5_f(u32 b, u32 c, u32 d)
{ return F(b, c, d); }

static u32 md5_g(u32 b, u32 c, u32 d)
{ return G(b, c, d); }

static u32 md5_h(u32 b, u32 c, u32 d)
{ return H(b, c, d); }

static u32 md5_i(u32 b, u32 c, u32 d)
{ return I(b, c, d); }

static u32 (*const md5_operations[])(u32, u32, u32) = {
	&md5_f,
	&md5_g,
	&md5_h,
	&md5_i
};

static u64 md5_chunk_index_f(u64 idx)
{ return CHUNK_INDEX_F(idx); }

static u64 md5_chunk_index_g(u64 idx)
{ return CHUNK_INDEX_G(idx); }

static u64 md5_chunk_index_h(u64 idx)
{ return CHUNK_INDEX_H(idx); }

static u64 md5_chunk_index_i(u64 idx)
{ return CHUNK_INDEX_I(idx); }

static u64 (*const md5_chunk_index_resolvers[])(u64) = {
	&md5_chunk_index_f,
	&md5_chunk_index_g,
	&md5_chunk_index_h,
	&md5_chunk_index_i
};

void	md5_update(void* const vctx, u8* const msg)
{
	md5_ctx_t*	ctx = (md5_ctx_t*)vctx;
	md5_ctx_t	start_ctx = *ctx;
	u32			operation_res;
	u32			rotation_vector;
	u64			chunk_index;

	for (u64 i = 0 ; i < MD5_ITERATIONS ; i++)
	{
		operation_res = md5_operations[i / MD5_OPSZ](ctx->B, ctx->C, ctx->D);
		chunk_index = md5_chunk_index_resolvers[i / MD5_OPSZ](i);
		rotation_vector = operation_res + ctx->A + md5_sines[i] + ((u32*)(msg))[chunk_index];

		ctx->A = ctx->D;
		ctx->D = ctx->C;
		ctx->C = ctx->B;
		ctx->B += VROTL32(rotation_vector, md5_shift_amounts[i]);
	}

	ctx->A += start_ctx.A;
	ctx->B += start_ctx.B;
	ctx->C += start_ctx.C;
	ctx->D += start_ctx.D;
}

result_t	md5_final(void* const vctx, u8* const chunk_msg, u64 chunk_len, u64 total_len)
{
	u8	byte_one;
	u64	msg_len;

#if BYTE_ORDER == LITTLE_ENDIAN
	byte_one = BSWAP8((u8)1);
	msg_len = total_len * 8;
#elif BYTE_ORDER == BIG_ENDIAN
	byte_one = 1;
	///TODO: BSWAP 64
	msg_len = BSWAP32((u32)total_len * 8);
#else
# error "What kind of system is this?"
#endif

	///TODO: Stress test this 

	/* chunk[n-1] -> [msg] ; chunk[n] -> [1][0s][msg_len] */
	if (chunk_len == CHUNK_LEN_MD5)
	{
		md5_update(vctx, chunk_msg);
		memset(chunk_msg, 0, CHUNK_LEN_MD5 - sizeof(u64));
		chunk_buffer[0] = byte_one;
	}
	/* chunk[n-1] -> [msg][1][0s] ; chunk[n] -> [0s][msg_len] */
	else if (CHUNK_LEN_MD5 - chunk_len < sizeof(u64) + 1)
	{
		chunk_msg[chunk_len] = byte_one;
		for (u64 i = chunk_len + 1 ; i < CHUNK_LEN_MD5 ; i++)
			chunk_msg[i] = 0;
		md5_update(vctx, chunk_msg);
		memset(chunk_msg, 0, CHUNK_LEN_MD5 - sizeof(u64));
	}
	/* chunk[n] -> [msg][1][0s][msg_len] */
	else
	{
		chunk_msg[chunk_len] = byte_one;
		for (u64 i = chunk_len + 1 ; i < CHUNK_LEN_MD5 - sizeof(u64); i++)
			chunk_msg[i] = 0;
	}
	*(u64*)&chunk_msg[CHUNK_LEN_MD5 - sizeof(u64)] = msg_len;
	md5_update(vctx, chunk_msg);

	return u32_to_str(vctx, 36, 4);
}
