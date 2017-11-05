/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_data.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/04 05:51:34 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 07:48:53 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

void	data_fd_clean(t_client *c)
{
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

void	data_write_end(t_client *c)
{
	data_fd_clean(c);
}

void	data_write_fail(t_client *c)
{
	data_fd_clean(c);
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
			d = write(sock, str, n);
			if (d < 0)
				data_write_fail(c);
			else if (d == 0 && c->data_size != c->data_do)
				data_write_fail(c);
			else
				c->data_do += n;
		}
		else
			data_write_fail(c);
		if (c->data_do == c->data_size)
			data_write_end(c);
	}
	else
		n = recv(sock, str, BUF_SIZE, 0);
}
