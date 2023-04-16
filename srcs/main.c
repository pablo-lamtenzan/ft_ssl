
#include <ssl_engine.h>
#include <ssl_utils.h>
#include <ftlibc.h>

#include <unistd.h>

#define PROMT __progname "> "
#define CMD_HELP "help"
#define CMD_EXIT "exit"
#define CLI_HELP_LONG "--help"
#define CLI_HELP_SHORT "-h"

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

__always_inline
static err_t handle_stdin_cli(char*** data)
{
	err_t	st = SUCCESS;
	static	u8 buff[0x1000] = {0};

	fprintf(stdout, PROMT);
	fflush(stdout);

	const ssize_t nread = read(STDIN_FILENO, buff, ARRLEN(buff));
	if (nread < 0)
	{
		ERAISE_ERRNO("read");
		st = E_SYSCALL;
	}
	else if (nread <= 1)
		st = E_UNAVALAIBLE;
	else
	{
		buff[nread - 1] = '\0';
		*data = split_multicharset((char*)buff, " ");

		if (ft_strcmp((*data)[0], CMD_HELP) == 0)
		{
			display_commands();
			free_split(*data);
			st = E_UNKOWNARGUMENT;
		}
		else if (ft_strcmp((*data)[0], CMD_EXIT) == 0)
		{
			st = E_UNAVALAIBLE;
			free_split(*data);
		}
	}
	return st;
}

u8	chunk_buffer[CHUNK_LEN_MAX];

int main(int ac, char* av[])
{
	err_t		st = SUCCESS;
	command_t*	cmd;
	bool		loop = true;
	char**		split_start;

	++av;

	if (ac == 2 && (!ft_strcmp(*av, CLI_HELP_LONG) || (!ft_strcmp(*av, CLI_HELP_SHORT))))
	{
		display_usage();
		st = E_UNKOWNARGUMENT;
	}
	else
	{
		while (loop == true)
		{
			loop = false;

			if (ac == 1)
			{
				st = handle_stdin_cli(&av);
				if (st == E_SYSCALL)
					break ;
				else if (st == E_UNKOWNARGUMENT)
					continue ;
				else if (st == E_UNAVALAIBLE)
				{
					st = SUCCESS;
					break ;
				}
				else
					loop = true;
			}

			split_start = av;

			if ((st = select_command((const char***)&av, &cmd)) == SUCCESS)
				cmd->run(cmd, (const char***)&av);
			else if (st == E_UNKOWNARGUMENT)
				display_commands();

			if (ac == 1)
				free_split(split_start);
		}
	}
	return st;
}
