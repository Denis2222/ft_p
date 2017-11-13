/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_env.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 13:58:26 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/13 16:59:23 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

void	env_init_fd(t_env *e)
{
	int				i;

	i = 0;
	while (i < e->maxfd)
	{
		clean_fd(&e->fds[i]);
		i++;
	}
}

void	env_init(t_env *e, int ac, char **argv)
{
	struct rlimit	rlp;

	getrlimit(RLIMIT_NOFILE, &rlp);
	if (ac > 1 && ft_atoi(argv[1]))
		e->port = ft_atoi(argv[1]);
	else
		e->port = 2000;
	if (e->port < 1024 || e->port > 65534)
		e->port = 2000;
	e->maxfd = rlp.rlim_cur;
	e->fds = (t_fd*)malloc(sizeof(*e->fds) * e->maxfd);
	if (e->fds == NULL)
		exit(0);
	e->pwd = ft_strnew(PATH_MAX);
	getcwd(e->pwd, PATH_MAX);
	if (ac > 2)
	{
		if (chdir(argv[2]) == 0)
			getcwd(e->pwd, PATH_MAX);
		else
			ft_printf("Invalid pwd !\n");
	}
	ft_printf("PWD : %s\n", e->pwd);
}
