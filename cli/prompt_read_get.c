/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_read_get.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/03 22:51:16 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/04 05:31:39 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

static int	prompt_read_get_fstat(t_client *c, int fd, int ret)
{
	struct stat	buf;

	if (ret)
		return (1);
	fstat(fd, &buf);
	if ((buf.st_mode & S_IFMT) != S_IFREG)
	{
		writemsg(c, "====ERROR Can't erase Special file");
		return (1);
	}
	return (0);
}

static void	fucknorme(t_client *c, int *ret)
{
	writemsg(c, "====ERROR No permission to write");
	*ret = 1;
}

int			prompt_read_get(t_client *c, char *cmd)
{
	char	**tab;
	int		fd;
	int		create;
	int		ret;

	ret = 0;
	create = 0;
	tab = ft_strsplit(cmd, ' ');
	fd = open(tab[1], O_WRONLY, S_IRWXU);
	if (fd <= 0)
	{
		fd = open(tab[1], O_CREAT, S_IRWXU);
		if (fd > 0)
			create = 1;
		else
			fucknorme(c, &ret);
	}
	ret = prompt_read_get_fstat(c, fd, ret);
	if (create)
		unlink(tab[1]);
	if (fd > 0)
		close(fd);
	ft_tabfree(tab);
	return (ret);
}
