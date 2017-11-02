/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/31 09:05:30 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/02 00:38:49 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

int		input_get_connect(t_client *c, int sock, char *cmd)
{
	char **tab;

	ft_dprintf(2, "Input_get_connect()\n");
	tab = ft_strsplit(cmd, ':');
	if (ft_tablen(tab) == 5)
	{
		c->data_size = ft_atoi(tab[3]);
		c->data_do = 0;
		c->data_file = ft_strnew(PATH_MAX);
		strcat(c->data_file, c->pwd);
		strcat(c->data_file, "/");
		strcat(c->data_file, tab[2]);
		ft_dprintf(2, "c->data_file(output):%s\n", c->data_file);
		c->data_way = WAYIN;
		socket_data(c, tab[1]);
	}
	ft_tabfree(tab);
	return (0);
}

int		input_put_connect(t_client *c, int sock, char *cmd)
{
	char **tab;

	ft_dprintf(2, "Input_put_connect()\n");
	tab = ft_strsplit(cmd, ':');
	if (ft_tablen(tab) == 5)
	{
		c->data_size = ft_atoi(tab[3]);
		c->data_do = 0;
		c->data_file = ft_strnew(PATH_MAX);
		strcat(c->data_file, c->pwd);
		strcat(c->data_file, "/");
		strcat(c->data_file, tab[2]);
		ft_dprintf(2, "c->data_file(output):%s\n", c->data_file);
		c->data_way = WAYOUT;
		socket_data(c, tab[1]);
	}
	ft_tabfree(tab);
	return (0);
}


int		input(t_client *c, int sock, char *cmd)
{
	int	i;
	char **tab;

	i = 0;
	tab = ft_strsplit(cmd, '\n');
	if (ft_tablen(tab) > 0)
	while (tab[i])
	{
		//ft_dprintf(2, "INPUT   %d\n", ft_strncmp(tab[i], "dataget:", 8));
		if (ft_strncmp(tab[i], "dataget:", 8) == 0)
		{
			ft_dprintf(2, "dataget\n");
			input_get_connect(c, sock, tab[i]);
		}
		if (ft_strncmp(tab[i], "dataput:", 8) == 0)
		{
			ft_dprintf(2, "dataput\n");
			input_put_connect(c, sock, tab[i]);
		}
		i++;
	}
	return (0);
}
