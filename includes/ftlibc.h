
#pragma once

#include <stdlib.h>

void	ft_memset(char* dest, int c, size_t size);
void	*ft_calloc(size_t nitems, size_t n);
size_t 	ft_strlen(const char* str);
int     ft_strcmp(const char *a, const char *b);
void	*ft_memcpy(void *dst, const void *src, size_t n);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
char*   ft_uitoa_base(unsigned long nb, int base, char letter);
char*   ft_uitoa_base_len(unsigned long nb, int base, char letter, unsigned long len);
char	**split_multicharset(char *str, char *charset);
void	free_split(char** s);