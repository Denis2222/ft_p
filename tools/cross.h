/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cross.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/24 13:46:48 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/11/04 06:32:33 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CROSS_H

# include <signal.h>
# include <sys/types.h>
# include <sys/socket.h>

# define CROSS_H

# ifndef SO_NOSIGPIPE
#  define SO_NOSIGPIPE 0
# endif

void    osx_pipe(int socket);
void    linux_pipe(void);

#endif