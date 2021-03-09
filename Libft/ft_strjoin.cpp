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

char	*ft_strjoin(char const *s1, char const *s2)
{
	char		*new_str;
	size_t		len;

	if (!s1 || !s2)
		return (NULL);
	len = strlen(s1) + strlen(s2);
	if (!(new_str = (char *)malloc(len + 1)))
		return (NULL);
	strcpy(new_str, s1);
	strcat(new_str, s2);
	return (new_str);
}

