/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_fd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 13:58:26 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/04 02:40:32 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

static void	bzero_fd(t_fd *fd)
{
	fd->data = 0;
	fd->way = 0;
	fd->port = 0;
	fd->size = 0;
	fd->done = 0;
	fd->time = 0;
	fd->brh = 0;
	fd->bwh = 0;
	fd->fd = 0;
}

void		clean_fd(t_fd *fd)
{
	if (fd->sock)
		ft_printf("clean_fd(%d)\n", fd->sock);
	fd->type = FD_FREE;
	fd->fct_write = NULL;
	fd->fct_read = NULL;
	if (fd->br)
		free(fd->br);
	fd->br = NULL;
	if (fd->bw)
		free(fd->bw);
	fd->bw = NULL;
	if (fd->pwd)
		free(fd->pwd);
	fd->pwd = NULL;
	close(fd->sock);
	bzero_fd(fd);
}

void		fd_new(t_fd *fd, t_env *e, int type, int sock)
{
	fd->sock = sock;
	fd->type = type;
	if (type == FD_SERV)
		fd->fct_read = srv_accept;
	else if (type == FD_CLIENT)
	{
		fd->fct_read = client_read;
		fd->fct_write = client_write;
	}
	else if (type == FD_DATA)
	{
		fd->fct_read = srv_data_accept;
		fd->fct_write = data_write;
	}
	fd->br = ft_strnew(BUF_SIZE);
	fd->bw = ft_strnew(BUF_SIZE);
	fd->pwd = ft_strnew(PATH_MAX);
	fd->pwd = ft_strcat(fd->pwd, e->pwd);
	bzero_fd(fd);
}

void		fd_init(t_env *e)
{
	int		s;
	t_fd	*fd;

	s = 0;
	e->max = 0;
	FD_ZERO(&e->fd_read);
	FD_ZERO(&e->fd_write);
	while (s < e->maxfd)
	{
		fd = &e->fds[s];
		if (fd->type != FD_FREE)
		{
			FD_SET(s, &e->fd_read);
			if (fd->type == FD_CLIENT && ft_strlen(fd->bw))
				FD_SET(s, &e->fd_write);
			else if (fd->type == FD_DATA && fd->way == WAYOUT)
				FD_SET(s, &e->fd_write);
			e->max = MAX(e->max, s);
		}
		s++;
	}
}

void		fd_check(t_env *e)
{
	int	s;

	s = 0;
	while (s < e->maxfd && e->r > 0)
	{
		if (e->fds[s].type != FD_FREE)
		{
			if (FD_ISSET(s, &e->fd_read) && e->fds[s].fct_read)
			{
				e->fds[s].fct_read(e, s);
				e->r--;
			}
			if (FD_ISSET(s, &e->fd_write) && e->fds[s].fct_write)
			{
				e->fds[s].fct_write(e, s);
				e->r--;
			}
		}
		s++;
	}
}
