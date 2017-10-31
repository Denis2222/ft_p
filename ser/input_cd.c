/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/30 22:47:02 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/10/31 05:40:31 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

void input_cd(t_env *e, int cs, char *filename)
{
	char *tmppath;

	printf("BEFORE cd .. %s\n", e->fds[cs].pwd);
	if (filename)
	{
		printfw(&e->fds[cs] , "cd %s\n", filename);
		tmppath = ft_strdup(e->fds[cs].pwd);
		ft_strcat(e->fds[cs].pwd, "/");
		ft_strcat(e->fds[cs].pwd, filename);
		
		if (chdir(e->fds[cs].pwd)==0)
		{
			getcwd(e->fds[cs].pwd, PATH_MAX);
			if (ft_strncmp(e->pwd, e->fds[cs].pwd, ft_strlen(e->pwd)) == 0)
			{
				printfw(&e->fds[cs] ,"new path : %s\n====SUCCESS\n", e->fds[cs].pwd);
			}
			else
			{
				ft_strcpy(e->fds[cs].pwd, tmppath);
				printfw(&e->fds[cs], "====ERROR out of pwd!\npath : %s\n", e->fds[cs].pwd);
			}
			chdir(e->pwd);
		}
		else
		{
			ft_strcpy(e->fds[cs].pwd, tmppath);
			printfw(&e->fds[cs], "====ERROR unknow path!\npath : %s\n", e->fds[cs].pwd);
		}
		free(tmppath);
	}
	else
		printfw(&e->fds[cs], "====ERROR empty path!\npath : %s\n", e->fds[cs].pwd);
}
