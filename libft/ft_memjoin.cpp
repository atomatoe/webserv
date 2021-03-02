//
// Created by Wynn Elease on 2/18/21.
//

#include "libft.h"

char		*ft_memjoin(char *s1, char *s2, size_t len1, size_t len2)
{
	char	*res;
	size_t	i1 = 0;
	size_t	i2 = 0;
	size_t	i3 = 0;

	res = (char *)malloc(len1 + len2);
	if (res == NULL)
		return (NULL);
	memcpy(res, s1, len1);
	//while (i1 < len1)
	//	res[i3++] = s1[i1++];
	memcpy(res + len1, s2, len2);
	//while (i2 < len2)
	//	res[i3++] = s2[i2++];
	return (res);
}