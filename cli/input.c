/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/31 09:05:30 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 07:51:42 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

static int		input_get_connect(t_client *c, char *cmd)
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

static int		input_put_connect(t_client *c, char *cmd)
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

static void		input_error(t_client *c, char *cmd)
{
	char *str;

	if (c->status_data)
	{
		str = ft_mprintf("%s No more space or Permission change ", cmd);
		writemsg(c, str);
		free(str);
		data_fd_clean(c);
	}
	else
		writemsg(c, cmd);
}

static void		input_write(t_client *c, char *cmd)
{
	if (ft_strncmp(cmd, "====SUCCESS Down", 16) == 0)
		if (c->data_size == 0)
			data_fd_clean(c);
	writemsg(c, cmd);
}

int				input(t_client *c, char *cmd)
{
	int		i;
	char	**tab;

	i = 0;
	tab = ft_strsplit(cmd, '\n');
	if (ft_tablen(tab) > 0)
		while (tab[i])
		{
			if (ft_strncmp(tab[i], "dataget:", 8) == 0)
				input_get_connect(c, tab[i]);
			else if (ft_strncmp(tab[i], "dataput:", 8) == 0)
				input_put_connect(c, tab[i]);
			else if (ft_strncmp(tab[i], "STARTDATA", 9) == 0)
				c->status_data = 1;
			else if (ft_strncmp(tab[i], "====ERROR", 9) == 0)
				input_error(c, tab[i]);
			else
				input_write(c, tab[i]);
			i++;
		}
	ft_tabfree(tab);
	free(cmd);
	return (0);
}
