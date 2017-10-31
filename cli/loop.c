/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/30 16:40:15 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/10/31 13:13:13 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

void	data_read_end(t_client *c, int sock)
{
	close(c->data_fd);
	writemsg(c, "Transfert finish !");
	free(c->data_file);
	c->data_file = NULL;
	c->data_do = 0;
	c->data_way = 0;
	c->data_size = 0;
	c->data_fd = 0;
	c->socket_data = 0;
}

void	data_read_fail(t_client *c, int sock)
{
	close(c->data_fd);
	writemsg(c, "Transfert fail !");
	free(c->data_file);
	c->data_file = NULL;
	c->data_do = 0;
	c->data_way = 0;
	c->data_size = 0;
	c->data_fd = 0;
	c->socket_data = 0;
}

void	data_read(t_client *c, int sock)
{
	int	n;
	int	d;
	char str[BUF_SIZE+1];
		n = recv(sock, str, BUF_SIZE, 0);
		ft_dprintf(2, "data_read():%d\n", n);
		if (n > 0)
		{
			if (c->status_data)
			{
				d = write(c->data_fd, str, n);
				if (d < 0)
				{
					ft_dprintf(2, "erreur data_read write\n");
					data_read_fail(c, sock);
				}
				else
				{
					c->data_do += n;
				}
			}
		}
		else
		{
			data_read_fail(c, sock);
			ft_dprintf(2, "erreur data_read read\n");
		}
		if (c->data_do == c->data_size)
		{
			data_read_end(c, sock);
		}
}

void	socket_read(t_client *c, int sock)
{
	int n;
	char *str;

	str = ft_strnew(4096);
	n = recv(sock, str, 4096, 0);
	if (n == 0)
	{
		writemsg(c, "Disconnect from server\n");
		client_reset(c);
	}
	else if (n > 0)
	{
		writemsg(c, str);
		input(c, sock, str);
		view(c);
	}
}

void	socket_write(t_client *c, int sock)
{
	int n;

	n = send(sock, c->bw, ft_strlen(c->bw), 0);
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
		if (client->data_way == WAYIN)
		{
			ft_dprintf(2, "WAYIN\n");
			FD_SET(client->socket_data, &client->fd_read);
		}
		if (client->data_way == WAYOUT)
		{
			ft_dprintf(2, "WAYOUT\n");
			FD_SET(client->socket_data, &client->fd_write);
		}
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
			ft_dprintf(2,"socket_data:read\n");
			data_read(client, client->socket_data);
		}
		if (FD_ISSET(client->socket_data, &client->fd_write))
		{
			ft_dprintf(2, "socket_data:write\n");
//			socket_(client, client->socket_data);
		}
	}
	return (1);
}
