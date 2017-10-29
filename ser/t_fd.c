/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_fd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 13:58:26 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/10/24 14:01:29 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

void	clean_fd(t_fd *fd)
{
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
}

void	fd_new(t_fd *fd, t_env *e, int type)
{
	fd->type = type;
	if (type == FD_SERV)
	{
		fd->fct_read = srv_accept;
	}
	else if (type == FD_CLIENT)
	{
		fd->fct_read = client_read;
		fd->fct_write = client_write;
	}
	else if (type == FD_DATA)
	{
		fd->fct_read = data_read;
		fd->fct_write = data_write;
	}
	fd->br = ft_strnew(BUF_SIZE);
	fd->bw = ft_strnew(BUF_SIZE);
	fd->brh = 0;
	fd->bwh = 0;
	fd->pwd = ft_strdup(e->pwd);
}
