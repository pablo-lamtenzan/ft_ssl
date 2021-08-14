/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/14 22:08:28 by pablo             #+#    #+#             */
/*   Updated: 2021/08/14 23:21:44 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# define ERR_USASE_MSG ""
# define ERR_UNKNOWN_INPUT_MSG ""
# define ERR_FILENAME_NO_FOUND ""

typedef enum	e_err
{
	E_USAGE,
	E_UNKNOWN_CMD,
	E_FILENAME_NO_FOUND,
	E_STRING_TO_DO
}				t_err;

typedef			void (*const t_err_msg)(const char*);

t_err			throw_error(t_err err, const char *msg);
