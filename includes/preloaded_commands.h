/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preloaded_commands.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/14 22:08:41 by pablo             #+#    #+#             */
/*   Updated: 2021/08/14 23:28:46 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include "parsers.h"
# include "crypto.h"

# define PROMPT "ft_SSL> "

# define CMD_MD5 "md5"
# define CMD_SHA265 "sha256"

static const t_command g_commands[] = {
	{
		.name = CMD_MD5,
        .parser = &digest_parser,
        .crypto_alg = &md5,
        .display = 0
	},
    {
        .name = CMD_SHA265,
        .parser = &digest_parser,
        .crypto_alg = &sha256,
        .display = 0
    }
};

static const t_command g_nomatch = {0};


