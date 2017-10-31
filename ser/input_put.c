/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_put.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/31 06:29:52 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/10/31 13:56:57 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

char	*input_put_check(t_env *e, int s, char *filename)
{
	char	*filepath;
	int		fd;

	filepath = ft_strnew(PATH_MAX);
	ft_strcat(filepath, e->fds[s].pwd);
	ft_strcat(filepath, "/");
	ft_strcat(filepath, filename);
	fd = open(filepath, O_CREAT | O_EXCL);
	if (fd <= 0)
	{
		perror("write()");
		free(filepath);
		return (0);
	}
	close(fd);
	return (filepath);
}

long long int		input_put_size(t_env *e, int s, char *filepath)
{
	int			fd;
	struct stat	buf;

	fd = open(filepath, O_RDONLY);
	if (fd <= 0)
		return (-1);
	fstat(fd, &buf);
	close(fd);
	return (buf.st_size);
}

void	input_put_launch(t_env *e, int s, char *filepath, char *filename)
{
	int		sd = srv_listen_data(e);
	t_fd	*fd;
	char	*str;

	ft_printf("input_put_launch()\n");
	fd = &e->fds[sd];
	fd->size = input_put_size(e, s, filepath);
	fd->pwd = ft_strcpy(fd->pwd, filepath);
	fd->way = WAYIN;
	fd->parent = s;
	e->fds[s].data = sd;
	
	str = ft_mprintf("dataput:%d:%s:%lld:ready\n", fd->port , filename, fd->size);
	fd_send(&e->fds[s], str);
}

void	input_put(t_env *e, int s, char *cmd)
{
	char **tab;
	char *filepath;
	tab = ft_strsplit(cmd, ' ');
	if (ft_tablen(tab) > 1)
	{
		if((filepath = input_put_check(e, s, tab[1])))
		{
			input_put_launch(e, s, filepath, tab[1]);
			free(filepath);
		}
	}
	ft_tabfree(tab);
}
