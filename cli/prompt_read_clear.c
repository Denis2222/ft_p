/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_read_clear.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/05 13:53:36 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 13:58:15 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

int	prompt_read_clear(t_client *c, char *cmd)
{
	char *str;

	clearmsg(c);
	str = ft_mprintf("[local CMD]>%s", cmd);
	writemsg(c, str);
	free(str);
	writemsg(c, "====SUCCESS Clear Console");
	return (1);
}
