/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_data_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 11:09:12 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/04 02:41:58 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

void	data_read_fail(t_env *e, int s)
{
	t_fd	*fd;

	fd = &e->fds[s];
	ft_printf("data_read_fail()\n");
	printfw(&e->fds[fd->parent],
		"====ERROR Upload %s Fail\n", fd->filepath);
	data_fd_clean(fd);
}

void	data_read_success(t_env *e, int s)
{
	t_fd	*fd;

	ft_printf("data_read_success()\n");
	fd = &e->fds[s];
	printfw(&e->fds[fd->parent],
		"====SUCCESS Upload %s Complete\n", fd->filepath);
	calcspeed(e, fd);
	data_fd_clean(fd);
}

int		data_read(t_env *e, int s)
{
	t_fd	*fd;
	int		n;
	char	str[BUF_SIZE + 1];

	fd = &e->fds[s];
	n = read(s, str, BUF_SIZE);
	if (n > 0 || fd->size == 0)
	{
		write(fd->fd, str, n);
		fd->done += n;
	}
	else if (fd->size != fd->done)
		data_read_fail(e, s);
	if (fd->size == fd->done)
	{
		ft_printf("Transfert complete\n");
		data_read_success(e, s);
	}
	return (0);
}
