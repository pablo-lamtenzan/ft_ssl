/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/14 23:22:04 by pablo             #+#    #+#             */
/*   Updated: 2021/08/14 23:22:31 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <errors.h>
# include <sys/types.h>
# include <stdio.h>

static void	display_err_usage(const char *input)
{
	(void)input;
	printf(ERR_USASE_MSG);
}

static void display_err_unknown_cmd(const char *input)
{
	printf(ERR_UNKNOWN_INPUT_MSG, input);
}

static void display_err_filename_no_found(const char *input)
{
	printf(ERR_FILENAME_NO_FOUND, input);
}

static void display_err_string_TODO(const char *input)
{
	(void)input;
}

t_err	throw_error(t_err err, const char *msg)
{
	size_t				i;
	static const t_err	error_nbs[] = {
		E_USAGE,
		E_UNKNOWN_CMD,
		E_FILENAME_NO_FOUND,
		E_STRING_TO_DO
	};
	static const t_err_msg err_msgs[] = {
		&display_err_usage,
		&display_err_unknown_cmd,
		&display_err_filename_no_found,
		&display_err_string_TODO
	};

	i = -1;
    while (i < sizeof(error_nbs) / sizeof(*error_nbs))
        if (error_nbs[i] == err)
		{
			err_msgs[i](msg);
			break ;
		}
    return (err);
}
