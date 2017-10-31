/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/31 09:05:30 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/10/31 13:10:42 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

int		input_get_connect(t_client *c, int sock, char *cmd)
{
	char **tab;

	ft_dprintf(2, "Input_get_connect()\n");
	tab = ft_strsplit(cmd, ':');
//	ft_dprintf(2, "ft_strsplit:%d\n", ft_tablen(tab));
	if (ft_tablen(tab) == 5)
	{
//		ft_dprintf(2, "%s\n", cmd);
//		ft_dprintf(2, "%s %s %s PATH_MAX:%d \n", tab[1], tab[2], tab[3], PATH_MAX);
		
//		ft_dprintf(2, "c->pwd:%s\n", c->pwd);
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
		i++;
	}
	return (0);
}
