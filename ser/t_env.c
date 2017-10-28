/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_env.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 13:58:26 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/10/24 14:01:29 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftpd.h"

void	env_init(t_env *e)
{
	int				i;
	struct rlimit	rlp;

	X(-1, getrlimit(RLIMIT_NOFILE, &rlp), "getrlimit");
	e->maxfd = rlp.rlim_cur;
	e->fds = (t_fd*)malloc(sizeof(*e->fds) * e->maxfd);
	if (e->fds == NULL)
	{
		ft_printf("Malloc error");
		exit(0);
	}
	getcwd(e->pwd, PATH_MAX);
	ft_printf("boot pwd : %s", e->cwd);
	i = 0;
	while (i < e->maxfd)
	{
		clean_fd(&e->fds[i]);
		i++;
	}
}
