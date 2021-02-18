/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welease <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 19:12:26 by welease           #+#    #+#             */
/*   Updated: 2020/10/31 21:39:20 by welease          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *arr, int c, size_t n)
{
	size_t			i;
	unsigned char	c1;

	c1 = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (((unsigned char*)arr)[i] == c1)
			return ((unsigned char *)arr + i);
		i++;
	}
	return (NULL);
}
