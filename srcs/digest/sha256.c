
#include <crypt/ssl_sha256.h>
#include <crypt/ssl_utils.h>
#include <ssl_engine.h>

void	sha256_init(void* const vctx)
{
	*(sha256_ctx_t*)vctx = (sha256_ctx_t){
		.A = SHA256_INIT_DATA_A,
		.B = SHA256_INIT_DATA_B,
		.C = SHA256_INIT_DATA_C,
		.D = SHA256_INIT_DATA_D,
		.E = SHA256_INIT_DATA_E,
		.F = SHA256_INIT_DATA_F,
		.G = SHA256_INIT_DATA_G,
		.H = SHA256_INIT_DATA_H
	};
}

void	sha256_update(void* const vctx, u8* const msg_u8)
{
	sha256_ctx_t*	ctx = (sha256_ctx_t*)vctx;
	sha256_ctx_t	start_ctx = *ctx;
	u32*			msg = (u32*)msg_u8;
	u32				copy[SHA256_ITERATIONS] = {0};
	u32				s0;
	u32				s1;

	for (u64 i = 0 ; i < SHA256_ITERATIONS ; i++)
	{
		if (i < 16)
			copy[i] = BSWAP32(msg[i]);
		else
			copy[i] = SHA_I32(copy[i - 2]) + SHA_H32(copy[i - 15]) + copy[i - 7] + copy[i - 16];
	}
	msg = copy;

	for (u64 i = 0 ; i < SHA256_ITERATIONS ; i++)
	{
		s0 = ctx->H + SHA_G32(ctx->E) + SHA_CH(ctx->E, ctx->F, ctx->G) + sha256_factorials[i] + msg[i];
		s1 = SHA_MA(ctx->A, ctx->B, ctx->C) + SHA_F32(ctx->A);

		ctx->H = ctx->G;
		ctx->G = ctx->F;
		ctx->F = ctx->E;
		ctx->E = ctx->D + s0;
		ctx->D = ctx->C;
		ctx->C = ctx->B;
		ctx->B = ctx->A;
		ctx->A = s0 + s1;
	}

	ctx->A += start_ctx.A;
	ctx->B += start_ctx.B;
	ctx->C += start_ctx.C;
	ctx->D += start_ctx.D;
	ctx->E += start_ctx.E;
	ctx->F += start_ctx.F;
	ctx->G += start_ctx.G;
	ctx->H += start_ctx.H;
}

result_t	sha256_final(void* const vctx, u8* const chunk_msg, u64 chunk_len, u64 total_len)
{
	handle_padding_arg_t padding_arg = (handle_padding_arg_t){
		.chunk_msg = chunk_msg,
		.chunk_len = chunk_len,
		.target_chunk_len = CHUNK_LEN_SHA256,
		.total_len = total_len,
		.update = &sha256_update,
		.bswap = true
	};

	handle_padding(vctx, &padding_arg);
	return u32_to_str(vctx, SHA256_HASHLEN + 1, sizeof(sha256_ctx_t) / sizeof(u32), false);
}
