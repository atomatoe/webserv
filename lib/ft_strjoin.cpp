/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welease <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 16:07:26 by welease           #+#    #+#             */
/*   Updated: 2020/11/01 14:11:50 by welease          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strjoin(char const *s1, char const *s2)
{
	char	*res;
	size_t	index1;
	size_t	index2;

	if (!s1 || !s2)
	{
		res = ft_substr("", 0, 1);
		ft_bzero(res, 1);
		return (res);
	}
	index1 = ft_strlen(s1);
	index2 = ft_strlen(s2);
	res = (char *)malloc(index1 + index2 + 1);
	if (res == NULL)
		return (NULL);
	index1 = 0;
	while (*s1)
		res[index1++] = *s1++;
	while (*s2)
		res[index1++] = *s2++;
	res[index1] = '\0';
	return (res);
}
