/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/30 21:43:39 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/02 12:48:00 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

void			input_process(t_env *e, int sock, char *cmd)
{
	char	**tab;

	printfw(&e->fds[sock], "[CMD]>%s\n", cmd);
	if (ft_strncmp(cmd, "ls", 2) == 0)
		input_ls(e, sock, cmd);
	else if (ft_strncmp(cmd, "cd", 2) == 0)
	{
		tab = ft_strsplit(cmd, ' ');
		if (ft_tablen(tab) > 1)
			input_cd(e, sock, tab[1]);
		ft_tabfree(tab);
		return ;
	}
	else if (ft_strncmp(cmd, "get ", 4) == 0)
		input_get(e, sock, cmd);
	else if (ft_strncmp(cmd, "put ", 4) == 0)
		input_put(e, sock, cmd);
	else if (ft_strncmp(cmd, "CANCELDATA", 10) == 0)
	{
		ft_printf("GET CANCELDATA");
		int i = 0;
		while (i < e->maxfd)
		{
			if (e->fds[i].parent == sock)
				data_fd_clean(&e->fds[i]);
			i++;
		}
	}
	else
		printfw(&e->fds[sock], "====ERROR Command not found !\n", cmd);
}

static char		*get_next_cmd(char *buffer)
{
	int		length;
	int		cmdlength;
	int		pos;
	char	*cmd;
	char	*tmp;

	if (!ft_strchr(buffer, '\n'))
		return (NULL);
	length = ft_strlen(buffer);
	pos = ft_strlen(ft_strchr(buffer, '\n'));
	cmdlength = length - pos;
	cmd = ft_strsub(buffer, 0, cmdlength);
	tmp = ft_strsub(buffer, cmdlength + 1, length);
	ft_bzero(buffer, BUF_SIZE + 1);
	ft_strcpy(buffer, tmp);
	free(tmp);
	return (cmd);
}

void			input_pi(t_env *e, int i)
{
	char	*cmd;

	cmd = NULL;
	cmd = get_next_cmd(e->fds[i].br);
	if (cmd)
	{
		e->fds[i].brh = 0;
		ft_bzero(e->fds[i].br, BUF_SIZE);
		input_process(e, i, cmd);
		free(cmd);
	}
}
