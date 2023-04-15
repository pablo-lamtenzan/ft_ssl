
#pragma once

#include "parse/ssl_parse_digest.h"
#include "crypt/ssl_crypt_digest.h"

#define CMD_NAME_MD5 "md5"
#define CMD_NAME_SHA224 "sha224"
#define CMD_NAME_SHA256 "sha256"
#define CMD_NAME_SHA384 "sha384"
#define CMD_NAME_SHA512 "sha512"

#define CHUNK_LEN_MD5 64
#define CHUNK_LEN_SHA224 64
#define CHUNK_LEN_SHA256 64
#define CHUNK_LEN_SHA384 128
#define CHUNK_LEN_SHA512 128

#define CTX_BUFF_SIZE 64

#define CHUNK_LEN_MAX CHUNK_LEN_SHA512

typedef enum
{
	DISPLAY_STDIN,
	DISPLAY_STRING,
	DISPLAY_FILE
} display_t;

typedef struct command command_t;

typedef err_t (*const run_func_t)(command_t* const, const char***);
typedef	err_t (*const parse_func_t)(const char***, void* const);
typedef void (*const crypt_init_func_t)(void* const);
typedef void (*const crypt_update_func_t)(void* const, u8* const);
typedef result_t (*const crypt_final_func_t)(void* const, u8* const, u64, u64);
typedef void (*const display_func_t)(command_t* const, result_t const, display_t, void* const);

err_t	digest_run(command_t* const cmd, const char** av[]);
void	digest_display(command_t* const cmd, result_t const hash, display_t source, void* const vparse);

typedef struct
{
	u8* const chunk_msg;
	u64 chunk_len;
	u64 target_chunk_len;
	u64 total_len;
	crypt_update_func_t update;
	bool bswap;
} handle_padding_arg_t;

void	handle_padding(void* const vctx, handle_padding_arg_t* const args);

struct command
{
	const char*				name;
	u64						chunklen;
	run_func_t				run;
	parse_func_t			parse;
	crypt_init_func_t		init;
	crypt_update_func_t		update;
	crypt_final_func_t		final;
	display_func_t			display;
};

static const command_t commands[] = {
	{
		.name = CMD_NAME_MD5,
		.chunklen = CHUNK_LEN_MD5,
		.run = &digest_run,
		.parse = &parse_digest_cli,
		.init = &md5_init,
		.update = &md5_update,
		.final = &md5_final,
		.display = &digest_display
	},
	{
		.name = CMD_NAME_SHA224,
		.chunklen = CHUNK_LEN_SHA224,
		.run = &digest_run,
		.parse = &parse_digest_cli,
		.init = &sha224_init,
		.update = &sha224_update,
		.final = &sha224_final,
		.display = &digest_display
	},
	{
		.name = CMD_NAME_SHA256,
		.chunklen = CHUNK_LEN_SHA256,
		.run = &digest_run,
		.parse = &parse_digest_cli,
		.init = &sha256_init,
		.update = &sha256_update,
		.final = &sha256_final,
		.display = &digest_display
	},
	{
		.name = CMD_NAME_SHA384,
		.chunklen = CHUNK_LEN_SHA384,
		.run = &digest_run,
		.parse = &parse_digest_cli,
		.init = &sha384_init,
		.update = &sha384_update,
		.final = &sha384_final,
		.display = &digest_display
	},
	{
		.name = CMD_NAME_SHA512,
		.chunklen = CHUNK_LEN_SHA512,
		.run = &digest_run,
		.parse = &parse_digest_cli,
		.init = &sha512_init,
		.update = &sha512_update,
		.final = &sha512_final,
		.display = &digest_display
	},
};

extern u8 chunk_buffer[CHUNK_LEN_MAX];

err_t	select_command(const char** cmd_input_name[], command_t** const dest);

err_t	compute_stdin(command_t* const cmd, void* const dest, result_t* const res, u8** bytes);
void	compute_string(command_t* const cmd, void* const dest, const char* str, result_t* const res);
err_t	compute_file(command_t* const cmd, const char* filename, void* const dest, result_t* const res);
