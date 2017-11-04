/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_data.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 11:13:25 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/04 07:46:38 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

void	calcspeed(t_env *e, t_fd *fd)
{
	char			*str;
	unsigned long	now;
	unsigned long	octetsbysec;

	now = time(NULL) - fd->time;
	if (now > 0)
	{
		octetsbysec = fd->size / now;
		str = ft_mprintf("Time %d sec \nSpeed : %ldko/s\n%ld o/s\n",
								now, octetsbysec / 1024, octetsbysec);
		printfw(&e->fds[fd->parent], "%s\n", str);
		free(str);
	}
	else
	{
		octetsbysec = fd->size;
		str = ft_mprintf("Time %d sec \nSpeed : %ldko/s\n%ld o/s\n",
								now, octetsbysec / 1024, octetsbysec);
		printfw(&e->fds[fd->parent], "%s\n", str);
		free(str);
	}
}

void	data_fd_clean(t_fd *fd)
{
	fd->way = 0;
	close(fd->sock);
	close(fd->fd);
	clean_fd(fd);
}
