/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/30 18:27:36 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/02 10:36:26 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

static void	render_text(t_client *client, WINDOW *w)
{
	t_msg			*msg;
	int				i;

	i = 0;
	msg = client->msg;
	while (msg)
	{
		if (i < client->ws->scroll)
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

int			little_ls(t_client *c, char *lsparam)
{
	int		link[2];
	pid_t	pid;
	char	*line;
	int		nbchar;
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
		while ((nbchar = get_next_line_single(link[0], &line)))
		{
			files++;
			writemsglocal(c, line);
			free(line);
		}
		wait(NULL);
	}
	return (files);
}

static void	render_local(t_client *c, WINDOW *w)
{
	char			*str;
	int				files;
	t_msg			*msg;
	int				i;

	clearmsglocal(c);
	str = ft_mprintf("PATH:%s", c->pwd);
	writemsglocal(c, str);
	free(str);
	files = little_ls(c, "-la");
	if (c->ws->lastlscroll != files)
	{
		c->ws->lscroll = lenmsg(c->msglocal);
		c->ws->lastlscroll = files;
	}
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

void	view_info(t_client *c)
{
	char	*str;
	int		pourcent;
	int		barre;

	if (c->socket_data && c->data_size > 0)
	{
		wmove(c->ws->info, 1, 1);
		pourcent = (c->data_do * 100) / c->data_size;
		barre = (pourcent * COLS - 2) / 100;
		str = ft_strnew(barre + 1);
		ft_memset(str, '#', barre);
		waddstr(c->ws->info, str);
		free(str);
		str = ft_mprintf("%d/100", pourcent);
		wmove(c->ws->info, 1, COLS / 2 - 3);
		waddstr(c->ws->info, str);
		free(str);
	}
	else
	{
		str = ft_mprintf("MSG:%d LOCAL:%d scroll:%d lscroll:%d", lenmsg(c->msg), lenmsg(c->msglocal), c->ws->scroll, c->ws->lscroll);
		wmove(c->ws->info, 1, 1);
		waddstr(c->ws->info, str);
		free(str);
	}
}

void	view(t_client *c)
{
	WINDOW		*cbox;
	WINDOW		*lbox;
	WINDOW		*pbox;
	t_windows	*w;

	w = c->ws;
	clear();
	cbox = subwin(stdscr, LINES - 5, COLS / 2, 0, 0);
	lbox = subwin(stdscr, LINES - 5, COLS / 2, 0, COLS / 2);
	pbox = subwin(stdscr, 4, COLS, LINES - 4, 0);
	w->info = subwin(stdscr, 3, COLS, LINES - 6, 0);
	w->msg = subwin(stdscr, LINES - 6, COLS / 2 - 2, 1, 1);
	w->local = subwin(stdscr, LINES - 6, COLS / 2 - 2, 1, COLS / 2 + 2);
	w->prompt = subwin(stdscr, 2, COLS - 2, LINES - 3, 1);
	scrollok(w->msg, TRUE);
	scrollok(w->prompt, TRUE);
	scrollok(w->local, TRUE);
	render_text(c, w->msg);
	wmove(w->prompt, 0, 0);
	render_line(&c->lnbuffer, w->prompt);
	view_info(c);
	render_local(c, w->local);
	box(w->info, ACS_VLINE, ACS_HLINE);
	box(cbox, ACS_VLINE, ACS_HLINE);
	box(pbox, ACS_VLINE, ACS_HLINE);
	box(lbox, ACS_VLINE, ACS_HLINE);
	wrefresh(w->msg);
	wrefresh(w->prompt);
	wrefresh(w->info);
	wrefresh(w->local);
	refresh();
}
