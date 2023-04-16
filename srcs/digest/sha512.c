
#include <crypt/ssl_sha512.h>
#include <crypt/ssl_utils.h>
#include <ssl_engine.h>

void	sha512_init(void* const vctx)
{
	*(sha512_ctx_t*)vctx = (sha512_ctx_t){
		.A = SHA512_INIT_DATA_A,
		.B = SHA512_INIT_DATA_B,
		.C = SHA512_INIT_DATA_C,
		.D = SHA512_INIT_DATA_D,
		.E = SHA512_INIT_DATA_E,
		.F = SHA512_INIT_DATA_F,
		.G = SHA512_INIT_DATA_G,
		.H = SHA512_INIT_DATA_H
	};
}

void	sha512_update(void* const vctx, u8* const msg_u8)
{
	sha512_ctx_t*	ctx = (sha512_ctx_t*)vctx;
	sha512_ctx_t	start_ctx = *ctx;
	u64*			msg = (u64*)msg_u8;
	u64				copy[SHA512_ITERATIONS] = {0};
	u64				s0;
	u64				s1;

	for (u64 i = 0 ; i < SHA512_ITERATIONS ; i++)
	{
		if (i < 16)
			copy[i] = BSWAP64(msg[i]);
		else
			copy[i] = SHA_I64(copy[i - 2]) + SHA_H64(copy[i - 15]) + copy[i - 7] + copy[i - 16];
	}
	msg = copy;

	for (u64 i = 0 ; i < SHA512_ITERATIONS ; i++)
	{
		s0 = ctx->H + SHA_G64(ctx->E) + SHA_CH(ctx->E, ctx->F, ctx->G) + sha512_factorials[i] + msg[i];
		s1 = SHA_MA(ctx->A, ctx->B, ctx->C) + SHA_F64(ctx->A);

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

result_t	sha512_final(void* const vctx, u8* const chunk_msg, u64 chunk_len, u64 total_len)
{
	handle_padding_arg_t padding_arg = (handle_padding_arg_t){
		.chunk_msg = chunk_msg,
		.chunk_len = chunk_len,
		.target_chunk_len = CHUNK_LEN_SHA512,
		.total_len = total_len,
		.update = &sha512_update,
		.bswap = true
	};

	handle_padding(vctx, &padding_arg);
	return u64_to_str(vctx, SHA512_HASHLEN + 1, sizeof(sha512_ctx_t) / sizeof(u64), false);
}
