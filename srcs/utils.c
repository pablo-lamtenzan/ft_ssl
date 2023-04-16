
#include <ssl_types.h>
#include <ssl_error.h>
#include <crypt/ssl_utils.h>
#include <ssl_engine.h>
#include <ftlibc.h>

result_t		 u32_to_str(u32* vectors, u64 alloc_size, u64 size, bool bswap)
{
	result_t	result;
	char*		tmp;

	if ((result = malloc(sizeof(*result) * alloc_size)) == NULL)
	{
		ERAISE_ERRNO("malloc");
		return NULL;
	}

	for (u64 i = 0 ; i < size ; i++)
	{


#if BYTE_ORDER == LITTLE_ENDIAN
	if (bswap)
		vectors[i] = BSWAP32(vectors[i]);
#else
	if (!bswap)
		vector[i] = BSWAP32(vectors[i]);
#endif

		if ((tmp = ft_uitoa_base_len(vectors[i], 16, 'a', 8)) == NULL)
		{
			ERAISE_ERRNO("malloc");
			return NULL;
		}
		ft_strlcpy((char*)(result + (i * 8)), tmp, 9);
		free(tmp);
	}

	return result;
}

result_t		 u64_to_str(u64* vectors, u64 alloc_size, u64 size, bool bswap)
{
	result_t	result;
	char*		tmp;

	if ((result = malloc(sizeof(*result) * alloc_size)) == NULL)
	{
		ERAISE_ERRNO("malloc");
		return NULL;
	}

	for (u64 i = 0 ; i < size ; i++)
	{

#if BYTE_ORDER == LITTLE_ENDIAN
	if (bswap)
		vectors[i] = BSWAP64(vectors[i]);
#else
	if (!bswap)
		vectors[i] = BSWAP64(vectors[i]);
#endif

		if ((tmp = ft_uitoa_base_len(vectors[i], 16, 'a', 16)) == NULL)
		{
			ERAISE_ERRNO("malloc");
			return NULL;
		}
		ft_strlcpy((char*)(result + (i * 16)), tmp, 17);
		free(tmp);
	}

	return result;
}

// The algorithm inverts neigbours inverting first blocks of 1, then of 2, and finally of 4
/* ABCDEFGH -> BADCFEHG -> DCBAHGFE -> HGFEDCBA */
u64		swap_u64bits(u64 target)
{
    target = ((target << 8) & 0xff00ff00ff00ff00ull) | ((target >> 8) & 0xff00ff00ff00ffull);
    target = ((target << 16) & 0xffff0000ffff0000ull) | ((target >> 16) & 0xffff0000ffffull);
    return ((target << 32) | (target >> 32));
}

u32		swap_u32bits(u32 target)
{
    target = ((target << 8) & 0xff00ff00) | ((target >> 8) & 0xff00ff);
    return ((target << 16) | (target >> 16));
}

void	handle_padding(void* const vctx, handle_padding_arg_t* const args)
{
	u8	byte_one;
	u64	msg_len;

#if BYTE_ORDER == LITTLE_ENDIAN
	byte_one = BSWAP8((u8)1);
	msg_len = args->bswap ? BSWAP64(args->total_len * 8) : args->total_len * 8;
#elif BYTE_ORDER == BIG_ENDIAN
	byte_one = 1;
	msg_len = args->bswap ? args->total_len * 8 : BSWAP64(args->total_len * 8);
#else
# error "What kind of system is this?"
#endif

	/* chunk[n-1] -> [msg] ; chunk[n] -> [1][0s][msg_len] */
	if (args->chunk_len == args->target_chunk_len)
	{
		args->update(vctx, args->chunk_msg);
		ft_memset((char*)args->chunk_msg, 0, args->target_chunk_len - sizeof(u64));
		args->chunk_msg[0] = byte_one;
	}
	/* chunk[n-1] -> [msg][1][0s] ; chunk[n] -> [0s][msg_len] */
	else if (args->target_chunk_len - args->chunk_len < sizeof(u64) + 1)
	{
		args->chunk_msg[args->chunk_len] = byte_one;
		for (u64 i = args->chunk_len + 1 ; i < args->target_chunk_len ; i++)
			args->chunk_msg[i] = 0;
		args->update(vctx, args->chunk_msg);
		ft_memset((char*)args->chunk_msg, 0, args->target_chunk_len - sizeof(u64));
	}
	/* chunk[n] -> [msg][1][0s][msg_len] */
	else
	{
		args->chunk_msg[args->chunk_len] = byte_one;
		for (u64 i = args->chunk_len + 1 ; i < args->target_chunk_len - sizeof(u64); i++)
			args->chunk_msg[i] = 0;
	}
	*(u64*)&args->chunk_msg[args->target_chunk_len - sizeof(u64)] = msg_len;
	args->update(vctx, args->chunk_msg);
}
