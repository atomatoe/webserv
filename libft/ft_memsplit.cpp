//
// Created by Wynn Elease on 2/19/21.
//

#include "libft.h"
#include <iostream>
static void			line_getting(char **s, size_t *ll, char *c, size_t *size, size_t len){
	char *tmp;

	*s = *s + *ll;
	if (*ll && *ll != len)
		*s += len;
	*ll = 0;

	if ((tmp = ft_memmem(*s, *size, c, len))){
		*ll = tmp - *s;
		if (*ll == len)
			*size = *size - *ll;
		else
			*size = *size - *ll - len;
	}
	else
		*size = 0;
}

memBody	*ft_memsplit(char *s, char *c, size_t size, size_t len)
{
	memBody		*mass;
	char		*line = s;
	size_t		ll = 0;
	size_t 		tmp_size = size;

	if (!s)
		return (new memBody());
	mass = new memBody();
	while (tmp_size){
		line_getting(&line, &ll, c, &tmp_size, len);
		//std::cout << "+++++" << ft_memdup(line, ll) << "+++++" << std::endl;
		if (ll == 0)
			continue;
		mass->push_back(std::pair<size_t, char *>(ll, ft_memdup(ll == len ? "" : line, ll)));
	}
	return (mass);
}