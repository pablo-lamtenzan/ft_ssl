
#include <ftlibc.h>

void	ft_memset(char* dest, int c, size_t size)
{
	for (size_t i = 0 ; i < size ; i++)
		dest[i] = c;
}

void	*ft_calloc(size_t nitems, size_t n)
{
	void *p;

	if (!(p = malloc(nitems * n)))
		return (NULL);
	ft_memset(p, 0, nitems * n);
	return (p);
}

size_t 		ft_strlen(const char* str)
{
    char*	it;
	
	it = (char* )str;
	while (*it)
		it++;
	return (it - str);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char *temp;

	if (!dst && !src)
		return (0);
	temp = dst;
	while (n--)
		*temp++ = *(char *)src++;
	return (dst);
}

size_t			ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t		count;
	size_t		size;

	count = 0;
	size = 0;
	if (!dst)
		return (0);
	while (src[size])
		size++;
	if (!dstsize)
		return (size);
	while (src[count] && count < dstsize - 1)
	{
		dst[count] = src[count];
		count++;
	}
	dst[count] = '\0';
	return (size);
}

char*   ft_uitoa_base(unsigned long nb, int base, char letter)
{
    unsigned long   tmp;
    int             lenght;
    char*           result;

    tmp = nb;
    lenght = 1;
    while (tmp /= base)
        ++lenght;
    if (!(result = ft_calloc(lenght + 1, sizeof(char))))
        return (NULL);
    while (lenght--)
    {
        if (nb % base >= 10)
            result[lenght] = nb % base - 10 + letter;
        else
            result[lenght] = nb % base + '0';
        nb /= base;
    }
    return (result);
}

char*   ft_uitoa_base_len(unsigned long nb, int base, char letter, unsigned long len)
{
    int     i;
    int     diff;
    char   *result;
    char*   tmp;
    
    i = 0;
    result = ft_uitoa_base(nb, base, letter);
    if (result && (diff = len - ft_strlen(result)) > 0)
    {
        if (!(tmp = ft_calloc(len + 1, sizeof(char))))
            return (NULL);
        while (i < diff)
            tmp[i++] = '0';
        ft_memcpy(tmp + i, result, len - diff);
        free(result);
        result = tmp;
    }
    return (result);
}

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t				i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < n - 1 && s1[i] == s2[i] && s1[i] && s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
