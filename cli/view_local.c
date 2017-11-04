/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view_local.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/04 06:07:59 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/04 06:27:00 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

static char	*get_next_cmd(char *buffer)
{
	int		length;
	int		cmdlength;
	int		pos;
	char	*cmd;
	char	*tmp;

	if (!ft_strchr(buffer, '\n'))
		return (NULL);
	length = ft_strlen(buffer);
	pos = ft_strlen(ft_strchr(buffer, '\n'));
	cmdlength = length - pos;
	cmd = ft_strsub(buffer, 0, cmdlength);
	tmp = ft_strsub(buffer, cmdlength + 1, length);
	ft_bzero(buffer, BUF_SIZE + 1);
	ft_strcpy(buffer, tmp);
	free(tmp);
	return (cmd);
}

int			little_ls_pipe(t_client *c, int fd)
{
	char	buf[BUF_SIZE + 1];
	int		n;
	char	*test;
	int		files;

	files = 0;
	ft_bzero(buf, BUF_SIZE + 1);
	while ((n = read(fd, buf, BUF_SIZE)))
	{
		while ((test = get_next_cmd(buf)))
		{
			files++;
			writemsglocal(c, test);
			free(test);
		}
	}
	return (files);
}

int			little_ls(t_client *c, char *lsparam)
{
	int		link[2];
	pid_t	pid;
	int		files;

	files = 0;
	pipe(link);
	pid = fork();
	if (pid == 0)
	{
		dup2(link[1], STDOUT_FILENO);
		close(link[0]);
		close(link[1]);
		execl("/bin/ls", "ls", lsparam, (char *)0);
	}
	else
	{
		close(link[1]);
		files = little_ls_pipe(c, link[0]);
		wait(NULL);
	}
	return (files);
}

static void	render_local_scroll(t_client *c, int files)
{
	if (c->ws->lastlscroll != files)
	{
		c->ws->lscroll = lenmsg(c->msglocal);
		c->ws->lastlscroll = files;
	}
}

void		render_local(t_client *c, WINDOW *w)
{
	char			*str;
	int				files;
	t_msg			*msg;
	int				i;

	clearmsglocal(c);
	str = ft_mprintf("PATH:%s", c->pwd);
	writemsglocal(c, str);
	free(str);
	files = little_ls(c, c->ws->localls);
	render_local_scroll(c, files);
	i = 0;
	msg = c->msglocal;
	while (msg)
	{
		if (i < c->ws->lscroll)
		{
			wattron(w, COLOR_PAIR(msg->color));
			waddstr(w, msg->text);
			waddch(w, '\n');
			wattroff(w, COLOR_PAIR(msg->color));
		}
		msg = msg->next;
		i++;
	}
}
