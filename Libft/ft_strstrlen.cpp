//
// Created by Quaro Tamarillo on 2/19/21.
//

#include "libft.h"

size_t		ft_strstrlen(char **splitted)
{
    size_t	i;

    i = 0;
    while (splitted[i])
        i++;
    return (i);
}