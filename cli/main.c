/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 13:46:06 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/04 04:55:11 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

int		checkhost(t_client *client, char *hostname)
{
	struct hostent	*hostinfo;

	hostinfo = NULL;
	hostinfo = gethostbyname(hostname);
	if (hostinfo == NULL)
	{
		ft_printf("        Unknown host\n");
		ft_printf("Unknown host %s.\n", hostname);
		return (1);
	}
	client->hostinfo = hostinfo;
	return (0);
}

int		get_socket_pi(t_client *client)
{
	int	optval;

	optval = 1;
	client->socket_pi = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(client->socket_pi, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	setsockopt(client->socket_pi, SOL_SOCKET, SO_NOSIGPIPE, &optval, sizeof(optval));
	if (client->socket_pi == INVALID_SOCKET)
	{
		ft_printf("socket error \n");
		return (1);
	}
	return (0);
}

int		socket_pi(t_client *client, char *port)
{
	if (port == NULL || ft_atoi(port) <= 0)
	{
		ft_printf("Invalid port number");
		return (1);
	}
	ft_bzero(&(client->sin), sizeof(struct sockaddr_in));
	client->sin.sin_addr = *(struct in_addr *)client->hostinfo->h_addr;
	client->sin.sin_port = htons(ft_atoi(port));
	client->sin.sin_family = AF_INET;
	if (connect(client->socket_pi, (struct sockaddr *)&client->sin,
		sizeof(struct sockaddr)) == SOCKET_ERROR)
	{
		ft_printf("Connection fail : Check host and port !\n");
		return (1);
	}
	client->status_pi = 1;
	ft_printf("Connection established !");
	return (0);
}

int		get_socket_data(t_client *client)
{
	int	optval;

	optval = 1;
	client->socket_data = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(client->socket_data, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	setsockopt(client->socket_data, SOL_SOCKET, SO_NOSIGPIPE, &optval, sizeof(optval));
	if (client->socket_data == INVALID_SOCKET)
	{
		perror("socket()");
		ft_dprintf(2, "socket error \n");
		return (1);
	}
	return (0);
}

int		socket_data(t_client *client, char *port)
{
	if (client->data_way == WAYIN)
		client->data_fd = open(client->data_file, O_CREAT | O_WRONLY, S_IRWXU | S_IRWXG | S_IRWXO);
	if (client->data_way == WAYOUT)
		client->data_fd = open(client->data_file, O_RDONLY);
	if (client->data_fd < 0)
	{
		ft_dprintf(2, "Cant open :%s\n", client->data_file);
		//socket_send(client, "CANCELDATA");
		data_fd_clean(client, 0);
		perror("open()");
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
		data_fd_clean(client, 0);
		return (1);
	}
	writemsg(client, "Connection established !\n");
	return (0);
}

int		connect_pi(char *host, char *port, t_client *client)
{
	if (checkhost(client, host) == 1)
		return (0);
	if (get_socket_pi(client) == 1)
		return (0);
	if (socket_pi(client, port) == 1)
		return (0);
	return (1);
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
	client->prompt = ft_strnew(4096);
	client->ws = malloc(sizeof(t_windows));
	client->ws->scroll = MAX_MSG;
	client->ws->lscroll = MAX_MSG;
	client->ws->lastlscroll = 0;
	client->ws->localls = NULL;;
	client->msg = NULL;
	client->msglocal = NULL;
	client->data_size = -1;
	client->data_do = -1;
	client->data_file = NULL;
	client->data_way = 0;
	make_buffer(&client->lnbuffer);
	client->bw = ft_strnew(4096);
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
