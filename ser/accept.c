/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accept.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/01 18:04:33 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/02 02:19:40 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

int		srv_accept(t_env *e, int s)
{
	int					sock;
	struct sockaddr_in	socksin;
	socklen_t			socksin_len;

	socksin_len = sizeof(socksin);
	sock = accept(s, (struct sockaddr *)&socksin, &socksin_len);
	if (sock == -1)
	{
		perror("accept()");
		ft_printf("accept going wrong !\n");
		return (0);
	}
	fd_new(&e->fds[sock], e, FD_CLIENT, sock);
	ft_printf("New client(%d)\n", sock);
	return (0);
}

int		srv_data_accept(t_env *e, int s)
{
	int					sock;
	struct sockaddr_in	socksin;
	socklen_t			socksin_len;
	t_fd				*fd;

	ft_printf("srv_data_accept() !\n");

	socksin_len = sizeof(socksin);
	sock = accept(s, (struct sockaddr *)&socksin, &socksin_len);
	if (sock == -1)
	{
		perror("accept()");
		ft_printf("accept srv_data going wrong !");
		close(s);
		close(sock);
		clean_fd(&e->fds[s]);
		return (0);
	}
	ft_memcpy(&e->fds[sock], &e->fds[s], sizeof(t_fd));
	close(s);
	e->fds[s].type = FD_FREE;
	fd = &e->fds[sock];
	
	if (fd->way == WAYIN)
		fd->fd = open(fd->filepath, O_WRONLY | O_CREAT, 0766);
	if (fd->way == WAYOUT)
		fd->fd = open(fd->filepath, O_RDONLY);

	ft_printf("fd->way:%d!!!!! fd->fd%d | fd->filepath %s\n",fd->way,  fd->fd,  fd->filepath);
	if (fd->fd >0)
	{
		ft_printf("All Right ! open(%d) ok \n", fd->fd);
		fd->fct_write = data_write;
		fd->fct_read = data_read;
		fd_send(&e->fds[fd->parent], "STARTDATA\n");
	}
	else
	{
		perror("open()");
		fd_send(&e->fds[fd->parent], "CANCELDATA\n");
		fd_send(&e->fds[fd->parent], "====ERROR Permission denied\n");
		ft_printf("Probleme de fichier open():%d  path:%s\n", fd->fd, fd->filepath);
	}
	return (1);
}


