/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/30 16:40:15 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/10/31 02:44:13 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

void	socket_read(t_client *c, int sock)
{
	int n;
	char *str;

	str = ft_strnew(4096);
	n = read(sock, str, 4096);
	if (n == 0)
	{
		writemsg(c, "Disconnect from server\n");
		client_reset(c);
	}
	else if (n > 0)
	{
		writemsg(c, str);
		view(c);
	}
}

void	socket_write(t_client *c, int sock)
{
	int n;

	n = write(sock, c->bw, ft_strlen(c->bw));
	if (n == 0)
	{
		writemsg(c, "Disconnect from server\n");
		client_reset(c);
	}
	else if (n > 0)
	{
		if (ft_strlen(c->bw) == n)
			bzero(c->bw, 4096);
		else
			writemsg(c, "Not all send ! Look at socket_write !");
	}
}

int loop(t_client *client)
{
	int	fdmax;

	FD_ZERO(&client->fd_read);
	FD_ZERO(&client->fd_write);

	FD_SET(STDIN_FILENO, &client->fd_read);
	if (client->status_pi == 1)
	{
		FD_SET(client->socket_pi, &client->fd_read);
		if (ft_strlen(client->bw) > 0)
			FD_SET(client->socket_pi, &client->fd_write);
	}
	if (client->status_data == 1)
	{
		//FD_SET(client->socket_data, &client->fd_read);
		//FD_SET(client->socket_data, &client->fd_write);
	}
	

	fdmax = MAX(client->socket_pi, client->socket_data) + 1;
	client->select = select(fdmax, &client->fd_read, &client->fd_write, NULL, NULL);
	if (FD_ISSET(STDIN_FILENO, &client->fd_read))
	{
		prompt_read(client);
	}
	if (client->status_pi)
	{
		if (FD_ISSET(client->socket_pi, &client->fd_read))
		{
			socket_read(client, client->socket_pi);
		}
		if (FD_ISSET(client->socket_pi, &client->fd_write))
		{
			socket_write(client, client->socket_pi);
		}
	}
	if (client->status_data)
	{
		if (FD_ISSET(client->socket_data, &client->fd_read))
		{
			printf("socket_data:read\n");
		}
		if (FD_ISSET(client->socket_data, &client->fd_write))
		{
			printf("socket_data:write\n");
		}
	}
	return (1);
}
