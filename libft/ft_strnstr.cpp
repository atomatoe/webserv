/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welease <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 16:30:08 by welease           #+#    #+#             */
/*   Updated: 2020/10/31 21:46:13 by welease          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	res(char *str, char *to_find, size_t i, size_t len)
{
	int		k;

	k = i;
	while ((*(str + k) == *to_find) && *to_find && len)
	{
		k++;
		to_find++;
		len--;
	}
	if (*to_find == '\0')
		return (1);
	return (0);
}

char			*ft_strnstr(char *str, char *to_find, size_t len)
{
	size_t	i;
	size_t	rest;

	i = 0;
	while (*(str + i) && len)
	{
		if (*(str + i) == *to_find)
		{
			rest = res(str, to_find, i, len);
			if (rest > 0)
				return (str + i);
		}
		i++;
		len--;
	}
	if (*to_find == 0)
		return (str);
	return (0);
}
