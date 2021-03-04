/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welease <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 15:41:46 by welease           #+#    #+#             */
/*   Updated: 2020/11/01 15:41:35 by welease          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <cstring>
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	index;
	char	*result_string;

	index = 0;
	if (s == NULL)
		return (NULL);
	if (start >= strlen(s))
		return (result_string = strdup(""));
	result_string = (char *)malloc(len + 1);
	if (result_string == NULL)
		return (NULL);
	memmove(result_string, s + start, len);
	//while ((index < len))
	//	result_string[index++] = s[start++];
	result_string[len] = '\0';
	return (result_string);
}
