
#include <ssl_engine.h>
#include <crypt/ssl_sha512.h>

#include <stdlib.h>

__always_inline
static void	display_usage_digest()
{
	fprintf(stdout, "USAGE: " __progname " <digest> [ -pqr ] [ -s <string> ] [ file1[, file2[, ...] ] ]\n");
}

err_t	digest_run(command_t* const cmd, const char** av[])
{
	err_t			st = SUCCESS;
	digest_parse_t	parse = {0};
	u64				ctxbuff[sizeof(sha512_ctx_t)] = {0};
	result_t		res;

	if ((st = cmd->parse(av, &parse)) != SUCCESS)
		goto end;	

	if (parse.opts & O_DISPLAY_HELP)
	{
		display_usage_digest();
		goto end;
	}

	if ((**av == NULL && !(parse.opts & O_INPUT_STRING)) || parse.opts & O_PRINT_STDIN)
	{
		if ((st = compute_stdin(cmd, (void*)ctxbuff, &res, &parse.input_stdin)) == E_SYSCALL)
			goto end;
		else if (st == SUCCESS)
		{
			cmd->display(cmd, res, DISPLAY_STDIN, (void*)&parse);
			free(parse.input_stdin);
			free(res);
		}
	}

	if (parse.opts & O_INPUT_STRING)
	{
		compute_string(cmd, (void*)ctxbuff, parse.input_str, &res);
		cmd->display(cmd, res, DISPLAY_STRING, (void*)&parse);
		free(res);
	}

	for (u64 i = 0 ; (*av)[i] != NULL ; i++)
	{
		parse.curr_file = (*av)[i];
		res = NULL;
		if ((st = compute_file(cmd, (*av)[i], (void*)ctxbuff, &res)) == E_SYSCALL)
			goto end;
		else if (st == SUCCESS)
			cmd->display(cmd, res, DISPLAY_FILE, (void*)&parse);
		free(res);
	}

end:
	return st;
}
