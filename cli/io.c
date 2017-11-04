/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/01 17:36:23 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/04 03:54:26 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

void	data_fd_clean(t_client *c, int sock)
{
	ft_dprintf(2, "data_fd_clean(%d)", sock);
	if (c->data_fd > 0)
		close(c->data_fd);
	if (c->socket_data > 0)
		close(c->socket_data);
	free(c->data_file);
	c->data_file = NULL;
	c->data_do = 0;
	c->data_way = 0;
	c->data_size = 0;
	c->data_fd = 0;
	c->socket_data = 0;
	c->status_data = 0;
}

void	data_read_end(t_client *c, int sock)
{
	writemsg(c, "Transfert recv finish !");
	data_fd_clean(c, sock);
}

void	data_read_fail(t_client *c, int sock)
{
	writemsg(c, "Transfert recv fail !");
	data_fd_clean(c, sock);
}

void	data_write_end(t_client *c, int sock)
{
	writemsg(c, "Transfert send finish !");
	data_fd_clean(c, sock);
}

void	data_write_fail(t_client *c, int sock)
{
	writemsg(c, "Transfert send fail !");
	data_fd_clean(c, sock);
}

void	data_write(t_client *c, int sock)
{
	int		n;
	int		d;
	char	str[BUF_SIZE + 1];

	if (c->status_data)
	{
		n = read(c->data_fd, str, BUF_SIZE);
		if (n > 0 || c->data_size == 0)
		{
			d = send(sock, str, n, 0);
			if (d < 0)
			{
				data_write_fail(c, sock);
			}
			else
				c->data_do += n;
		}
		else
			data_write_fail(c, sock);
		if (c->data_do == c->data_size)
			data_write_end(c, sock);
	}
	else
		n = recv(sock, str, BUF_SIZE, 0);
}

void	data_read(t_client *c, int sock)
{
	int		n;
	int		d;
	char	str[BUF_SIZE + 1];

	if (c->status_data)
	{
		n = recv(sock, str, BUF_SIZE, 0);
		if (n > 0 || (c->data_size == 0))
		{
			d = write(c->data_fd, str, n);
			if (d < 0)
			{
				data_read_fail(c, sock);
			}
			else
				c->data_do += n;
		}
		else
			data_read_fail(c, sock);
		if (c->data_do == c->data_size)
			data_read_end(c, sock);
	}
	else
		n = recv(sock, str, BUF_SIZE, 0);
}

void	socket_read(t_client *c, int sock)
{
	int		n;
	char	*str;

	str = ft_strnew(BUF_SIZE);
	n = recv(sock, str, BUF_SIZE, 0);
	if (n == 0)
	{
		writemsg(c, "Disconnect from server\n");
		client_reset(c);
	}
	else if (n > 0)
	{
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

void	socket_send(t_client *c, char *str)
{
	ft_strcat(c->bw, str);
	ft_strcat(c->bw, "\n");
}
