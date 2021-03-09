//
// Created by Quaro Tamarillo on 2/19/21.
//

#include "libft.h"

void	ft_free_strstr(char **splitted)
{
    size_t	i;

    i = 0;
    while (splitted[i])
    {
        free(splitted[i]);
        i++;
    }
    free(splitted);
    splitted = NULL;
}