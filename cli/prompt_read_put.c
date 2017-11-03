/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_read_put.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/03 22:54:10 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/03 23:10:28 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

int		check_put_file(t_client *c, char *filename)
{
	char		*str;
	int			fd;
	struct stat	buf;

	str = ft_strnew(PATH_MAX);
	ft_strcat(str, c->pwd);
	ft_strcat(str, "/");
	ft_strcat(str, filename);
	fd = open(str, O_RDONLY);
	if (fd <= 0)
	{
		writemsg(c, "====ERROR File not found OR no Read right !");
		free(str);
		return (0);
	}
	fstat(fd, &buf);
	close(fd);
	if ((buf.st_mode & S_IFMT) != S_IFREG)
	{
		writemsg(c, "====ERROR Special File");
		return (0);
	}
	str = ft_mprintf("put %s:%lld\n", filename, buf.st_size);
	ft_strcat(c->bw, str);
	free(str);
	return (1);
}

int	prompt_read_put(t_client *c, char *cmd)
{
	char	**tab;
	char	*str;

	str = ft_mprintf("[CMD]>%s", cmd);
	writemsg(c, str);
	free(str);
	tab = ft_strsplit(cmd, ' ');
	if (ft_tablen(tab) > 1)
	{
		check_put_file(c, tab[1]);
	}
	ft_tabfree(tab);
	return (1);
}
