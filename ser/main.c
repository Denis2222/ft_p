/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 13:46:06 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/10/31 13:44:25 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"
#include <netinet/in.h>
#include <arpa/inet.h>

void	do_select(t_env *e)
{
	struct timeval tv;
	tv.tv_sec = 3;
	tv.tv_usec = 0;
	e->r = select(e->max + 1, &e->fd_read, &e->fd_write, NULL, NULL);
}

int main(int ac, char **argv)
{
	(void)ac;
	(void)argv;
	
	t_env e;

	env_init(&e);
	srv_listen(&e, ft_atoi(argv[1]));

	ft_printf("FT_PD ready on port [%d]:\n", e.port);

	while (1)
	{
		fd_init(&e);
		do_select(&e);
		fd_check(&e);
	}
	return (0);
}

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
	printf("pwd in accept:%s\n", fd->pwd);
	fd->fd = open(fd->pwd, O_RDONLY);
	fd->mmap = mmap(0, fd->size, PROT_READ, MAP_SHARED, fd->fd, 0);
	printf("All Right\n");
	fd->fct_write = data_write;
	//ft_printf(fd->mmap);
	fd->fct_read = data_read;

	fd_send(&e->fds[fd->parent], "Connection OK! Ready for send \n");
	return (1);
}

int		client_write(t_env *e, int s)
{
	ft_printf("CLI_WRITE");
	char tmp[BUF_SIZE];
	int n;

	n = write(s, e->fds[s].bw, ft_strlen(e->fds[s].bw));
	if (n > 0)
	{
		if (n < ft_strlen(e->fds[s].bw))
		{
			ft_strcpy(tmp, &e->fds[s].bw[n]);
			ft_bzero(e->fds[s].bw, BUF_SIZE);
			ft_strcpy(e->fds[s].bw, tmp);
			e->fds[s].bwh = ft_strlen(tmp);
		}
		else
		{
			e->fds[s].bwh = 0;
			bzero(e->fds[s].bw, BUF_SIZE);
		}
	}
	else
	{
		clean_fd(&e->fds[s]);
	}
	return (0);
}

int		client_read(t_env *e, int s)
{
	t_fd *fd;

	fd = &e->fds[s];

	int n = read(s, &fd->br[fd->brh], 4096);
	if (n > 0)
	{
		fd->brh += n;
		fd->br[fd->brh] = '\0';
		input_pi(e, s);
	}
	else if (n == 0)
	{
		clean_fd(fd);
		return (0);
	}
	return (0);
}

int		data_read(t_env *e, int s)
{
	t_fd	*fd;
	int		n;
	char	str[BUF_SIZE + 1];
	
	fd = &e->fds[s];
	
	
	n = recv(s, str, BUF_SIZE, 0);

	ft_printf("data_read():%d\n", n);
	return (0);
}

void	data_write_fail(t_env *e, int s)
{
	t_fd	*fd;

	fd = &e->fds[s];
	ft_printf("data_write_fail()\n");
	fd->way = 0;
	close(fd->sock);
	clean_fd(fd);
}

void	data_write_end(t_env *e, int s)
{
	t_fd	*fd;

	ft_printf("data_write_end()\n");
	fd = &e->fds[s];

	fd->way = 0;
	close(fd->sock);
	clean_fd(fd);
}

int		data_write(t_env *e, int s)
{
	int		n;
	t_fd	*fd;
	int		to;

	fd = &e->fds[s];
	to = fd->size - fd->done;
	to = MIN(to, BUF_SIZE);
	//ft_printf("data_write():%d, fd:%d\n",to, s);
	n = write(s, fd->mmap + fd->done, to);
	if (n>0)
	{
		fd->done += n;
	}
	else if (n == 0)
	{
		perror("write()");
		ft_printf("Fini Ou Deco !");
		data_write_fail(e, s);
	}
	if (fd->done == fd->size)
		data_write_end(e, s);
	return (0);
}

void	printfd(t_fd *fd, char *format, long long int data)
{
	char *str;

	if (format)
		if (ft_strlen(format))
		{
		//	dprintf(fd->sock, format, data);
			str = ft_mprintf(format, data);
			fd_send(fd, str);
			free(str);
		}
}

void	printfw(t_fd *fd, char *format, void *data)
{
	char *str;
	
	if (format)
		if (ft_strlen(format))
		{
			//dprintf(fd->sock, format, data);
			str = ft_mprintf(format, data);
			fd_send(fd, str);
			free(str);
		}
}
