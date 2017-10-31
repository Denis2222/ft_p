/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_env.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 13:58:26 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/10/30 20:35:52 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"
#include <sys/time.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>


void	srv_listen(t_env *e, int port)
{
	struct	protoent	*pe;
	struct sockaddr_in	sin;
	int					sock;

	pe = getprotobyname("tcp");
	if (!pe)
	{
		ft_printf("Erreur getprotobyname\n");
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, pe->p_proto);
	if (socket <= 0)
	{
		ft_printf("Erreur socket %d\n", socket);
		exit(1);
	}
	//Creation sockaddr_in
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);
	e->port = port;
	if (bind(sock, (struct sockaddr*)&sin, sizeof(sin)) == -1)
	{
		perror("bind()");
		ft_printf("Error de bind sur le socket\n");
		exit (1);
	}
	if (listen(sock, 42) == -1)
	{
		ft_printf("Error sur listen\n");
		exit(1);
	}

	fd_new(&e->fds[sock], e, FD_SERV, sock);
}

void	env_init(t_env *e)
{
	int				i;
	struct rlimit	rlp;

	getrlimit(RLIMIT_NOFILE, &rlp);
	e->maxfd = rlp.rlim_cur;
	e->fds = (t_fd*)malloc(sizeof(*e->fds) * e->maxfd);
	if (e->fds == NULL)
	{
		ft_printf("Malloc error");
		exit(0);
	}
	e->pwd = ft_strnew(PATH_MAX);
	getcwd(e->pwd, PATH_MAX);
	ft_printf("boot pwd : %s\n", e->pwd);
	i = 0;
	while (i < e->maxfd)
	{
		clean_fd(&e->fds[i]);
		i++;
	}
}
