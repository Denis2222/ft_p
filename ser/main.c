/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 13:46:06 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 06:10:16 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

void	do_select(t_env *e)
{
	e->r = select(e->max + 1, &e->fd_read, &e->fd_write, NULL, NULL);
	if (e->r < 0)
		perror("select()");
	ft_printf("select:%d\n", e->r);
}

int		main(int ac, char **argv)
{
	t_env e;

	signal(SIGTSTP, signalstop);
	if (!SO_NOSIGPIPE)
		linux_pipe();
	env_init(&e, ac, argv);
	srv_listen(&e);
	ft_printf("{green}Server FT_P ready on port [{red}%d{green}]{eoc}\n",
													e.port);
	while (1)
	{
		fd_init(&e);
		do_select(&e);
		fd_check(&e);
	}
	return (0);
}

void	printfd(t_fd *fd, char *format, long long int data)
{
	char *str;

	if (format)
		if (ft_strlen(format))
		{
			str = ft_mprintf(format, data);
			fd_send(fd, str);
			free(str);
		}
}

void	printfw(t_fd *fd, char *format, void *data)
{
	char *str;

	if (format)
		if (ft_strlen(format))
		{
			str = ft_mprintf(format, data);
			fd_send(fd, str);
			free(str);
		}
}

void	fd_send(t_fd *fd, char *str)
{
	if (fd->type != FD_FREE)
	{
		if ((ft_strlen(fd->bw) + ft_strlen(str)) >= BUF_SIZE + 1)
		{
			ft_printf("Buffer overflow\n");
			return ;
		}
		else
			ft_strcat(&fd->bw[fd->bwh], str);
	}
}
