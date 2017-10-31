/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 13:46:06 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/10/31 05:06:06 by dmoureu-         ###   ########.fr       */
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
//		ft_printf("CLI_READ:%s", fd->br);
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
	ft_printf("DATA_READ");
	return (0);
}
int		data_write(t_env *e, int s)
{
	ft_printf("DATA_WRITE");
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
