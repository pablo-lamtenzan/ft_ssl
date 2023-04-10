
#pragma once

#include "../ssl_types.h"

typedef struct
{
	u64	A;
	u64	B;
	u64	C;
	u64	D;
	u64	E;
	u64	F;
	u64	G;
	u64	H;
} __attribute__((packed, aligned(8))) sha512_ctx_t;
