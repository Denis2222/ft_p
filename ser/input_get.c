/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_get.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/31 06:29:52 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 06:10:49 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

char					*input_get_check(t_env *e, int s, char *filename)
{
	char		*filepath;
	int			fd;
	struct stat	buf;

	filepath = ft_strnew(PATH_MAX);
	ft_strcat(filepath, e->fds[s].pwd);
	ft_strcat(filepath, "/");
	ft_strcat(filepath, filename);
	fd = open(filepath, O_RDONLY);
	if (fd <= 0)
	{
		printfw(&e->fds[s],
			"====ERROR %s File not found OR Not readable\n", filename);
		free(filepath);
		return (0);
	}
	fstat(fd, &buf);
	close_fd(fd);
	if ((buf.st_mode & S_IFMT) != S_IFREG)
	{
		fd_send(&e->fds[s], "====ERROR Special File");
		return (0);
	}
	return (filepath);
}

static long long int	input_get_size(char *filepath)
{
	int			fd;
	struct stat	buf;

	fd = open(filepath, O_RDONLY);
	if (fd <= 0)
		return (-1);
	fstat(fd, &buf);
	close_fd(fd);
	return (buf.st_size);
}

void					input_get_launch(t_env *e, int s,
							char *filepath, char *filename)
{
	int		sd;
	t_fd	*fd;
	char	*str;

	sd = srv_listen_data(e);
	ft_printf("input_get_launch()\n");
	fd = &e->fds[sd];
	fd->size = input_get_size(filepath);
	fd->filepath = ft_strdup(filepath);
	fd->way = WAYOUT;
	fd->parent = s;
	e->fds[s].data = sd;
	str = ft_mprintf("dataget:%d:%s:%lld:ready\n",
		fd->port, filename, fd->size);
	fd_send(&e->fds[s], str);
	free(str);
}

void					input_get(t_env *e, int s, char *cmd)
{
	char	**tab;
	char	*filepath;

	tab = ft_strsplit(cmd, ' ');
	if (ft_tablen(tab) > 1)
	{
		if ((filepath = input_get_check(e, s, tab[1])))
		{
			input_get_launch(e, s, filepath, tab[1]);
			free(filepath);
		}
	}
	ft_tabfree(tab);
}
