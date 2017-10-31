/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_read.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/31 02:42:15 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/10/31 05:28:19 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

int		prompt_read_cmd(t_client *c, char *cmd)
{
	char **tab;

	if (ft_strncmp(cmd, "lcd ", 4) == 0)
	{
		tab = ft_strsplit(cmd, ' ');
		if (ft_tablen(tab) > 1)
		{
			chdir(tab[1]);
			getcwd(c->pwd, PATH_MAX);
			view(c);
			c->ws->lscroll = lenmsg(c->msglocal);
		}
		ft_tabfree(tab);
		return (1);
	}
	return (0);
}

void	prompt_read(t_client *c)
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
	view(c);
}
