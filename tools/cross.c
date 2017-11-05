/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cross.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/31 09:05:30 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 06:34:04 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cross.h"

void	osx_pipe(int socket)
{
	int	optval;

	optval = 1;
	setsockopt(socket, SOL_SOCKET, SO_NOSIGPIPE, &optval, sizeof(optval));
}

void	linux_pipe(void)
{
	signal(SIGPIPE, SIG_IGN);
}

void	signalstop(int c)
{
	(void)c;
	signal(SIGTSTP, signalstop);
	ft_dprintf(2, "SIGTSTP Catch\n");
}

void	close_fd(int fd)
{
	if (fd == 0)
		return ;
	close(fd);
}
