/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/11 16:21:21 by ncolomer          #+#    #+#             */
/*   Updated: 2019/11/11 17:47:21 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

typedef struct	s_str
{
	char			*content;
	struct s_str	*next;
}				t_str;

int
	ft_strlen(char const *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char
	*ft_strdup(char const *str)
{
	char	*cpy;
	int		i;
	int		j;

	if (!(cpy = (char*)malloc(sizeof(*cpy) * (ft_strlen(str) + 1))))
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
		cpy[j++] = str[i++];
	cpy[j] = 0;
	return (cpy);
}

char
	*ft_substr(char const *str, int start, int length)
{
	int		i;
	int		str_length;
	char	*cpy;

	str_length = ft_strlen(str);
	if (!(cpy = (char*)malloc(sizeof(*cpy) * (str_length + 1))))
		return (NULL);
	i = 0;
	while (start + i < str_length && i < length)
	{
		cpy[i] = str[start + i];
		i++;
	}
	return (cpy);
}

int
	ft_strchr(char const *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
		if ((unsigned char)str[i++] == (unsigned char)c)
			return (1);
	return (0);
}

t_str
	*str_add(t_str **str, char *content)
{
	t_str	*new;
	t_str	*first;

	if (!content)
		return (NULL);
	if (!(new = (t_str*)malloc(sizeof(*new))))
		return (NULL);
	new->content = content;
	new->next = NULL;
	if (!*str)
		*str = new;
	else
	{
		first = *str;
		while ((*str)->next)
			*str = (*str)->next;
		(*str)->next = new;
		*str = first;
	}
	return (new);
}

int
	str_strchr(t_str *str, char c)
{
	while (str)
	{
		if (ft_strchr(str->content, c))
			return (1);
		str = str->next;
	}
	return (0);
}

int
	str_clear(t_str **str)
{
	t_str	*tmp;

	while (*str)
	{
		tmp = (*str)->next;
		free((*str)->content);
		free(*str);
		*str = tmp;
	}
	return (0);
}

int
	str_length(t_str *str)
{
	int	total;
	int	i;

	total = 0;
	while (str)
	{
		i = 0;
		while (str->content[i] && str->content[i] != '\n')
			i++;
		total += i;
		str = str->next;
	}
	return (total);
}

int
	str_write(char **line, t_str **str)
{
	int		i;
	int		j;
	int		length;
	char	*tmp;
	t_str	*stmp;

	length = str_length(*str);
	if (!(tmp = (char*)malloc(sizeof(*tmp) * (length + 1))))
		return (0);
	*line = tmp;
	j = 0;
	while (*str)
	{
		i = 0;
		while((*str)->content[i] && (*str)->content[i] != '\n')
			(*line)[j++] = (*str)->content[i++];
		if (!(*str)->content[i])
		{
			stmp = (*str)->next;
			free((*str)->content);
			free(*str);
			*str = stmp;
		}
		else
		{
			(*line)[j] = 0;
			j = 0;
			i++;
			while ((*str)->content[i])
				(*str)->content[j++] = (*str)->content[i++];
			(*str)->content[j] = 0;
			return (1);
		}
	}
	(*line)[j] = 0;
	return (1);
}

int
	read_until_nl(t_str **str)
{
	char	buffer[129];
	int		i;
	int		r;
	int		total;
	int		stop;

	stop = 0;
	total = 0;
	i = 0;
	while (i < 128)
		buffer[i++] = 0;
	while (!stop && (r = read(0, buffer, 128)) > 0)
	{
		buffer[r] = 0;
		total += r;
		if (!(str_add(str, ft_strdup(buffer))))
			return (-1);
		if (ft_strchr(buffer, '\n'))
			return (1);
	}
	return (total);
}

/*
** Written in ~55min (Without Visual Studio Code)
*/
int
	get_next_line(char **line)
{
	static t_str	*str = NULL;
	int				need_read;
	int				read_ret;

	need_read = 1;
	if (str && str_strchr(str, '\n'))
		need_read = 0;
	read_ret = 0;
	if (need_read)
		read_ret = read_until_nl(&str);
	if (read_ret < 0)
		return (str_clear(&str) | -1);
	if (!str)
		return (0);
	if (!str_write(line, &str))
		return (str_clear(&str) | -1);
	if (!str || !str_strchr(str, '\n'))
		read_ret = read_until_nl(&str);
	if (read_ret < 0)
		return (str_clear(&str) | -1);
	if (str)
		return (1);
	return (0);
}
