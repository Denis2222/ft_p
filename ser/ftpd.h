/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftpd.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 13:46:48 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/10/24 14:01:46 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

# include <libft.h>

typedef struct		s_fd
{
	int				type;
	void			(*fct_read)();
	void			(*fct_write)();
	void			(*fct_send)();
	void			(*fct_receive)();
	char			*buf_read;
	char			*buf_write;
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
	char			pwd[PATH_MAX];
}					t_env;

void			env_init(t_env *e);