//
// Created by Quaro Tamarillo on 2/19/21.
//

#include "libft.h"

bool	ft_str_is_num(char const *str)
{
    int j;

    j = 0;
    while (str[j]) {
        if (!ft_isdigit(str[j]))
            return (false);
        j++;
    }
    return (true);
}
