/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 03:23:14 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/10/30 18:44:14 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

void	make_buffer(t_inline *buf)
{
	buf->ln = malloc(PROMPT_SIZE_MAX + 1);
	ft_bzero(buf->ln, PROMPT_SIZE_MAX + 1);
	buf->length = 0;
	buf->capacity = PROMPT_SIZE_MAX;
	buf->cursor = 0;
	buf->last_rendered = 0;
}

void	destroy_buffer(t_inline *buf)
{
	free(buf->ln);
	make_buffer(buf);
}

void	render_line(t_inline *buf, WINDOW *win)
{
	int		i;
	int		rendered;
	chtype	c;

	i = 0;
	while (i < buf->length)
	{
		c = buf->ln[i];
		if (i == buf->cursor)
			c |= A_REVERSE;
		waddch(win, c);
		i++;
	}
	if (buf->cursor == buf->length)
	{
		waddch(win, ' ' | A_REVERSE);
		i++;
	}
	rendered = i;
	while (i < buf->last_rendered)
	{
		waddch(win, ' ');
		i++;
	}
	buf->last_rendered = rendered;
}
