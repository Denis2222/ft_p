/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_ls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/30 22:46:00 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 05:48:01 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

void	little_ls_while(t_env *e, int cs, int *link)
{
	int		nbchar;
	char	foo[500 + 1];

	close_fd(link[1]);
	bzero(foo, 500 + 1);
	while ((nbchar = read(link[0], foo, 500)))
	{
		fd_send(&e->fds[cs], foo);
		bzero(foo, 500 + 1);
		ft_printf("Output: %d\n", nbchar);
	}
	wait(NULL);
}

int		little_ls(t_env *e, int cs, char *lsparam)
{
	int		link[2];
	pid_t	pid;

	if (lsparam && ft_strchr(lsparam, 'R'))
	{
		printfw(&e->fds[cs], " -R replace by -a\n", 0);
		ft_strrepchr(lsparam, 'R', 'a');
	}
	pipe(link);
	pid = fork();
	if (pid == 0)
	{
		dup2(link[1], STDOUT_FILENO);
		close(link[0]);
		close(link[1]);
		execl("/bin/ls", "ls", lsparam, (char *)0);
	}
	else
	{
		little_ls_while(e, cs, link);
	}
	return (0);
}

void	input_ls(t_env *e, int cs, char *cmd)
{
	char **tab;

	chdir(e->fds[cs].pwd);
	tab = ft_strsplit(cmd, ' ');
	little_ls(e, cs, tab[1]);
	ft_tabfree(tab);
	chdir(e->pwd);
	fd_send(&e->fds[cs], "====SUCCESS");
}
