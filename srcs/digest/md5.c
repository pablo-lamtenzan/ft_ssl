
#include <crypt/ssl_crypt_digest.h>
#include <crypt/ssl_md5.h>
#include <crypt/ssl_utils.h>
#include <ssl_engine.h>

void	md5_init(void* const vctx)
{
	*(md5_ctx_t*)vctx = (md5_ctx_t){
		.A = MD5_INIT_DATA_A,
		.B = MD5_INIT_DATA_B,
		.C = MD5_INIT_DATA_C,
		.D = MD5_INIT_DATA_D
	};
}

static u32 md5_f(u32 b, u32 c, u32 d)
{ return MD5_F(b, c, d); }

static u32 md5_g(u32 b, u32 c, u32 d)
{ return MD5_G(b, c, d); }

static u32 md5_h(u32 b, u32 c, u32 d)
{ return MD5_H(b, c, d); }

static u32 md5_i(u32 b, u32 c, u32 d)
{ return MD5_I(b, c, d); }

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
		rotation_vector = operation_res + ctx->A + md5_sines[i] + ((u32*)msg)[chunk_index];

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
	handle_padding_arg_t padding_arg = (handle_padding_arg_t){
		.chunk_msg = chunk_msg,
		.chunk_len = chunk_len,
		.target_chunk_len = CHUNK_LEN_MD5,
		.total_len = total_len,
		.update = &md5_update,
		.bswap = false
	};

	handle_padding(vctx, &padding_arg);
	return u32_to_str(vctx, MD5_HASHLEN + 1, sizeof(md5_ctx_t) / sizeof(u32), true);
}
