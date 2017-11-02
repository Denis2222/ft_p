/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_read.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/31 02:42:15 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/02 07:59:12 by dmoureu-         ###   ########.fr       */
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
	str = ft_mprintf("put %s:%lld", filename, buf.st_size);
	ft_strcat(c->bw, str);
	ft_strcat(c->bw, "\n");
	free(str);
	return (1);
}

int		prompt_read_cmd(t_client *c, char *cmd)
{
	char	**tab;
	char	*str;

	if (ft_strncmp(cmd, "lcd ", 4) == 0)
	{
		tab = ft_strsplit(cmd, ' ');
		if (ft_tablen(tab) > 1)
		{
			chdir(tab[1]);
			getcwd(c->pwd, PATH_MAX);
			view(c);
		}
		ft_tabfree(tab);
		return (1);
	}
	if (ft_strncmp(cmd, "put ", 4) == 0)
	{
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
	if (ft_strncmp(cmd, "lls", 3) == 0)
	{
		return (1);
	}
	if (ft_strncmp(cmd, "lpwd", 4) == 0)
	{
		return (1);
	}
	if (ft_strncmp(cmd, "quit", 4) == 0)
	{
		c->run = 0;
		return (1);
	}
	return (0);
}

int	prompt_read(t_client *c)
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
	else if (len == -1)
		return (1);
	return (0);
}
