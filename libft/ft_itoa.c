/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/25 15:52:30 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/10/23 14:10:01 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long long int	ft_itoa_sign(long long int n)
{
	if (n >= 0)
		return (0);
	return (1);
}

static long long int	ft_itoa_length(long long int n)
{
	int	length;

	length = 1;
	if (ft_itoa_sign(n))
		n = n * -1;
	while (n > 9)
	{
		n = n / 10;
		length++;
	}
	return (length);
}

char					*ft_itoa(long long int n)
{
	char			*str;
	int				length;
	long long int	abs;

	length = ft_itoa_length(n) + ft_itoa_sign(n);
	abs = n;
	if (ft_itoa_sign(n))
		abs = n * -1;
	str = (char*)ft_memalloc(length + 1);
	if (!str)
		return (NULL);
	str[length] = '\0';
	while (length > 0)
	{
		length--;
		str[length] = (abs % 10) + 48;
		abs = abs / 10;
	}
	if (ft_itoa_sign(n))
		str[0] = '-';
	return (str);
}
