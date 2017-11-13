/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_connect.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/04 05:59:44 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/13 16:59:49 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

int		checkhost(t_client *client, char *hostname)
{
	struct hostent	*hostinfo;
	char			*str;

	hostinfo = NULL;
	hostinfo = gethostbyname(hostname);
	if (hostinfo == NULL)
	{
		str = ft_mprintf("Unknow host %s", hostname);
		writemsg(client, str);
		free(str);
		return (1);
	}
	client->hostinfo = hostinfo;
	return (0);
}

int		get_socket_pi(t_client *client)
{
	int	optval;

	optval = 1;
	client->socket_pi = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(client->socket_pi, SOL_SOCKET,
		SO_REUSEADDR, &optval, sizeof(optval));
	if (SO_NOSIGPIPE)
		osx_pipe(client->socket_data);
	if (client->socket_pi == INVALID_SOCKET)
	{
		ft_printf("socket error \n");
		return (1);
	}
	return (0);
}

int		socket_pi(t_client *client, int port)
{
	ft_bzero(&(client->sin), sizeof(struct sockaddr_in));
	client->sin.sin_addr = *(struct in_addr *)client->hostinfo->h_addr;
	client->sin.sin_port = htons(port);
	client->sin.sin_family = AF_INET;
	if (connect(client->socket_pi, (struct sockaddr *)&client->sin,
		sizeof(struct sockaddr)) == SOCKET_ERROR)
	{
		writemsg(client, "Connection fail : Check host and port !");
		return (1);
	}
	client->status_pi = 1;
	writemsg(client, "Connection established !");
	return (0);
}

int		connect_pi(int ac, char **argv, t_client *client)
{
	int		port;
	char	*str;

	port = 2000;
	if (ac == 1)
		client->host = ft_strdup("localhost");
	if (ac > 1)
		client->host = ft_strdup(argv[1]);
	if (ac > 2)
		port = ft_atoi(argv[2]);
	if (port < 1024 || port > 65000)
		port = 2000;
	str = ft_mprintf("Connection to %s:%d", client->host, port);
	writemsg(client, str);
	free(str);
	if (checkhost(client, client->host) == 1)
		return (0);
	if (get_socket_pi(client) == 1)
		return (0);
	if (socket_pi(client, port) == 1)
		return (0);
	return (1);
}
