//
// Created by Wynn Elease on 2/18/21.
//

#include "libft.h"

char		*ft_memjoin(char *s1, char *s2, size_t len1, size_t len2)
{
	char	*res;

	res = (char *)malloc(len1 + len2);
	if (res == NULL)
		return (NULL);
	memcpy(res, s1, len1);
	memcpy(res + len1, s2, len2);
	return (res);
}
