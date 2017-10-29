/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 13:46:06 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/10/24 13:56:57 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"
#include <netinet/in.h>
#include <arpa/inet.h>


void	init_fd(t_env *e)
{
	int	s;
	t_fd *fd;

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
			{
				FD_SET(s, &e->fd_write);
			}
			e->max = MAX(e->max, s);
		}
		s++;
	}
}

void	do_select(t_env *e)
{
	e->r = select(e->max + 1, &e->fd_read, &e->fd_write, NULL, NULL);
}

void	check_fd(t_env *e)
{
	int	s;

	s = 0;
	while (s < e->maxfd && e->r > 0)
	{
		if (e->fds[s].type != FD_FREE)
		{
			if (FD_ISSET(s, &e->fd_read))
			{
				e->fds[s].fct_read(e, s);
				e->r--;
			}
			if (FD_ISSET(s, &e->fd_write))
			{
				e->fds[s].fct_write(e, s);
				e->r--;
			}
		}
		s++;
	}
}

int main(int ac, char **argv)
{
	printf("server\n");
	(void)ac;
	(void)argv;
	
	t_env e;

	env_init(&e);
	srv_listen(&e, ft_atoi(argv[1]));

	ft_printf("FT_PD ready on port [%d]:\n", e.port);

	while (1)
	{
		ft_printf("Do start");
		init_fd(&e);
		do_select(&e);
		check_fd(&e);
		ft_printf("Do finis");
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
	fd_new(&e->fds[sock], e, FD_CLIENT);
	
	write(sock, "Wololo\n", 8);
	ft_printf("SRV_ACCEPT");

	return (0);
}

int		client_write(t_env *e, int s)
{
	ft_printf("CLI_WRITE");
	return (0);
}
int		client_read(t_env *e, int s)
{
	t_fd *fd;

	fd = &e->fds[s];

	int n = read(s, &fd->bw[fd->brh], 4096);
	if (n > 0)
	{
		fd->brh += n;
	}
	else if (n == 0)
	{
		clean_fd(fd);
		return (0);
	}
	ft_printf("CLI_READ:%s", fd->bw);
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

