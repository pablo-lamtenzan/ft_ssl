
#pragma once

#include "parse/ssl_parse_digest.h"
#include "crypt/ssl_crypt_digest.h"
#include "display/ssl_display_digest.h"

#define CMD_NAME_MD5 "md5"
#define CMD_NAME_SHA256 "sha256"
#define CMD_NAME_SHA512 "sha512"

#define CHUNK_LEN_MD5 64
#define CHUNK_LEN_SHA256 128
#define CHUNK_LEN_SHA512 128

#define CHUNK_LEN_MAX CHUNK_LEN_SHA512

typedef	err_t (*const parse_func_t)(const char***, void* const);
typedef result_t (*const crypt_func_t)(void* const);
typedef void (*const display_func_t)(result_t* const, u64, void* const);

typedef struct
{
	const char*		name;
	u64				chunklen;
	parse_func_t	parse;
	crypt_func_t	crypt;
	display_func_t	display;
} command_t;

static const command_t commands[] = {
	{
		.name = CMD_NAME_MD5,
		.chunklen = CHUNK_LEN_MD5,
		.parse = &parse_digest_cli,
		.crypt = 0, //&md5,
		.display = 0, //&display_digest
	},
	// {
	// 	.name = CMD_NAME_SHA256,
	// 	.chunklen = CHUNK_LEN_SHA256,
	// 	.parse = &parse_digest_cli,
	// 	.crypt = &sha256,
	// 	.display = &display_digest
	// },
	// {
	// 	.name = CMD_NAME_SHA512,
	// 	.chunklen = CHUNK_LEN_SHA512,
	// 	.parse = &parse_digest_cli,
	// 	.crypt = &sha512,
	// 	.display = &display_digest
	// }
};

extern u8 chunk_buffer[CHUNK_LEN_MAX];

err_t	select_command(const char** cmd_input_name[], command_t** const dest);
