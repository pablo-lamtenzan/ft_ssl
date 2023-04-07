
#include <ssl_engine.h>
#include <ssl_utils.h>

#include <string.h> // TODO: remove

err_t	select_command(const char** cmd_input_name[], command_t** dest)
{
	const char* const cmd_names[] = {
		CMD_NAME_MD5,
		CMD_NAME_SHA256,
		CMD_NAME_SHA512
	};

	err_t				st = SUCCESS;
	command_t*			found = NULL;

	for (u64 i = 0 ; i < ARRLEN(cmd_names) ; i++)
	{
		if (strcmp(**cmd_input_name, cmd_names[i]) == 0)
		{
			found = (command_t*)&commands[i];
			break ;
		}
	}
	if (found == NULL)
	{
		st = E_UNKOWNARGUMENT;
		FERROR(EFMT_CMDNOTFOUND, **cmd_input_name);
	}
	else
	{
		*dest = found;
		++(*cmd_input_name);
	}
	return st;
}
