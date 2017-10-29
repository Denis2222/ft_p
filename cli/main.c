/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 13:46:06 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/10/24 13:51:54 by dmoureu-         ###   ########.fr       */
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

int		checksocket(t_client *client)
{
	client->socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client->socket == INVALID_SOCKET)
	{
		ft_printf("socket error \n");
		return (1);
	}
	return (0);
}

int		connectsocket(t_client *client, char *port)
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
	if (connect(client->socket, (struct sockaddr *)&client->sin,
		sizeof(struct sockaddr)) == SOCKET_ERROR)
	{
		ft_printf("Connection fail : Check host and port !\n");
		return (1);
	}
	ft_printf("Connection established !");
	return (0);
}

int		connect_host(char *host, char *port, t_client *client)
{
	if (checkhost(client, host) == 1)
		return (0);
	if (checksocket(client) == 1)
		return (0);
	if (connectsocket(client, port) == 1)
		return (0);
	return (1);
}

int main(int ac, char **argv)
{
	t_client client;
	(void)ac;
	
	printf("client\n");

	connect_host(argv[1], argv[2], &client);


	write(client.socket, "TOTO", 5);
	return (0);
}
