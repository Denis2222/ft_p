/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_read_lls.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/04 00:15:07 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 14:04:32 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

int		prompt_read_lls(t_client *c, char *cmd)
{
	char	**tab;
	char	*str;

	str = ft_mprintf("[local CMD]>%s", cmd);
	writemsg(c, str);
	free(str);
	tab = ft_strsplit(cmd, ' ');
	if (ft_tablen(tab) > 1)
	{
		if (ft_strlen(tab[1]))
		{
			if (c->ws->localls)
				free(c->ws->localls);
			if (ft_strchr(tab[1], 'R'))
				ft_strrepchr(tab[1], 'R', 'a');
			c->ws->localls = ft_strdup(tab[1]);
		}
	}
	ft_tabfree(tab);
	writemsg(c, "====SUCCESS");
	return (1);
}
