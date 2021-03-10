/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qtamaril <qtamaril@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/20 12:18:24 by qtamaril          #+#    #+#             */
/*   Updated: 2020/09/07 15:47:22 by qtamaril         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.hpp"

size_t	ft_strlen2(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strchr2(char *s, int c)
{
	char	*s2;
	char	symbol;

	s2 = (char *)s;
	symbol = (char)c;
	while (*s2 != symbol)
	{
		if (*s2 == '\0')
			return (0);
		s2++;
	}
	return (s2);
}

char	*ft_strcpy(char *dest, char *src)
{
	int i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strdup2(char *s)
{
	int		length;
	char	*str;

	length = ft_strlen2(s);
	str = NULL;
	if (!(str = static_cast<char*>(malloc(sizeof(char) * (length + 1)))))
		return (NULL);
	ft_strcpy(str, s);
	return (str);
}

char	*ft_strjoin_new(char *s1, char *s2)
{
	char		*strjoin;
	size_t		i;
	size_t		j;

	i = 0;
	j = 0;
	strjoin = NULL;
	if (!s1 && !s2)
		return (NULL);
	if (!s1 || !s2)
		return (!s1 ? ft_strdup2(s2) : ft_strdup2(s1));
	if (!(strjoin = static_cast<char*>(malloc((ft_strlen2(s1) + ft_strlen2(s2) + 1) * sizeof(char)))))
		return (NULL);
	while (s1[j])
		strjoin[i++] = s1[j++];
	free(s1);
	j = 0;
	while (s2[j])
		strjoin[i++] = s2[j++];
	strjoin[i] = '\0';
	return (strjoin);
}
