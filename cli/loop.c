/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/30 16:40:15 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 17:47:52 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

static void	loop_fd_set(t_client *client)
{
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
}

static int	loop_fd_isset(t_client *client, int i)
{
	if (client->status_pi)
	{
		if (FD_ISSET(client->socket_pi, &client->fd_read))
		{
			socket_read(client, client->socket_pi);
			view(client);
		}
		if (FD_ISSET(client->socket_pi, &client->fd_write))
			socket_write(client, client->socket_pi);
	}
	if (client->status_data)
	{
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

int			loop(t_client *client, int i)
{
	int	fdmax;

	loop_fd_set(client);
	fdmax = MAX(client->socket_pi, client->socket_data) + 1;
	client->select = select(fdmax, &client->fd_read,
		&client->fd_write, NULL, NULL);
	if (FD_ISSET(STDIN_FILENO, &client->fd_read))
	{
		if (prompt_read(client))
			return (1);
		view(client);
	}
	return (loop_fd_isset(client, i));
}
