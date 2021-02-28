//
// Created by Wynn Elease on 2/18/21.
//

#include "libft.h"

char	*ft_memdup(char const *src, size_t size) {
	char	*dub;
	if ((dub = (char*)malloc(size + 1)) == NULL)
		return NULL;
	memcpy((void *)dub, (void *)(src), size);
	return (dub);
}
