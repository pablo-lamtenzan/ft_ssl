
#include <ssl_engine.h>

void print_chunk(unsigned char* chunk, size_t chunklen)
{
	printf("=============================\n");
	for (size_t i = 0 ; i < chunklen ; i++)
		printf("[%x] ", chunk[i]);
	printf("\n");
	printf("=============================\n");
}
