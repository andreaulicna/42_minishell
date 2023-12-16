/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulicna <aulicna@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 10:23:00 by aulicna           #+#    #+#             */
/*   Updated: 2023/12/16 13:07:52 by aulicna          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void    create_heredoc(t_list *heredoc, char *hd_file_name, t_list *env_list)
{
    int fd;
    char    *line;
    t_lexer *content_heredoc;
    t_simple_cmds   *node;

    fd = open(hd_file_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
    line = readline("> ");
    content_heredoc = (t_lexer *) heredoc->content;
    while (line && ft_strncmp(content_heredoc->word, line, ft_strlen(content_heredoc->word)))
    {
        if (contains_dollar(line))
        {
            node = malloc(sizeof(t_lexer));
            node->cmd = ft_calloc(2, sizeof(char *));
            node->cmd[0] = line;
            node->cmd[1] = NULL;
            node->redirects = NULL;
            node->hd_file = NULL;
            expander_loop_dollar(node, 0, env_list);
            ft_putstr_fd(node->cmd[0], fd);
            free_array(node->cmd);
            free(node);
        }
        else
        {
            ft_putstr_fd(line, fd);
            free(line);
        }
        ft_putstr_fd("\n", fd);
        line = readline("> ");
    }
    free(line);
    close(fd);
}

char    *get_hd_file_name(void)
{
    static int  i;
    char    *str_i;
    char    *hd_file_name;

    str_i = ft_itoa(i++);
    hd_file_name = ft_strjoin("./src/heredoc/.tmp_files/.tmp_heredoc_", str_i);
    free(str_i);
    return (hd_file_name);
}

void heredoc(t_list *simple_cmd, t_list *env_list)
{
    t_simple_cmds *content_simple_cmd;
    t_lexer *content_redirect;
    t_list  *current_redirect;

    content_simple_cmd = (t_simple_cmds *) simple_cmd->content;
    current_redirect  = content_simple_cmd->redirects;
    while (current_redirect)
    {
        content_redirect = (t_lexer *) current_redirect->content;
        if (content_redirect->token == LESS_2)
        {
            if (content_simple_cmd->hd_file)
                free(content_simple_cmd->hd_file);
            content_simple_cmd->hd_file = get_hd_file_name();
            create_heredoc(current_redirect, content_simple_cmd->hd_file, env_list);
            free(content_simple_cmd->hd_file);
        }
        current_redirect = current_redirect->next;
    }
}

int executor(t_data *data)
{
    t_list *current;

    current = data->simple_cmds;
    while (current)
    {
        heredoc(current, data->env_list);
        current = current->next;
    }
    return (0);
}