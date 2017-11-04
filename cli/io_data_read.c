/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_data_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/04 05:54:12 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/04 07:48:32 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

void	data_read_end(t_client *c, int sock)
{
	data_fd_clean(c, sock);
}

void	data_read_fail(t_client *c, int sock)
{
	data_fd_clean(c, sock);
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
