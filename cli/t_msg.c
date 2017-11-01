/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_msg.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/02 18:36:48 by anonymou          #+#    #+#             */
/*   Updated: 2017/11/01 17:44:21 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp.h"

t_msg	*newmsg(char *text, t_client *client)
{
	t_msg	*msg;;;
	char	**tab;;

	(void)client;
	msg = (t_msg *)malloc(sizeof(t_msg));
	msg->text = ft_strdup(text);
	msg->next = NULL;
	msg->color = 4;
	tab = ft_strsplit(text, ':');
	if (tab != NULL && ft_tablen(tab) > 0)
	{
		if (ft_strstr(tab[0], "====SUCCESS"))
			msg->color = 2;
		else if (ft_strstr(tab[0], "====ERROR"))
			msg->color = 1;
		else if (ft_strstr(tab[0], "====QUERY"))
			msg->color = 3;
		else
			msg->color = 4;
		ft_tabfree(tab);
	}
	return (msg);
}

t_msg	*addmsg(t_msg **lstmsg, t_msg *msg)
{
	t_msg	*beginlst;
	t_msg	*current;

	beginlst = *lstmsg;
	current = beginlst;
	if (!*lstmsg)
		beginlst = msg;
	else
	{
		while (current->next)
			current = current->next;
		current->next = msg;
	}
	return (beginlst);
}

int		lenmsg(t_msg *msg)
{
	int		length;
	t_msg	*mmsg;

	mmsg = msg;
	length = 0;
	while (mmsg)
	{
		length++;
		mmsg = mmsg->next;
	}
	return (length);
}

void	writemsg(t_client *client, char *cmd)
{
	t_msg	*tmp;
	int		i;
	char **tab;

	i = 0;
	tab = ft_strsplit(cmd, '\n');
	while(tab[i])
	{
		client->msg = addmsg(&client->msg, newmsg(tab[i], client));
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

void clearmsg(t_client *client)
{
	t_msg *tmp;

	while (client->msg != NULL)
	{
		tmp = client->msg->next;
		free(client->msg->text);
		free(client->msg);
		client->msg = tmp;
	}
}

void	writemsglocal(t_client *client, char *cmd)
{
	t_msg	*tmp;
	int		i;
	char **tab;

	i = 0;
	tab = ft_strsplit(cmd, '\n');
	while(tab[i])
	{
		client->msglocal = addmsg(&client->msglocal, newmsg(tab[i], client));
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
}

void clearmsglocal(t_client *client)
{
	t_msg *tmp;

	while (client->msglocal != NULL)
	{
		tmp = client->msglocal->next;
		free(client->msglocal->text);
		free(client->msglocal);
		client->msglocal = tmp;		
	}
}
