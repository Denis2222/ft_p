/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_read.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/31 02:42:15 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/04 05:58:36 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

int		prompt_read_cmd(t_client *c, char *cmd)
{
	if (ft_strncmp(cmd, "lcd", 3) == 0)
		return (prompt_read_lcd(c, cmd));
	if (ft_strncmp(cmd, "get ", 4) == 0)
		return (prompt_read_get(c, cmd));
	if (ft_strncmp(cmd, "put ", 4) == 0)
		return (prompt_read_put(c, cmd));
	if (ft_strncmp(cmd, "lls", 3) == 0)
		return (prompt_read_lls(c, cmd));
	if (ft_strncmp(cmd, "lpwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "help", 4) == 0)
		return (prompt_read_help(c, cmd));
	if (ft_strncmp(cmd, "quit", 4) == 0)
	{
		c->run = 0;
		return (1);
	}
	return (0);
}

int		prompt_read(t_client *c)
{
	char	ln[PROMPT_SIZE_MAX];
	int		len;

	len = get_line_non_blocking(c, &c->lnbuffer, ln, sizeof(ln));
	if (len > 0 && ft_strlen(ln))
	{
		if (!prompt_read_cmd(c, ln))
		{
			ft_strcat(c->bw, ln);
			ft_strcat(c->bw, "\n");
			ft_bzero(ln, PROMPT_SIZE_MAX);
		}
	}
	else if (len == -1)
		return (1);
	return (0);
}
