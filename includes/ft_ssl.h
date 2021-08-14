/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/14 22:08:21 by pablo             #+#    #+#             */
/*   Updated: 2021/08/14 22:18:31 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "errors.h"

# include <inttypes.h>

typedef struct				s_parse
{

}							t_parse;

typedef struct				s_crypto
{

}							t_crypto;

typedef uint8_t             t_opts;
typedef uint8_t *const		t_hash;

typedef						t_err (*const t_parser_func)(t_parse *const);
typedef						const t_hash (*const t_crypto_func)(t_crypto *const);
typedef						void (*const t_format_dispay)(const t_hash hash, t_opts opts, const char *const display_data);


typedef struct				s_command
{
	const char* const		name;
    const t_parser_func		parser;
    const t_crypto_func		crypto_alg;
	const t_format_dispay	display;	
}                   		t_command;

