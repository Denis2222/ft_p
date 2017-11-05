/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_data.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/04 05:51:34 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 03:39:38 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

void	data_fd_clean(t_client *c, int sock)
{
	//ft_dprintf(2, "data_fd_clean(%d)", sock);
	if (c->data_fd > 0)
	{
		ft_dprintf(2,"Close data_file_fd\n");
		close(c->data_fd);
	}
	if (c->socket_data > 0)
	{
		ft_dprintf(2,"Close data socket\n");
		close(c->socket_data);
	}
	free(c->data_file);
	c->data_file = NULL;
	c->data_do = 0;
	c->data_way = 0;
	c->data_size = 0;
	c->data_fd = 0;
	c->socket_data = 0;
	c->status_data = 0;
}

void	data_write_end(t_client *c, int sock)
{
	data_fd_clean(c, sock);
}

void	data_write_fail(t_client *c, int sock)
{
	data_fd_clean(c, sock);
}

void	data_write(t_client *c, int sock)
{
	int		n;
	int		d;
	char	str[BUF_SIZE + 1];

	//ft_dprintf(2, "data_write()\n");
	if (c->status_data)
	{
		//ft_dprintf(2, "read()\n");
		n = read(c->data_fd, str, BUF_SIZE);
		if (n > 0 || c->data_size == 0)
		{
			//ft_dprintf(2, "avant send()\n");
			d = write(sock, str, n);
			//ft_dprintf(2, "apres send():%d \n", d);
			if (d < 0)
			{
				//ft_dprintf(2, "send fail\n");
				data_write_fail(c, sock);
			}
			else if (d == 0 && c->data_size != c->data_do)
			{
				//ft_dprintf(2, "send fail2\n");
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
	//ft_dprintf(2, "end of write()\n");
}
