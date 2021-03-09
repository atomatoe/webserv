//
// Created by Quaro Tamarillo on 2/19/21.
//

#include "libft.h"

bool	ft_isspace(int c)
{
    c = (unsigned char)c;
    if (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
        || c == ' ')
        return (true);
    return (false);
}