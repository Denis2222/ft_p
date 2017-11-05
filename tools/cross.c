/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cross.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/31 09:05:30 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/04 09:14:26 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cross.h"

void    osx_pipe(int socket)
{
	int	optval;

    optval = 1;
    setsockopt(socket, SOL_SOCKET, 0x0800, &optval, sizeof(optval));
}

void    linux_pipe(void)
{
    signal(SIGPIPE, SIG_IGN);
}