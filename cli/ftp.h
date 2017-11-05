/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 13:46:48 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 09:45:24 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTP_H

# define FTP_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

# include <libft.h>
# include <sys/wait.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <dirent.h>
# include <strings.h>
# include <ncurses.h>
# include <limits.h>
# include <sys/stat.h>

# include "../tools/cross.h"

# define INVALID_SOCKET	-1
# define SOCKET_ERROR	-1
# define MAX(a, b)	((a > b) ? a : b)
# define PROMPT_SIZE_MAX 10
# define MAX_MSG 150
# define WAYIN 1
# define WAYOUT 2
# define BUF_SIZE 1024 * 16

# define OPENOPT O_CREAT | O_WRONLY
# define OPENPERM S_IRWXU | S_IRWXG | S_IRWXO

typedef struct			s_input_line {
	char				*ln;
	int					length;
	int					capacity;
	int					cursor;
	int					last_rendered;
}						t_inline;

typedef struct			s_windows
{
	WINDOW				*msg;
	int					scroll;
	WINDOW				*local;
	int					lscroll;
	WINDOW				*prompt;
	WINDOW				*info;
	int					lastlscroll;
	char				*localls;
}						t_windows;

typedef struct			s_msg
{
	char				*text;
	struct s_msg		*next;
	int					color;
}						t_msg;

typedef struct			s_client
{
	char				*pwd;
	fd_set				fd_read;
	fd_set				fd_write;

	struct sockaddr_in	sin;
	struct hostent		*hostinfo;

	int					run;
	int					status_pi;
	int					status_data;

	char				*bw;

	int					socket_pi;
	int					socket_data;

	long long int		data_size;
	long long int		data_do;
	char				*data_file;
	int					data_way;
	int					data_fd;

	int					select;

	char				*prompt;
	t_inline			lnbuffer;
	t_msg				*msg;
	t_msg				*msglocal;

	t_windows			*ws;
}						t_client;

int						connect_pi(int ac, char **argv, t_client *client);

int						loop(t_client *client, int i);
void					ncurse_init(void);
void					ncurse_end(void);

void					view(t_client *c);
void					render_local(t_client *c, WINDOW *w);
void					view_info(t_client *c);

void					client_init(t_client *client);
void					client_reset(t_client *client);

int						prompt_read(t_client *c);
int						prompt_read_lcd(t_client *c, char *cmd);
int						prompt_read_get(t_client *c, char *cmd);
int						prompt_read_put(t_client *c, char *cmd);
int						prompt_read_help(t_client *c);
int						prompt_read_lls(t_client *c, char *cmd);

int						input(t_client *c, char *cmd);
int						socket_data(t_client *client, char *port);

void					make_buffer(t_inline *buf);
void					destroy_buffer(t_inline *buf);
void					render_line(t_inline *buf, WINDOW *win);
int						retrieve_content(t_inline *buf, char *target,
						int max_len);
void					add_char(t_inline *buf, char ch);
int						handle_input(t_inline *buf, char *target, int max_len,
						int key);
int						get_line_non_blocking(t_client *client,
t_inline *buf, char *target, int max_len);

t_msg					*newmsg(char *text);
t_msg					*addmsg(t_msg **lstmsg, t_msg *msg);
int						lenmsg(t_msg *msg);
void					writemsg(t_client *client, char *cmd);
void					clearmsg(t_client *client);
void					showmsghelp(t_client *client);
void					writemsglocal(t_client *client, char *cmd);
void					clearmsglocal(t_client *client);

void					socket_write(t_client *c, int sock);
void					socket_read(t_client *c, int sock);
void					data_read(t_client *c, int sock);
void					data_write(t_client *c, int sock);
void					data_fd_clean(t_client *c);

void					socket_send(t_client *c, char *str);

#endif
