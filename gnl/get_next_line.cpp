/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qtamaril <qtamaril@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/20 12:18:28 by qtamaril          #+#    #+#             */
/*   Updated: 2020/09/06 10:44:28 by qtamaril         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.hpp"

static int	ft_create_line(char **current_fd, char **line,
	char *p_newline, char *tmp)
{
	*p_newline++ = 0;
	free(tmp);
	if (!(*line = ft_strdup2(*current_fd)))
		return (-1);
	if (!p_newline)
	{
		free(*current_fd);
		current_fd = NULL;
		return (1);
	}
	*current_fd = ft_strcpy(*current_fd, p_newline);
	return (1);
}

static int	ft_get_remainder(char **current_fd,
	char **line, char *tmp, int byte_readed)
{
	char	*p_end;

	if (byte_readed < 0)
	{
		free(tmp);
		return (-1);
	}
	if (*current_fd && (p_end = ft_strchr2(*current_fd, '\n')))
		return (ft_create_line(current_fd, line, p_end, tmp));
	if (*current_fd)
	{
		free(tmp);
		*line = *current_fd;
		*current_fd = NULL;
		return (0);
	}
	free(tmp);
	*line = ft_strdup2((char *)"");
	return (0);
}

static int	ft_check_line(char **current_fd, char **line, char *tmp,
	int byte_readed)
{
	char	*p_end;

	tmp[byte_readed] = 0;
	if (!(*current_fd = ft_strjoin_new(*current_fd, tmp)))
	{
		free(tmp);
		return (-1);
	}
	if ((p_end = ft_strchr2(*current_fd, '\n')))
		return (ft_create_line(current_fd, line, p_end, tmp));
	return (2);
}

int			get_next_line(int fd, char **line)
{
	static char		*p_fd[1024];
	int				byte_readed;
	int				result;
	char			*tmp;
	int             BUFFER_SIZE;

	BUFFER_SIZE = 32;
	if (fd < 0 || !line || BUFFER_SIZE <= 0 || read(fd, p_fd, 0) < 0
	|| fd > 1024 || !(tmp = static_cast<char*>(malloc(BUFFER_SIZE + 1))))
		return (-1);
	while ((byte_readed = read(fd, tmp, BUFFER_SIZE)) > 0)
		if ((result = ft_check_line(&p_fd[fd], line, tmp, byte_readed)) != 2)
			return (result);
	return (ft_get_remainder(&p_fd[fd], line, tmp, byte_readed));
}
