/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   srv_listen_data.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 17:51:13 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 18:16:48 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

int			srv_listen_data_bind_listen(
								struct sockaddr_in *sin, int port, int sock)
{
	srv_listen_sin(sin, port);
	srv_setsockopt(sock);
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
