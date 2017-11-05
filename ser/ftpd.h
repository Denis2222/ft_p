/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftpd.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 13:46:48 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 18:23:39 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTPD_H
# define FTPD_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <dirent.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/time.h>
# include <sys/resource.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <netinet/in.h>
# include <netdb.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <time.h>

# include "../tools/cross.h"

# include "libft.h"

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

# define FD_FREE 0
# define FD_SERV 1
# define FD_CLIENT 2
# define FD_DATA 3

# define BUF_SIZE 1024 * 16

# define MAX(a, b)	((a > b) ? a : b)
# define MIN(a, b)	((a < b) ? a : b)

# define WAYIN 1
# define WAYOUT 2

# define MSGCDUKN "====ERROR unknow path!\npath : %s\n"
# define MSGCDEMPTY "====ERROR empty path!\npath : %s\n"

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
	int				data;

	int				parent;
	int				way;
	int				port;
	char			*filepath;
	long long int	size;
	long long int	done;
	int				fd;
	unsigned long	time;
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

int					getproto(void);
void				srv_listen(t_env *e);
int					srv_listen_data(t_env *e);
void				srv_setsockopt(int sock);
void				srv_listen_sin(struct sockaddr_in *sin, int port);
int					srv_accept(t_env *e, int s);
int					srv_data_accept(t_env *e, int s);

void				env_init(t_env *e, int ac, char **argv);
void				fd_new(t_fd *fd, t_env *e, int type, int sock);
void				clean_fd(t_fd *fd);

int					client_write(t_env *e, int s);
int					client_read(t_env *e, int s);

int					data_read(t_env *e, int s);
int					data_write(t_env *e, int s);
void				calcspeed(t_env *e, t_fd *fd);
void				data_fd_clean(t_fd *fd);

void				fd_init(t_env *e);
void				fd_check(t_env *e);
void				fd_send(t_fd *fd, char *str);

void				input_pi(t_env *e, int sock);

void				input_ls(t_env *e, int cs, char *cmd);
void				input_cd(t_env *e, int cs, char *filename);
void				input_get(t_env *e, int s, char *cmd);
void				input_put(t_env *e, int s, char *cmd);
void				input_pwd(t_env *e, int s);

void				printfw(t_fd *fd, char *format, void *data);
void				printfd(t_fd *fd, char *format, long long int data);

void				close_fd(int fd);

#endif
