/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_data_write.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 11:05:59 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 03:08:30 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

void	data_write_fail(t_env *e, int s)
{
	t_fd	*fd;

	fd = &e->fds[s];
	ft_printf("data_write_fail()\n");
	printfw(&e->fds[fd->parent],
		"====ERROR Download %s Fail\n", fd->filepath);
	data_fd_clean(fd);
}

void	data_write_success(t_env *e, int s)
{
	t_fd	*fd;

	ft_printf("data_write_end()\n");
	fd = &e->fds[s];
	printfw(&e->fds[fd->parent],
		"====SUCCESS Download %s Complete\n", fd->filepath);
	calcspeed(e, fd);
	data_fd_clean(fd);
}

int		data_write(t_env *e, int s)
{
	int		n;
	t_fd	*fd;
	int		tosend;
	char	str[BUF_SIZE];

	fd = &e->fds[s];
	tosend = fd->size - fd->done;
	tosend = MIN(tosend, BUF_SIZE);
	tosend = read(fd->fd, str, tosend);
	n = write(s, str, tosend);
	if (n > 0 || fd->size == 0)
	{
		fd->done += n;
		if (fd->done == fd->size)
			data_write_success(e, s);
	}
	else if (n == 0)
	{
		ft_printf("Fini Ou Deco !");
		data_write_fail(e, s);
	}
	else if (n < 0)
	{
		data_write_fail(e,s);
		ft_printf("Plus de place");
	}
	return (0);
}
