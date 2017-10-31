/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftpd.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 13:46:48 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/10/31 03:35:31 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <dirent.h>
#include <sys/stat.h>
# include "libft.h"
# define PATH_MAX 4096

# define FD_FREE 0
# define FD_SERV 1
# define FD_CLIENT 2
# define FD_DATA 3

# define BUF_SIZE 4096 * 2

# define MAX(a, b)	((a > b) ? a : b)

typedef struct		s_fd
{
	int				sock;
	int				type;
	int				(*fct_read)();
	int				(*fct_write)();
	char			*br;
	int				brh;
	char			*bw;
	int				bwh;
	char			*pwd;
}					t_fd;

typedef struct		s_env
{
	t_fd			*fds;
	int				port;
	int				maxfd;
	int				max;
	int				r;
	fd_set			fd_read;
	fd_set			fd_write;
	char			*pwd;
}					t_env;

void	srv_listen(t_env *e, int port);
void	env_init(t_env *e);
void	fd_new(t_fd *fd, t_env *e, int type, int sock);
void	clean_fd(t_fd *fd);


int		srv_accept(t_env *e, int s);

int		client_write(t_env *e, int s);
int		client_read(t_env *e, int s);

int		data_read(t_env *e, int s);
int		data_write(t_env *e, int s);

void	fd_init(t_env *e);
void	fd_check(t_env *e);
void	fd_send(t_fd *fd, char *str);

void	input_pi(t_env *e, int sock);

void	input_ls(t_env *e, int cs);
void	input_cd(t_env *e, int cs, char *filename);


void	printfw(t_fd *fd, char *format, void *data);
void	printfd(t_fd *fd, char *format, long long int data);

