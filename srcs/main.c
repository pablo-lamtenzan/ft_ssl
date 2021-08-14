/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/14 22:08:17 by pablo             #+#    #+#             */
/*   Updated: 2021/08/14 23:28:26 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <preloaded_commands.h>
# include <errors.h>

# include <unistd.h>
# include <stdbool.h>

static t_command *get_command(const char* cmd_name)
{
    size_t  i;
    
    i = -1;
    while (++i < sizeof(g_commands) / sizeof(*g_commands))
        if (g_commands[i].name == cmd_name)
            return (&g_commands[i]);
    return (&g_nomatch);
}

static t_command *read_command()
{
    static const char   prompt[] = PROMPT;
    static char         buff[16];
    size_t              i;

    i = -1;
    write(STDOUT_FILENO, prompt, sizeof(prompt));
    while (++i < (sizeof(buff) / sizeof(*buff)) - 1
    && read(STDIN_FILENO, &buff[i], 1) == 1)
    {
        if (buff[i] == '\n')
        {
            buff[i] = 0;
            break ;
        }
    }
    return (get_command(buff));
}

static t_command *parse_command(const char* cmd_name)
{
    if (cmd_name)
        return (get_command(cmd_name));
    return (read_command());
}

int main(int ac, const char* av[])
{
    const t_command* cmd = parse_command(av[1]);

    if (cmd != &g_nomatch)
        return (throw_error(E_UNKNOWN_CMD, av[1]));
    else if (av[1] == NULL) // If bonus is not enabled must throw an error (E_USAGE)
        ; // execute reading from stdin (bonus)
    else
        ; // regular execution
    return (0);
}
