
#include <ssl_engine.h>
#include <ssl_utils.h>

__always_inline
static void display_usage()
{
	fprintf(stdout, "usage: " __progname " command [flags] [file/string]\n");
}

__always_inline
static void display_commands()
{
	fprintf(stdout, "\nCommands:\n");
	for (u64 i = 0 ; i < ARRLEN(commands) ; i++)
		fprintf(stdout, "%s\n", commands[i].name);

	fprintf(stdout, "\nFlags:\n");
	for (u64 i = 0 ; i < ARRLEN(opts_str) ; i++)
		fprintf(stdout, "%s ", opts_str[i]);
	fprintf(stdout, "\n");
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

	if ((st = select_command(&av, &cmd)) == SUCCESS)
		cmd->run(cmd, &av);
	else if (st == E_UNKOWNARGUMENT)
		display_commands();

end:
	return st;
}
