/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 13:46:06 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 06:12:45 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

int		get_socket_data(t_client *client)
{
	int	optval;

	optval = 1;
	client->socket_data = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(client->socket_data, SOL_SOCKET,
		SO_REUSEADDR, &optval, sizeof(optval));
	if (SO_NOSIGPIPE)
		osx_pipe(client->socket_data);
	if (client->socket_data == INVALID_SOCKET)
	{
		ft_dprintf(2, "socket error \n");
		return (1);
	}
	return (0);
}

int		socket_data(t_client *client, char *port)
{
	if (client->data_way == WAYIN)
		client->data_fd = open(client->data_file, OPENOPT, OPENPERM);
	if (client->data_way == WAYOUT)
		client->data_fd = open(client->data_file, O_RDONLY);
	if (client->data_fd < 0)
	{
		data_fd_clean(client);
		return (1);
	}
	get_socket_data(client);
	ft_bzero(&(client->sin), sizeof(struct sockaddr_in));
	client->sin.sin_addr = *(struct in_addr *)client->hostinfo->h_addr;
	client->sin.sin_port = htons(ft_atoi(port));
	client->sin.sin_family = AF_INET;
	if (connect(client->socket_data, (struct sockaddr *)&client->sin,
		sizeof(struct sockaddr)) == SOCKET_ERROR)
	{
		writemsg(client, "Connection fail : Check host and port !\n");
		data_fd_clean(client);
		return (1);
	}
	writemsg(client, "Connection established !\n");
	return (0);
}

void	client_init(t_client *client, int ac, char **argv)
{
	(void)ac;
	client->pwd = ft_strnew(PATH_MAX);
	getcwd(client->pwd, PATH_MAX);
	client->status_pi = 0;
	client->status_data = 0;
	client->socket_pi = 0;
	client->socket_data = 0;
	connect_pi(argv[1], argv[2], client);
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
	client->prompt = ft_strnew(4096);
	view(client);
}

int		main(int ac, char **argv)
{
	t_client	client;
	int			i;

	if (!SO_NOSIGPIPE)
		linux_pipe();
	signal(SIGTSTP, signalstop);
	client_init(&client, ac, argv);
	ncurse_init();
	view(&client);
	refresh();
	i = 0;
	while (client.run)
	{
		loop(&client, i);
		i++;
	}
	ncurse_end();
	return (0);
}
