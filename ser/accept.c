/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accept.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/01 18:04:33 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/01 21:31:55 by dmoureu-         ###   ########.fr       */
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
	printf("Two socket: %d %d\n", s, sock);
	ft_memcpy(&e->fds[sock], &e->fds[s], sizeof(t_fd));
	close(s);
	e->fds[s].type = FD_FREE;

	fd = &e->fds[sock];

	printf("pwd in accept:%s\n", fd->filepath);
	fd->fd = open(fd->filepath, O_RDWR | O_CREAT, 0766);
	perror("open()");
	ft_printf("Accepte ou pas bordel fd->way:%d!!!!! fd->fd%d | fd->filepath %s\n",fd->way,  fd->fd,  fd->filepath);
	if (fd->fd >0)
	{
		ft_printf("open(%d) ok on %s\n", fd->fd, fd->filepath);
		fd->mmap = NULL;//mmap(0, fd->size, PROT_WRITE | PROT_READ, MAP_SHARED, fd->fd, 0);
		ft_printf("MMAP: %p \n", fd->mmap);
		ft_printf("All Right\n");
		fd->fct_write = data_write;
		//ft_printf(fd->mmap);
		fd->fct_read = data_read;
		fd_send(&e->fds[fd->parent], "Connection OK! Ready for send \n");
	}
	else
	{
		ft_printf("Probleme de fichier open():%d  path:%s\n", fd->fd, fd->filepath);
	}
	return (1);
}


