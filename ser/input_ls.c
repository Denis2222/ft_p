/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_ls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/30 22:46:00 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/10/31 05:04:25 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

void input_ls(t_env *e, int cs)
{

//	ft_dprintf(cs, "========================\n", e->pwd,e->fds[cs].pwd, 0);

	printfw(&e->fds[cs],"Racine %s \n", e->pwd); 
	printfw(&e->fds[cs],"pwd %s \n========================\n", e->fds[cs].pwd);
	
	DIR *dir;
	struct dirent	*dp;
	struct stat		buf;
	int				fd;

	dir = opendir(e->fds[cs].pwd);
	if (dir)
	{
		while ((dp=readdir(dir)) != NULL) {
			fstat(fd, &buf);
			if ( !strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..") )
			{
				// do nothing (straight logic)
			} else {
				fd = open(dp->d_name, O_RDONLY);
				fstat(fd, &buf);
				if (dp->d_type == DT_DIR)
					printfw(&e->fds[cs], "<dir> %s\n", dp->d_name);
				else if(dp->d_type == DT_REG)
				{
					printfw(&e->fds[cs], "      %s", dp->d_name);
					printfd(&e->fds[cs], " (%lld octets)\n", buf.st_size);
				}
				close(fd);
			}
		}
		closedir(dir);
		printfw(&e->fds[cs],"====SUCCESS\n", 0); 
//		fd_send(e, cs, "====SUCCESS\n");
	}
	else
	{
		//presend(e, cs, "====ERROR invalid pwd\n");
	}
}

