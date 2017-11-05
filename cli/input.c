/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/31 09:05:30 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 05:24:01 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

int		input_get_connect(t_client *c, int sock, char *cmd)
{
	char **tab;

	tab = ft_strsplit(cmd, ':');
	if (ft_tablen(tab) == 5)
	{
		c->data_size = ft_atoi(tab[3]);
		c->data_do = 0;
		c->data_file = ft_strnew(PATH_MAX);
		strcat(c->data_file, c->pwd);
		strcat(c->data_file, "/");
		strcat(c->data_file, tab[2]);
		c->data_way = WAYIN;
		socket_data(c, tab[1]);
	}
	ft_tabfree(tab);
	return (0);
}

int		input_put_connect(t_client *c, int sock, char *cmd)
{
	char **tab;

	tab = ft_strsplit(cmd, ':');
	if (ft_tablen(tab) == 5)
	{
		c->data_size = ft_atoi(tab[3]);
		c->data_do = 0;
		c->data_file = ft_strnew(PATH_MAX);
		strcat(c->data_file, c->pwd);
		strcat(c->data_file, "/");
		strcat(c->data_file, tab[2]);
		c->data_way = WAYOUT;
		socket_data(c, tab[1]);
	}
	ft_tabfree(tab);
	return (0);
}

int		input(t_client *c, int sock, char *cmd)
{
	int		i;
	char	**tab;
	char	*str;
	i = 0;
	tab = ft_strsplit(cmd, '\n');
	if (ft_tablen(tab) > 0)
		while (tab[i])
		{
			if (ft_strncmp(tab[i], "dataget:", 8) == 0)
				input_get_connect(c, sock, tab[i]);
			else if (ft_strncmp(tab[i], "dataput:", 8) == 0)
				input_put_connect(c, sock, tab[i]);
			else if (ft_strncmp(tab[i], "STARTDATA", 9) == 0)
				c->status_data = 1;
			else if (ft_strncmp(tab[i], "====ERROR", 9) == 0)
			{
				if (c->status_data)
				{
					str = ft_mprintf("%s No more space or Permission change ", tab[i]);
					writemsg(c, str);
					free(str);
					data_fd_clean(c, sock);
				}
				else
				{
					writemsg(c, tab[i]);
				}
			}
			else
				writemsg(c, tab[i]);
			i++;
		}
	ft_tabfree(tab);
	free(cmd);
	return (0);
}
