/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_put.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/31 06:29:52 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/01 21:09:27 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

char	*input_put_check(t_env *e, int s, char *filename)
{
	char	*filepath;
//	int		fd;

	filepath = ft_strnew(PATH_MAX);
	ft_strcat(filepath, e->fds[s].pwd);
	ft_strcat(filepath, "/");
	ft_strcat(filepath, filename);
/*	fd = open(filepath, O_CREAT | O_EXCL);
	if (fd <= 0)
	{
		perror("write()");
		free(filepath);
		return (0);
	}
	close(fd);*/
	return (filepath);
}

int	input_put_launch(t_env *e, int s, int length, char *filename)
{
	int		sd = srv_listen_data(e);
	t_fd	*fd;
	char	*str;

	ft_printf("input_put_launch()\n");
	fd = &e->fds[sd];
	if (!(fd->filepath = input_put_check(e, s, filename)))
		return (0);
	fd->size = length;
	fd->done = 0;
	fd->way = WAYIN;
	fd->parent = s;
	e->fds[s].data = sd;
	
	str = ft_mprintf("dataput:%d:%s:%lld:ready\n", fd->port , filename, fd->size);
	fd_send(&e->fds[s], str);
	return (1);
}

void	input_put(t_env *e, int s, char *cmd)
{
	char	**tab;
	char	**tab2;

	tab = ft_strsplit(cmd, ' ');
	if (ft_tablen(tab) > 1)
	{
		tab2 = ft_strsplit(tab[1], ':');
		if (ft_tablen(tab2) == 2)
		{
			if(input_put_launch(e, s, ft_atoi(tab2[1]), tab2[0]))
			{
				ft_printf("Understand Upload DONE\n");
			}
			else
				printfw(&e->fds[s], "====ERROR file exist or not writable", 0);
		}
		else
			printfw(&e->fds[s], "====ERROR command not found", 0);
	}
	else
		printfw(&e->fds[s], "====ERROR incomplete command", 0);
	ft_tabfree(tab);
}
