/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/01 18:00:53 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/02 06:09:42 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

void	calcspeed(t_env *e, t_fd *fd)
{
	char 			*str;
	unsigned long	now = time(NULL);
	unsigned long	octetsbysec;

	now = now - fd->time;
	if (now>0)
	{
		octetsbysec = fd->size / now;
		str = ft_mprintf("Time %d sec \nSpeed : %ldko/s\n%ld o/s\n", now, octetsbysec/1024, octetsbysec);
		printfw(&e->fds[fd->parent], "%s\n", str);
		free(str);
	}
}

int		client_write(t_env *e, int s)
{
	char tmp[BUF_SIZE];
	int n;

	n = write(s, e->fds[s].bw, ft_strlen(e->fds[s].bw));
	if (n > 0)
	{
		if (n < ft_strlen(e->fds[s].bw))
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
	t_fd *fd;

	fd = &e->fds[s];

	int n = read(s, &fd->br[fd->brh], 4096);
	if (n > 0)
	{
		fd->brh += n;
		fd->br[fd->brh] = '\0';
		input_pi(e, s);
	}
	else if (n == 0)
	{
		clean_fd(fd);
		return (0);
	}
	return (0);
}

void	data_fd_clean(t_fd *fd)
{
	fd->way = 0;
	close(fd->sock);
	close(fd->fd);
	clean_fd(fd);
}

void	data_read_fail(t_env *e, int s)
{
	t_fd	*fd;

	fd = &e->fds[s];
	ft_printf("data_read_fail()\n");
	printfw(&e->fds[fd->parent], "====ERROR Upload %s Fail\n", fd->filepath);
	data_fd_clean(fd);
}

void	data_read_success(t_env *e, int s)
{
	t_fd	*fd;

	ft_printf("data_read_success()\n");
	fd = &e->fds[s];
	printfw(&e->fds[fd->parent], "====SUCCESS Upload %s Complete\n", fd->filepath);
	calcspeed(e, fd);
	data_fd_clean(fd);
}

int		data_read(t_env *e, int s)
{
	t_fd	*fd;
	int		n;
	char	str[BUF_SIZE + 1];
	
	fd = &e->fds[s];
	
	n = recv(s, str, BUF_SIZE, 0);
	//ft_printf("data_read():%d\n", n);
	if (n > 0)
	{
		write(fd->fd, str, n);
		fd->done += n;
		//ft_printf("Write %d octets in file", n);
	}
	else if(fd->size != fd->done)
	{
		data_read_fail(e, s);
	}

	if (fd->size == fd->done)
	{
		ft_printf("Transfert complete\n");
		data_read_success(e, s);
	}
	return (0);
}

void	data_write_fail(t_env *e, int s)
{
	t_fd	*fd;

	fd = &e->fds[s];
	ft_printf("data_write_fail()\n");
	printfw(&e->fds[fd->parent], "====ERROR Download %s Fail\n", fd->filepath);
	data_fd_clean(fd);
}

void	data_write_success(t_env *e, int s)
{
	t_fd	*fd;

	ft_printf("data_write_end()\n");
	fd = &e->fds[s];
	printfw(&e->fds[fd->parent], "====SUCCESS Download %s Complete\n", fd->filepath);
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
	//ft_printf("data_write():%d, fd:%d\n",to, s);
	tosend = read(fd->fd, str, tosend);
	n = send(s, str, tosend, 0);
//	ft_printf("send():%d/%d octets\n", n, tosend);
	if (n>0)
	{
		fd->done += n;
	}
	else if (n == 0)
	{
		ft_printf("Fini Ou Deco !");
		data_write_fail(e, s);
	}
	if (fd->done == fd->size)
		data_write_success(e, s);
	return (0);
}
