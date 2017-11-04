/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_connect.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/04 05:59:44 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/04 06:01:26 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

int		checkhost(t_client *client, char *hostname)
{
	struct hostent	*hostinfo;

	hostinfo = NULL;
	hostinfo = gethostbyname(hostname);
	if (hostinfo == NULL)
	{
		ft_printf("        Unknown host\n");
		ft_printf("Unknown host %s.\n", hostname);
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
	setsockopt(client->socket_pi, SOL_SOCKET,
		SO_NOSIGPIPE, &optval, sizeof(optval));
	if (client->socket_pi == INVALID_SOCKET)
	{
		ft_printf("socket error \n");
		return (1);
	}
	return (0);
}

int		socket_pi(t_client *client, char *port)
{
	if (port == NULL || ft_atoi(port) <= 0)
	{
		ft_printf("Invalid port number");
		return (1);
	}
	ft_bzero(&(client->sin), sizeof(struct sockaddr_in));
	client->sin.sin_addr = *(struct in_addr *)client->hostinfo->h_addr;
	client->sin.sin_port = htons(ft_atoi(port));
	client->sin.sin_family = AF_INET;
	if (connect(client->socket_pi, (struct sockaddr *)&client->sin,
		sizeof(struct sockaddr)) == SOCKET_ERROR)
	{
		ft_printf("Connection fail : Check host and port !\n");
		return (1);
	}
	client->status_pi = 1;
	ft_printf("Connection established !");
	return (0);
}

int		connect_pi(char *host, char *port, t_client *client)
{
	if (checkhost(client, host) == 1)
		return (0);
	if (get_socket_pi(client) == 1)
		return (0);
	if (socket_pi(client, port) == 1)
		return (0);
	return (1);
}
