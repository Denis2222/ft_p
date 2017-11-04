/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/01 17:36:23 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/04 05:53:02 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

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
