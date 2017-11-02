/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 02:28:48 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/02 07:54:09 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

int		retrieve_content(t_inline *buf, char *target, int max_len)
{
	int len;

	if (buf->length < (max_len - 1))
		len = buf->length;
	else
		len = (max_len - 1);
	ft_memcpy(target, buf->ln, len);
	target[len] = '\0';
	buf->cursor = 0;
	buf->length = 0;
	return (len + 1);
}

void	add_char(t_inline *buf, char ch)
{
	if (buf->length == buf->capacity)
		return ;
	ft_memmove(
		&buf->ln[buf->cursor + 1],
		&buf->ln[buf->cursor],
		buf->length - buf->cursor);
	buf->ln[buf->cursor] = ch;
	++buf->cursor;
	++buf->length;
}

int		handle_input_key(t_inline *buf, int key)
{
	if (key == KEY_HOME)
		buf->cursor = 0;
	if (key == KEY_END)
		buf->cursor = buf->length;
	if (key == KEY_BACKSPACE || key == 127 || key == 8)
	{
		if (buf->cursor <= 0)
			return (0);
		else
		{
			buf->cursor--;
			key = KEY_DC;
		}
	}
	if (key == KEY_DC)
		if (buf->cursor < buf->length)
		{
			ft_memmove(
				&buf->ln[buf->cursor],
				&buf->ln[buf->cursor + 1],
				buf->length - buf->cursor - 1);
			buf->length--;
		}
	return (0);
}

int		handle_input(t_inline *buf, char *target, int max_len, int key)
{
	if (!(key & KEY_CODE_YES) && ft_isprint(key))
	{
		add_char(buf, key);
		return (0);
	}
	if (key == ERR)
		return (0);
	if (key == KEY_LEFT)
		if (buf->cursor > 0)
			buf->cursor--;
	if (key == KEY_RIGHT)
		if (buf->cursor < buf->length)
			buf->cursor++;
	handle_input_key(buf, key);
	if (key == '\t')
		add_char(buf, '\t');
	if (key == KEY_ENTER || key == '\r' || key == '\n')
		return (retrieve_content(buf, target, max_len));
	return (0);
}

int		get_line_non_blocking(t_client *client, t_inline *buf, char *target, int max_len)
{
	int key;
	int n;

	while (1)
	{
		key = getch();
		//ft_dprintf(2, "key:%d", key);
		if (key == ERR)
			return (0);
		if (key == KEY_RESIZE)
			return (-1);
		if (key == 339 && client->ws->scroll > LINES - 7)
			client->ws->scroll--;
		if (key == 338 && client->ws->scroll < MAX_MSG)
			client->ws->scroll++;
		if (key == KEY_UP && client->ws->lscroll > LINES - 7)
			client->ws->lscroll--;
		if (key == KEY_DOWN && client->ws->lscroll < MAX_MSG)
			client->ws->lscroll++;
		n = handle_input(buf, target, max_len, key);
		if (n)
			return (n);
	}
}
