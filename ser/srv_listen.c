/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   srv_listen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 01:28:58 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 11:43:40 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

static int	getproto(void)
{
	struct protoent	*pe;

	pe = getprotobyname("tcp");
	if (!pe)
	{
		ft_printf("Erreur getprotobyname\n");
		exit(1);
	}
	return (pe->p_proto);
}

void		srv_listen_sin(struct sockaddr_in *sin, int port)
{
	sin->sin_family = AF_INET;
	sin->sin_addr.s_addr = INADDR_ANY;
	sin->sin_port = htons(port);
}

int			srv_listen_data_bind_listen(
								struct sockaddr_in *sin, int port, int sock)
{
	int	optval;

	optval = 1;
	srv_listen_sin(sin, port);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	if (SO_NOSIGPIPE)
		osx_pipe(sock);
	ft_printf("New listen socket_data ![%d]\n", sock);
	if (bind(sock, (struct sockaddr*)sin, sizeof(struct sockaddr_in)) == -1)
	{
		ft_printf("coninue next\n");
		return (0);
	}
	if (listen(sock, 42) == -1)
	{
		ft_printf("Error sur listen\n");
		return (0);
	}
	return (1);
}

int			srv_listen_data(t_env *e)
{
	int					bindport;
	int					sock;
	int					port;
	struct sockaddr_in	sin;

	port = e->port;
	bindport = 1;
	while (bindport)
	{
		port++;
		sock = socket(PF_INET, SOCK_STREAM, getproto());
		if (sock <= 0)
		{
			ft_printf("erreur socket()\n");
			return (0);
		}
		if (!srv_listen_data_bind_listen(&sin, port, sock))
			continue;
		bindport = 0;
	}
	ft_printf("Port bind & listen on %d\n", port);
	fd_new(&e->fds[sock], e, FD_DATA, sock);
	e->fds[sock].port = port;
	return (sock);
}

void		srv_listen(t_env *e)
{
	struct sockaddr_in	sin;
	int					sock;
	int					optval;

	optval = 1;
	sock = socket(PF_INET, SOCK_STREAM, getproto());
	if (socket <= 0)
	{
		ft_printf("Erreur socket %d\n", socket);
		exit(1);
	}
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	if (SO_NOSIGPIPE)
		osx_pipe(sock);
	srv_listen_sin(&sin, e->port);
	if (bind(sock, (struct sockaddr*)&sin, sizeof(sin)) == -1)
	{
		exit(1);
	}
	if (listen(sock, 42) == -1)
	{
		ft_printf("Error sur listen\n");
		exit(1);
	}
	fd_new(&e->fds[sock], e, FD_SERV, sock);
}
