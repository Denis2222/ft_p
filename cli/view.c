/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/30 18:27:36 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/01 17:51:48 by dmoureu-         ###   ########.fr       */
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

static void	render_local(t_client *c, WINDOW *w)
{
	DIR				*dir;
	struct dirent	*dp;
	char			*str;
	clearmsglocal(c);

	str = ft_mprintf("PATH:%s",c->pwd);
	writemsglocal(c, str);
	free(str);
	writemsglocal(c, "============================");
	dir = opendir(c->pwd);
	while ((dp = readdir(dir)) != NULL)
	{
		writemsglocal(c, dp->d_name);
	}
	closedir(dir);

	t_msg			*msg;
	int				i;

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
	char *str;

	str = ft_mprintf("MSG:%d LOCAL:%d scroll:%d lscroll:%d", lenmsg(c->msg), lenmsg(c->msglocal), c->ws->scroll, c->ws->lscroll);
	wmove(c->ws->info, 1, 1);
	waddstr(c->ws->info, str);
	free(str);
}

void	view(t_client *c)
{
	WINDOW * cbox;
	WINDOW * lbox;
	WINDOW * pbox;

	t_windows *w;

	w = c->ws;

	clear();
	cbox = subwin(stdscr, LINES - 5, COLS/2, 0, 0);
	lbox = subwin(stdscr, LINES - 5, COLS/2, 0 , COLS/2);
	pbox = subwin(stdscr, 4, COLS, LINES - 4, 0);

	w->info = subwin(stdscr, 3, COLS, LINES - 6, 0);
	w->msg = subwin(stdscr, LINES - 6, COLS/2 - 2, 1, 1);
	w->local = subwin(stdscr, LINES - 6, COLS/2 - 2, 1, COLS/2 + 2);
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
