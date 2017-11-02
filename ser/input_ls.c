/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_ls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/30 22:46:00 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/02 10:08:04 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

int little_ls(t_env *e, int cs, char *lsparam)
{
	int link[2];
	pid_t pid;
	char foo[500 + 1];
	int nbchar;

	pipe(link);
	pid = fork();
	if(pid == 0)
	{
		dup2 (link[1], STDOUT_FILENO);
		close(link[0]);
		close(link[1]);
		execl("/bin/ls", "ls", lsparam, (char *)0);
	}
	else
	{
		close(link[1]);
		bzero(foo, 500 + 1);
		while((nbchar = read(link[0], foo, 500)))
		{
			fd_send(&e->fds[cs], foo);
			bzero(foo, 500 + 1);
			ft_printf("Output: %d\n", nbchar);
		}
		wait(NULL);
	}
	return 0;
}

void input_ls(t_env *e, int cs, char *cmd)
{
	char **tab;

	chdir(e->fds[cs].pwd);
	tab = ft_strsplit(cmd, ' ');
	little_ls(e, cs, tab[1]);
	ft_tabfree(tab);
	chdir(e->pwd);
	fd_send(&e->fds[cs], "====SUCCESS");
}

