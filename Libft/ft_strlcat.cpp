/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welease <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 16:27:07 by welease           #+#    #+#             */
/*   Updated: 2020/10/31 21:42:57 by welease          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t i;

	i = 0;
	while (dst[i] && i < dstsize)
		i++;
	if (dstsize && i < dstsize - 1)
		i = i + ft_strlcpy(dst + i, src, dstsize - i);
	else
		i = i + ft_strlen(src);
	return (i);
}
