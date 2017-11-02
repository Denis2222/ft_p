/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/30 16:40:15 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/02 07:54:44 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

int loop(t_client *client, int i)
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
			//ft_dprintf(2, "WAYIN\n");
			FD_SET(client->socket_data, &client->fd_read);
		}
		if (client->data_way == WAYOUT)
		{
			//ft_dprintf(2, "WAYOUT\n");
			FD_SET(client->socket_data, &client->fd_write);
		}
	}
	fdmax = MAX(client->socket_pi, client->socket_data) + 1;
	client->select = select(fdmax, &client->fd_read, &client->fd_write, NULL, NULL);
	if (FD_ISSET(STDIN_FILENO, &client->fd_read))
	{
		if(prompt_read(client))
			return (1);//Resize bug
		view(client);
	}
	if (client->status_pi)
	{
		if (FD_ISSET(client->socket_pi, &client->fd_read))
		{
			socket_read(client, client->socket_pi);
			view(client);
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
			//ft_dprintf(2,"socket_data:read\n");
			data_read(client, client->socket_data);
		}
		if (FD_ISSET(client->socket_data, &client->fd_write))
		{
			//ft_dprintf(2, "socket_data:write\n");
			data_write(client, client->socket_data);
		}
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
