/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ncurses.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/30 18:10:24 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/02 10:28:24 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

void	ncurse_init(void)
{
	initscr();
	start_color();
	cbreak();
	nonl();
	timeout(0);
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	intrflush(stdscr, 0);
	leaveok(stdscr, 1);
	clear();
	init_pair(4, COLOR_WHITE, COLOR_BLACK);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
}

void	ncurse_end(void)
{
	delwin(stdscr);
	endwin();
}
