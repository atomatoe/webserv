/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welease <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 16:45:52 by welease           #+#    #+#             */
/*   Updated: 2020/11/01 14:33:30 by welease          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	i;
	size_t	size;
	char	*result;

	i = 0;
	if (!s)
		return (ft_strdup(""));
	size = ft_strlen(s);
	result = (char *)malloc(size + 1);
	if (result == NULL)
		return (NULL);
	result[size] = '\0';
	while (i < size)
	{
		result[i] = ((*f)((unsigned int)i, s[i]));
		i++;
	}
	return (result);
}
