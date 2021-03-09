/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welease <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 16:40:48 by welease           #+#    #+#             */
/*   Updated: 2020/10/31 21:58:44 by welease          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	char	*dub;
	int		i;

	i = 0;
	while (*(src + i))
		i++;
	i++;
	dub = (char*)malloc(i);
	if (dub == NULL)
		return (NULL);
	i = 0;
	while (*(src + i))
	{
		*(dub + i) = *(src + i);
		i++;
	}
	*(dub + i) = '\0';
	return (dub);
}
