/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_ls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/30 22:46:00 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/02 08:56:45 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

#include <sys/wait.h>
int little_ls(t_env *e, int cs, char *lsparam) {

  int link[2];
  pid_t pid;
  char foo[500 + 1];
	int nbchar;
  pipe(link);

  pid = fork();

  if(pid == 0) {

    dup2 (link[1], STDOUT_FILENO);
    close(link[0]);
    close(link[1]);
    execl("/bin/ls", "ls", lsparam, (char *)0);
//    die("execl");

  } else {

    close(link[1]);
	bzero(foo, 500 + 1);
    while((nbchar = read(link[0], foo, 500)))
    {
		fd_send(&e->fds[cs], foo);
		bzero(foo, 500 +1);
		ft_printf("Output: %d\n", nbchar);
	}
    wait(NULL);

  }
  return 0;

}


void input_ls(t_env *e, int cs, char *cmd)
{
	char **tab;
	chdir(e->fds[cs].pwd);

	tab = ft_strsplit(cmd, ' ');

	little_ls(e, cs, tab[1]);

	ft_tabfree(tab);
//	ft_dprintf(cs, "========================\n", e->pwd,e->fds[cs].pwd, 0);
/*
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
*/
	chdir(e->pwd);

}

