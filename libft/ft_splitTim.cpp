//
// Created by Quaro Tamarillo on 2/19/21.
//

#include "libft.h"

static void		ft_skip_c(char const **s, char c)
{
    while (**s == c && **s)
        (*s)++;
}

static int		ft_wordlength(char const *s, char c)
{
    size_t len;

    len = 0;
    while (s[len] != c && s[len])
        len++;
    return (len);
}

static size_t	ft_countwords(char const *s, char c)
{
    size_t words;

    words = 0;
    ft_skip_c(&s, c);
    while (*s)
    {
        while (*s != c && *s)
            s++;
        words++;
        ft_skip_c(&s, c);
    }
    return (words);
}

static char		**ft_memfree(char **splitted)
{
    size_t	i;

    i = 0;
    while (splitted[i])
    {
        free(splitted[i]);
        i++;
    }
    free(splitted);
    return (NULL);
}

char			**ft_splitTim(char const *s, char c)
{
    size_t	i;
    size_t	j;
    size_t	words;
    char	**splitted;

    if (!s)
        return (NULL);
    words = ft_countwords(s, c);
    if (!(splitted = (char**)malloc(sizeof(char*) * (words + 1))))
        return (NULL);
    i = 0;
    ft_skip_c(&s, c);
    while (i < words)
    {
        if (!(splitted[i] = (char*)malloc(sizeof(char) *
                                          (ft_wordlength(s, c) + 1))))
            return (ft_memfree(splitted));
        j = 0;
        while (*s != c && *s)
            splitted[i][j++] = *s++;
        splitted[i++][j] = '\0';
        ft_skip_c(&s, c);
    }
    splitted[i] = NULL;
    return (splitted);
}