/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welease <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 16:34:53 by welease           #+#    #+#             */
/*   Updated: 2020/10/31 21:51:28 by welease          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	checkspace(char *str)
{
	if (*str == ' ' ||
		*str == '\t' ||
		*str == '\n' ||
		*str == '\v' ||
		*str == '\f' ||
		*str == '\r')
		return (1);
	return (0);
}

int			ft_atoi(char *str)
{
	int	res;
	int	minus;

	res = 0;
	minus = 0;
	while (checkspace(str))
		str++;
	if ((*str == '-') || (*str == '+'))
	{
		if (*str == '-')
			minus++;
		str++;
	}
	while ((*str >= '0') && (*str <= '9') && *str)
	{
		res = res * 10 + *str - 48;
		str++;
	}
	if ((minus % 2) == 1)
		res = res * -1;
	return (res);
}
