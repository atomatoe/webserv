/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welease <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 16:45:04 by welease           #+#    #+#             */
/*   Updated: 2020/11/01 16:05:36 by welease          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void			add_nbr(int n, char *m, int i)
{
	long long int	a;

	a = n;
	if (a / 10)
		add_nbr(a / 10, m, i - 1);
	m[i] = a % 10 + '0';
}

static char			*if_negative(char *res, int n)
{
	while (n)
	{
		res[n] = res[n - 1];
		n--;
	}
	res[0] = '-';
	return (res);
}

char				*ft_itoa(int n)
{
	int				i;
	int				tmp;
	char			*res;

	i = 0;
	tmp = n;
	if (n == 0)
		return (res = ft_strdup("0"));
	if (tmp == -2147483648)
		return (res = ft_strdup("-2147483648"));
	while (n)
	{
		n = n / 10;
		i++;
	}
	if ((res = (char *)malloc(i + 2)) == NULL)
		return (NULL);
	res[i + 1] = '\0';
	res[i] = '\0';
	add_nbr(tmp > 0 ? tmp : tmp * -1, res, i - 1);
	if (tmp < 0)
		return (if_negative(res, i));
	return (res);
}
