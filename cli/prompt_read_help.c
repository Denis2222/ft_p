/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_read_help.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/04 00:01:33 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/04 00:06:50 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

int	prompt_read_help(t_client *c, char *cmd)
{
	writemsg(c, "================");
	writemsg(c, "======HELP======");
	writemsg(c, "================");
	writemsg(c, "local:");
	writemsg(c, "-lcd:  Change local dir");
	writemsg(c, "-lpwd: ....");
	writemsg(c, "-lls:  ....");

	writemsg(c, "server:");
	writemsg(c, "-cd:  Change server dir");
	writemsg(c, "-ls:  List files");
	writemsg(c, "transfert:");
	writemsg(c, "-get: Transfert server->local");
	writemsg(c, "-put: Transfert local->server");

	return (1);
}
