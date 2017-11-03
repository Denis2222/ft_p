/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/30 16:40:15 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/02 13:00:14 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

int	loop(t_client *client, int i)
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
			FD_SET(client->socket_data, &client->fd_read);
		if (client->data_way == WAYOUT)
			FD_SET(client->socket_data, &client->fd_write);
	}
	fdmax = MAX(client->socket_pi, client->socket_data) + 1;
	client->select = select(fdmax, &client->fd_read, &client->fd_write, NULL, NULL);
	//ft_dprintf(2, "select():%d", client->select);
	if (FD_ISSET(STDIN_FILENO, &client->fd_read))
	{
		//ft_dprintf(2, "isset stdin");
		if (prompt_read(client))
			return (1);
		view(client);
	}
	if (client->status_pi)
	{
		if (FD_ISSET(client->socket_pi, &client->fd_read))
		{
			//ft_dprintf(2, "client->status_pi read");
			socket_read(client, client->socket_pi);
			view(client);
		}
		if (FD_ISSET(client->socket_pi, &client->fd_write))
		{
			ft_dprintf(2, "pi write");
			socket_write(client, client->socket_pi);
		}
	}
	if (client->status_data)
	{
		//ft_dprintf(2, "client->status_data actif");
		if (FD_ISSET(client->socket_data, &client->fd_read))
			data_read(client, client->socket_data);
		if (FD_ISSET(client->socket_data, &client->fd_write))
			data_write(client, client->socket_data);
	}
	if (client->status_data && i % 10 == 0)
	{
		view_info(client);
		wrefresh(client->ws->info);
	}
	else if (!client->status_data)
		view(client);
	return (1);
}
