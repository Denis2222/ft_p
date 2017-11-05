/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   srv_listen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 01:28:58 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 18:22:15 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

int		getproto(void)
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

void	srv_listen_sin(struct sockaddr_in *sin, int port)
{
	sin->sin_family = AF_INET;
	sin->sin_addr.s_addr = INADDR_ANY;
	sin->sin_port = htons(port);
}

void	srv_setsockopt(int sock)
{
	int	optval;

	optval = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	if (SO_NOSIGPIPE)
		osx_pipe(sock);
}

void	srv_listen(t_env *e)
{
	struct sockaddr_in	sin;
	int					sock;

	sock = socket(PF_INET, SOCK_STREAM, getproto());
	if (socket <= 0)
	{
		ft_printf("Erreur socket %d\n", socket);
		exit(1);
	}
	srv_setsockopt(sock);
	srv_listen_sin(&sin, e->port);
	if (bind(sock, (struct sockaddr*)&sin, sizeof(sin)) == -1)
	{
		ft_printf("Port [%d] already in use\n", e->port);
		exit(1);
	}
	if (listen(sock, 42) == -1)
	{
		ft_printf("Error sur listen\n");
		exit(1);
	}
	fd_new(&e->fds[sock], e, FD_SERV, sock);
}
