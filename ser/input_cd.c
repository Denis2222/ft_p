/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/30 22:47:02 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/04 01:39:51 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

static char	*input_cd_filepath(t_env *e, int cs, char *filename)
{
	char	*tmppath;

	tmppath = ft_strdup(e->fds[cs].pwd);
	ft_strcat(e->fds[cs].pwd, "/");
	ft_strcat(e->fds[cs].pwd, filename);
	return (tmppath);
}

static void	input_cd_truepath(t_env *e, int cs, char *tmppath)
{
	getcwd(e->fds[cs].pwd, PATH_MAX);
	if (ft_strncmp(e->pwd, e->fds[cs].pwd, ft_strlen(e->pwd)) == 0)
		printfw(&e->fds[cs], "new path : %s\n====SUCCESS\n",
										e->fds[cs].pwd);
	else
	{
		ft_strcpy(e->fds[cs].pwd, tmppath);
		printfw(&e->fds[cs], "====ERROR out of pwd!\npath : %s\n",
												e->fds[cs].pwd);
	}
	chdir(e->pwd);
}

void		input_cd(t_env *e, int cs, char *cmd)
{
	char	*tmppath;
	char	**tab;
	char	*filename;

	tab = ft_strsplit(cmd, ' ');
	if (ft_tablen(tab) > 1)
	{
		filename = tab[1];
		if (filename)
		{
			tmppath = input_cd_filepath(e, cs, filename);
			printfw(&e->fds[cs], "cd %s\n", filename);
			if (chdir(e->fds[cs].pwd) == 0)
				input_cd_truepath(e, cs, tmppath);
			else
			{
				ft_strcpy(e->fds[cs].pwd, tmppath);
				printfw(&e->fds[cs], "====ERROR unknow path!\npath : %s\n",
															e->fds[cs].pwd);
			}
			free(tmppath);
		}
		else
			printfw(&e->fds[cs], "====ERROR empty path!\npath : %s\n",
														e->fds[cs].pwd);
	}
	ft_tabfree(tab);
	return ;
}
