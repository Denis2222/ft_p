/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 13:46:48 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/10/24 13:49:04 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

# include <libft.h>

# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <dirent.h>
# include <strings.h>


# define INVALID_SOCKET	-1
# define SOCKET_ERROR	-1

typedef struct			s_client
{
	int					socket;
	int					connect;
	fd_set				fd_read;
	fd_set				fd_writ;
	struct sockaddr_in	sin;
	struct hostent		*hostinfo;
}						t_client;
