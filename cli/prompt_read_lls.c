/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_read_lls.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/04 00:15:07 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/04 05:34:44 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

int		prompt_read_lls(t_client *c, char *cmd)
{
	char	**tab;

	tab = ft_strsplit(cmd, ' ');
	if (ft_tablen(tab) > 1)
	{
		if (ft_strlen(tab[1]))
		{
			if (c->ws->localls)
				free(c->ws->localls);
			c->ws->localls = ft_strdup(tab[1]);
		}
	}
	ft_tabfree(tab);
	return (1);
}
