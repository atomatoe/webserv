//
// Created by Wynn Elease on 2/21/21.
//
#include "libft.h"
#include <string.h>

char *ft_memmem(char *haystack, size_t hlen, char const *needle, size_t nlen) {
	if (nlen == 0)
		return haystack;
	if (hlen < nlen)
		return NULL;
	char *hlimit = haystack + hlen - nlen + 1;
	while ((haystack = static_cast<char *>(memchr(haystack, needle[0], hlimit - haystack)))) {
		if (!memcmp(haystack, needle, nlen))
			return haystack;
		haystack++;
	}
	return NULL;
}