
#pragma once

#include "../ssl_types.h"
#include "../ssl_error.h"

#define DGT_O_DISPLAY_HELP_STR "-h"
#define DGT_O_PRINT_STDIN_STR "-p"
#define DGT_O_INPUT_STRING_STR "-s"
#define DGT_O_QUIET_MODE_STR "-q"
#define DGT_O_REVERSE_OUT_STR "-r"

typedef enum
{
	O_DISPLAY_HELP	= (1 << 0),
	O_PRINT_STDIN	= (O_DISPLAY_HELP << 1),
	O_INPUT_STRING	= (O_PRINT_STDIN << 1),
	O_QUIET_MODE	= (O_INPUT_STRING << 1),
	O_REVERSE_OUT	= (O_QUIET_MODE << 1)
} digest_opts_t;

typedef struct
{
	opts_t			opts;
	const char*		input_str;
} digest_parse_t;

err_t	parse_digest_cli(const char** av[], void* const res);
