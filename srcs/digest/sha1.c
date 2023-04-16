
#include <crypt/ssl_utils.h>
#include <ssl_engine.h>
#include <crypt/ssl_sha1.h>

void	sha1_init(void* const vctx)
{
    *(sha1_ctx_t*)vctx = (sha1_ctx_t){
        .A = SHA1_INIT_DATA_A,
        .B = SHA1_INIT_DATA_B,
        .C = SHA1_INIT_DATA_C,
        .D = SHA1_INIT_DATA_D,
        .E = SHA1_INIT_DATA_E
    };
}

static u32 sha1_f(u32 b, u32 c, u32 d)
{ return SHA1_F(b, c, d); }

static u32 sha1_g(u32 b, u32 c, u32 d)
{ return SHA1_G(b, c, d); }

static u32 sha1_h(u32 b, u32 c, u32 d)
{ return SHA1_H(b, c, d); }

static u32 sha1_i(u32 b, u32 c, u32 d)
{ return SHA1_I(b, c, d); }

static u32 (*const sha1_operations[])(u32, u32, u32) = {
	&sha1_f,
	&sha1_g,
	&sha1_h,
	&sha1_i
};

static const u32 sha1_k[] = {
    SHA1_KF,
    SHA1_KG,
    SHA1_KH,
    SHA1_KI
};

void	sha1_update(void* const vctx, u8* const msg_u8)
{
    sha1_ctx_t*	ctx = (sha1_ctx_t*)vctx;
    sha1_ctx_t	start_ctx = *ctx;
    u32*		msg = (u32*)msg_u8;
    u32			copy[SHA1_ITERATIONS] = {0};
    u32			operation_res;
    u32         k;

    for (u64 i = 0 ; i < SHA1_ITERATIONS ; i++)
    {
        if (i < 16)
            copy[i] = BSWAP32(msg[i]);
        else
            copy[i] = VROTL32(copy[i - 3] ^ copy[i - 8] ^ copy[i - 14] ^ copy[i - 16], 1);
    }
    msg = copy;

    for (u64 i = 0 ; i < SHA1_ITERATIONS ; i++)
    {
        operation_res = sha1_operations[i / SHA1_OPSZ](ctx->B, ctx->C, ctx->D);
        k = sha1_k[i / SHA1_OPSZ];
        k += VROTL32(ctx->A, 5) + operation_res + ctx->E + msg[i];

        ctx->E = ctx->D;
        ctx->D = ctx->C;
        ctx->C = VROTL32(ctx->B, 30);
        ctx->B = ctx->A;
        ctx->A = k;
    }

	ctx->A += start_ctx.A;
	ctx->B += start_ctx.B;
	ctx->C += start_ctx.C;
	ctx->D += start_ctx.D;
	ctx->E += start_ctx.E;
}

result_t	sha1_final(void* const vctx, u8* const chunk_msg, u64 chunk_len, u64 total_len)
{
	handle_padding_arg_t padding_arg = (handle_padding_arg_t){
		.chunk_msg = chunk_msg,
		.chunk_len = chunk_len,
		.target_chunk_len = CHUNK_LEN_SHA1,
		.total_len = total_len,
		.update = &sha1_update,
		.bswap = true
	};
	handle_padding(vctx, &padding_arg);
	return u32_to_str(vctx, SHA1_HASHLEN + 1, sizeof(sha1_ctx_t) / sizeof(u32), false);
}
