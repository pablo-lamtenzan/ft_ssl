
#include <ssl_engine.h>

__always_inline
static void display_usage()
{
	fprintf(stdout, "USAGE: " __progname " <algorithm> [ ... ]\n");
}

__always_inline
static void	display_usage_digest()
{
	fprintf(stdout, "USAGE: " __progname " <digest> [ -pqr ] [ -s <string> ] [ file1[, file2[, ...] ] ]\n");
}

u8	chunk_buffer[CHUNK_LEN_MAX];

int main(int ac, const char* av[])
{
	err_t		st = SUCCESS;
	command_t*	cmd;

	if (ac == 1)
	{
		display_usage();
		st = E_UNKOWNARGUMENT;
		goto end;
	}

	++av;

	if ((st = select_command(&av, &cmd)) != SUCCESS)
		goto end;

	if (ac == 2)
	{
		///TODO: This is not polymorphic and maybe is valid ...
		/// I think this is invalid (e.g. just read from stdin is valid ...)

		display_usage_digest();
		st = E_UNKOWNARGUMENT;
		goto end;
	}

	///TODO: For testing purpose, better if i can only use polymorphism in the main
	digest_parse_t pg = {0};

	if ((st = cmd->parse(&av, &pg)) != SUCCESS)
		goto end;

	if (pg.opts & O_DISPLAY_HELP)
	{
		display_usage_digest();
		goto end;
	}

	printf("DEBUG: OPTS: [%d] | STR: %s\n", pg.opts, pg.input_str);

end:
	return st;
}
