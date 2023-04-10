
#include <ssl_engine.h>
#include <ssl_utils.h>

#include <string.h> // TODO remove

err_t	parse_digest_cli(const char** av[], void* const res)
{
	digest_parse_t* const	parse = (digest_parse_t*)res;
	err_t					st = SUCCESS;
	u64						i_av = 0;
	bool					found;

	while ((*av)[i_av])
	{
		found = false;
		for (u64 i_opts = 0 ; i_opts < ARRLEN(opts_str) ; i_opts++)
		{
			if (strcmp((*av)[i_av], opts_str[i_opts]) == 0)
			{
				found = true;
				parse->opts |= (1 << i_opts);

				if (i_opts == 2)
				{
					if ((*av)[i_av + 1])
						parse->input_str = (*av)[++i_av];
					else
					{
						FERROR(EFMT_OPTARGNULL, (*av)[i_av]);
						st = E_UNKOWNARGUMENT;
						goto error;
					}
				}
			}
		}
		if (found == false)
		{
			// FERROR(EFMT_OPTNOTFOUND, (*av)[i_av]);
			// st = E_UNKOWNARGUMENT;
			// goto error;
			break ;
		}
		i_av++;
	}
	*av += i_av;

error:
	return st;
}
