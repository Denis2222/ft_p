/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/30 18:27:36 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 03:37:44 by dmoureu-         ###   ########.fr       */
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

void		view_info(t_client *c)
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
		wmove(c->ws->info, 1, 1);
		str = ft_mprintf("socket_pi:%d, socket_data:%d, data_fd:%d ", c->socket_pi, c->socket_data, c->data_fd);
		waddstr(c->ws->info, str);
	}
}

void		box_text(WINDOW *c, WINDOW *l)
{
	wmove(c, 0, 2);
	waddstr(c, "Server :");
	wmove(l, 0, 2);
	waddstr(l, "Local :");
}

static void	view2(t_client *c)
{
	t_windows	*w;

	w = c->ws;
	w->info = subwin(stdscr, 3, COLS, LINES - 6, 0);
	w->msg = subwin(stdscr, LINES - 6, COLS / 2 - 2, 1, 1);
	w->local = subwin(stdscr, LINES - 6, COLS / 2 - 2, 1, COLS / 2 + 2);
	w->prompt = subwin(stdscr, 2, COLS - 2, LINES - 3, 1);
	scrollok(w->local, TRUE);
	scrollok(w->msg, TRUE);
	scrollok(w->prompt, TRUE);
	render_text(c, w->msg);
	wmove(w->prompt, 0, 0);
	render_line(&c->lnbuffer, w->prompt);
	view_info(c);
	render_local(c, w->local);
	box(w->info, ACS_VLINE, ACS_HLINE);
}

void		view(t_client *c)
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
	view2(c);
	box(cbox, ACS_VLINE, ACS_HLINE);
	box(pbox, ACS_VLINE, ACS_HLINE);
	box(lbox, ACS_VLINE, ACS_HLINE);
	box_text(cbox, lbox);
	wrefresh(w->msg);
	wrefresh(w->prompt);
	wrefresh(w->info);
	wrefresh(w->local);
	refresh();
}
