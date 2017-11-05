/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 09:06:42 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 11:36:46 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

void	client_init(t_client *client)
{
	client->pwd = ft_strnew(PATH_MAX);
	getcwd(client->pwd, PATH_MAX);
	client->status_pi = 0;
	client->status_data = 0;
	client->socket_pi = 0;
	client->socket_data = 0;
	client->run = 1;
	client->prompt = ft_strnew(BUF_SIZE + 1);
	client->ws = malloc(sizeof(t_windows));
	client->ws->scroll = MAX_MSG;
	client->ws->lscroll = MAX_MSG;
	client->ws->lastlscroll = 0;
	client->ws->localls = NULL;
	client->msg = NULL;
	client->msglocal = NULL;
	client->data_size = -1;
	client->data_do = -1;
	client->data_file = NULL;
	client->data_way = 0;
	make_buffer(&client->lnbuffer);
	client->bw = ft_strnew(BUF_SIZE + 1);
}

void	client_reset(t_client *client)
{
	client->status_pi = 0;
	client->status_data = 0;
	client->socket_pi = 0;
	client->socket_data = 0;
	client->run = 1;
	free(client->prompt);
	client->prompt = ft_strnew(PROMPT_SIZE_MAX);
	view(client);
}
