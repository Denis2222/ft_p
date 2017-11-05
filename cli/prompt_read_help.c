/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_read_help.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/04 00:01:33 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 14:10:10 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

int	prompt_read_help(t_client *c)
{
	writemsg(c, "================");
	writemsg(c, "======HELP======");
	writemsg(c, "================");
	writemsg(c, "local:");
	writemsg(c, "-lcd [dir]:  Change local dir");
	writemsg(c, "-lpwd: ....");
	writemsg(c, "-lls [options]:  ....");
	writemsg(c, "-clear:  clear msg");
	writemsg(c, "-quit:  ...");
	writemsg(c, "-help:  ...");
	writemsg(c, "server:");
	writemsg(c, "-cd:  Change server dir");
	writemsg(c, "-ls:  List files");
	writemsg(c, "-pwd:  List files");
	writemsg(c, "transfert:");
	writemsg(c, "-get: Transfert server->local");
	writemsg(c, "-put: Transfert local->server");
	return (1);
}
