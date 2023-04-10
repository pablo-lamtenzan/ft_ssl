
#include <ssl_types.h>
#include <ssl_error.h>
#include <crypt/ssl_utils.h>
#include <ftlibc.h>

result_t		 u32_to_str(u32* vectors, u64 alloc_size, u64 size)
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

///TODO:
//#if BYTE_ORDER == BIG_ENDIAN
		vectors[i] = BSWAP32(vectors[i]);
//#endif

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

result_t		 u64_to_str(u64* vectors, u64 alloc_size, u64 size)
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

#if BYTE_ORDER == BIG_ENDIAN
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
