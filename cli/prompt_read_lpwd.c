/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_read_lpwd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 13:53:36 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 14:02:53 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

int	prompt_read_lpwd(t_client *c, char *cmd)
{
	char *str;

	str = ft_mprintf("[local CMD]>%s\n %s", cmd, c->pwd);
	writemsg(c, str);
	free(str);
	writemsg(c, "====SUCCESS");
	return (1);
}
