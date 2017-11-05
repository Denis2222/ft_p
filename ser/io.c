/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/01 18:00:53 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 06:06:52 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

int		client_write(t_env *e, int s)
{
	char	tmp[BUF_SIZE];
	int		n;

	ft_printf("client_write():%d]n", s);
	n = write(s, e->fds[s].bw, ft_strlen(e->fds[s].bw));
	if (n > 0)
	{
		if ((size_t)n < ft_strlen(e->fds[s].bw))
		{
			ft_strcpy(tmp, &e->fds[s].bw[n]);
			ft_bzero(e->fds[s].bw, BUF_SIZE);
			ft_strcpy(e->fds[s].bw, tmp);
			e->fds[s].bwh = ft_strlen(tmp);
		}
		else
		{
			e->fds[s].bwh = 0;
			bzero(e->fds[s].bw, BUF_SIZE);
		}
	}
	else
	{
		clean_fd(&e->fds[s]);
	}
	return (0);
}

int		client_read(t_env *e, int s)
{
	t_fd	*fd;
	int		n;

	ft_printf("client_read():%d]n", s);
	fd = &e->fds[s];
	n = read(s, &fd->br[fd->brh], 4096);
	if (n > 0)
	{
		fd->brh += n;
		fd->br[fd->brh] = '\0';
		input_pi(e, s);
	}
	else if (n <= 0)
	{
		clean_fd(fd);
		return (0);
	}
	return (0);
}
