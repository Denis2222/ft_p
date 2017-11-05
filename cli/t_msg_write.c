/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_msg_write.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/04 05:32:26 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/05 06:03:35 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

void	writemsglocal(t_client *client, char *cmd)
{
	t_msg	*tmp;
	int		i;
	char	**tab;
	char	*s;

	s = ft_strdup(cmd);
	i = 0;
	tab = ft_strsplit(s, '\n');
	while (tab[i])
	{
		client->msglocal = addmsg(&client->msglocal, newmsg(tab[i]));
		if (lenmsg(client->msglocal) > MAX_MSG)
		{
			tmp = client->msglocal->next;
			free(client->msglocal->text);
			free(client->msglocal);
			client->msglocal = tmp;
		}
		i++;
	}
	ft_tabfree(tab);
	free(s);
}

void	writemsg(t_client *client, char *cmd)
{
	t_msg	*tmp;
	int		i;
	char	**tab;

	i = 0;
	tab = ft_strsplit(cmd, '\n');
	while (tab[i])
	{
		client->msg = addmsg(&client->msg, newmsg(tab[i]));
		if (lenmsg(client->msg) > MAX_MSG)
		{
			tmp = client->msg->next;
			free(client->msg->text);
			free(client->msg);
			client->msg = tmp;
		}
		i++;
	}
	client->ws->scroll = lenmsg(client->msg);
	ft_tabfree(tab);
}
