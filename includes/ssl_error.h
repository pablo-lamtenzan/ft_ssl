
#pragma once

#include "ssl_portability.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>

typedef enum
{
	SUCCESS,
	E_UNAVALAIBLE,
	E_SYSCALL,
	E_UNKOWNARGUMENT,
	E_FILENOTFOUND,
} err_t;

#define ERROR(msg) fprintf(stderr, msg)
#define FERROR(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)

#define PFX_ERROR __progname ": error: "
#define PFX_WARNING __progname ": warning: "

#define ERAISE_ERRNO(name) ( \
	FERROR( \
		PFX_ERROR "%s: %s (errno: %d).\n", \
		name, strerror(errno), errno \
	) \
)

#define EFMT_CMDNOTFOUND PFX_ERROR "'%s' is an invalid command.\n"
#define EFMT_OPTARGNULL PFX_ERROR "option '%s': missing arguments.\n"
#define EFMT_OPTNOTFOUND PFX_ERROR "no such option '%s'.\n"
#define EFMT_FILENOTFOUND PFX_ERROR "%s: %s: No such file or directory.\n"
