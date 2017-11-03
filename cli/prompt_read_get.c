/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_read_get.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/03 22:51:16 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/03 23:44:21 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

int	prompt_read_get(t_client *c, char *cmd)
{
	char	**tab;
	int		fd;
	int		create;
	struct stat	buf;
	int		ret;

	ret = 0;
	create = 0;
	tab = ft_strsplit(cmd, ' ');
	fd = open(tab[1], O_WRONLY, S_IRWXU);
	if (fd > 0)
		ft_dprintf(2, "WRITE OK \n");
	else
	{
		fd = open(tab[1], O_CREAT, S_IRWXU);
		if (fd > 0)
		{
			create = 1;
			ft_dprintf(2, "CREAT OK\n");
		}
		else
		{
			ft_dprintf(2, "GET IMPOSSIBLE file not writable \n");
			ret = 1;
		}
	}
	fstat(fd, &buf);
	if ((buf.st_mode & S_IFMT) != S_IFREG)
	{
		ft_dprintf(2, "Special file");
		ret = 1;
	}
	if (create)
		unlink(tab[1]);

	if (fd > 0)
		close(fd);
	ft_tabfree(tab);
	return (ret);
}
