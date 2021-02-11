/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welease <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 16:29:03 by welease           #+#    #+#             */
/*   Updated: 2020/11/01 15:55:08 by welease          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	ch;
	size_t	size;

	ch = (unsigned char)c;
	size = ft_strlen(s);
	while (size + 1)
	{
		if (s[size] == ch)
			return ((char *)s + size);
		size--;
	}
	return (NULL);
}
