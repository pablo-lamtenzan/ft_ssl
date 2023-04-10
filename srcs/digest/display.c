
#include <ssl_engine.h>
#include <ftlibc.h>

static char* ft_toupper(const char* s)
{
	static char buff[0x10];

	const u64 slen = ft_strlen(s);
	buff[slen] = 0;

	for (u64 i = 0 ; i < slen ; i++)
	{
		if (s[i] >= 'a' && s[i] <= 'z')
			buff[i] = s[i] - ('a' - 'A');
		else
			buff[i] = s[i];
	}

	return buff;
}

__always_inline
static void	display_stdin(command_t* const cmd, result_t const hash, const digest_parse_t* const parse)
{
	(void)cmd;

	if (parse->opts & O_PRINT_STDIN)
	{
		if (parse->opts & O_QUIET_MODE)
			fprintf(stdout, "%s\n%s\n", parse->input_stdin, hash);
		else
			fprintf(stdout, "(\"%s\")= %s\n", parse->input_stdin, hash);
	}
	else
	{
		char* format;

		if (parse->opts & O_QUIET_MODE)
			format = "%s\n";
		else if (parse->opts & O_REVERSE_OUT)
			format = "%s stdin\n";
		else
			format = "(stdin)= %s\n";
		fprintf(stdout, format, hash);
	}
}

__always_inline
static void	display_string(command_t* const cmd, result_t const hash, const digest_parse_t* const parse)
{
	if (parse->opts & O_QUIET_MODE)
		fprintf(stdout, "%s\n", hash);
	else if (parse->opts & O_REVERSE_OUT)
		fprintf(stdout, "%s \"%s\"\n", hash, parse->input_str);
	else
		fprintf(stdout, "%s (\"%s\") = %s\n", ft_toupper(cmd->name), parse->input_str, hash);
}

__always_inline
static void	display_file(command_t* const cmd, result_t const hash, const digest_parse_t* const parse)
{
	if (parse->opts & O_QUIET_MODE)
		fprintf(stdout, "%s\n", hash);
	else if (parse->opts & O_REVERSE_OUT)
		fprintf(stdout, "%s %s\n", hash, parse->curr_file);
	else
		fprintf(stdout, "%s (%s) = %s\n", ft_toupper(cmd->name), parse->curr_file, hash);
}

void	digest_display(command_t* const cmd, result_t const hash, display_t source, void* const vparse)
{
	const digest_parse_t parse = *(digest_parse_t*)vparse;

	switch (source)
	{
		case DISPLAY_STDIN:
			display_stdin(cmd, hash, &parse);
			break ;
		case DISPLAY_STRING:
			display_string(cmd, hash, &parse);
			break ;
		case DISPLAY_FILE:
			display_file(cmd, hash, &parse);
			break ;
		default:
			break ;
	}
}
