/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/11 14:30:34 by ncolomer          #+#    #+#             */
/*   Updated: 2019/11/11 16:10:08 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>

# define ABS(x)	(x < 0 ? -x : x)

typedef struct	s_str
{
	char		*content;
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
	*ft_substr(char const *str, int start, int length)
{
	int		i;
	char	*cpy;
	int		str_length;

	if (!(cpy = (char*)malloc(sizeof(*cpy) * (length + 1))))
		return (NULL);
	str_length = ft_strlen(str);
	i = 0;
	while (start + i < str_length && i < length)
	{
		cpy[i] = str[start + i];
		i++;
	}
	cpy[i] = 0;
	return (cpy);
}

int
	ft_in_set(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

int
	ft_is_digit(char c)
{
	return (c >= '0' && c <= '9');
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
	str_write(t_str *str)
{
	int	i;
	int	total;

	total = 0;
	while (str)
	{
		i = ft_strlen(str->content);
		total += i;
		if (i > 0)
			write(1, str->content, i);
		str = str->next;
	}
	return (total);
}

int
	parse_flags(char const *flags, int *width, int *precision)
{
	int	i;
	int	state;
	int	tmp;

	*width = -1;
	*precision = -1;
	i = 0;
	while (flags[i])
	{
		if (flags[i] == '.')
		{
			state = 1;
			*precision = 0;
			i++;
		}
		else if (ft_is_digit(flags[i]))
		{
			tmp = 0;
			while (ft_is_digit(flags[i]))
				tmp = (tmp * 10) + (flags[i++] - '0');
			if (state)
				*precision = tmp;
			else
				*width = tmp;
			state = 0;
		}
		else
			return (0);
	}
	return (1);
}

void
	*free_ptr(void *ptr)
{
	free(ptr);
	return (NULL);
}

char
	*copy_signed(int width, int precision, int val)
{
	char	*str;
	int		i;
	int		length;
	int		tmp;
	int		_length;
	long	_tmp;

	(void) precision;
	tmp = val;
	length = 1 + (val < 0);
	while (tmp > 9 || tmp < -9)
	{
		tmp /= 10;
		length++;
	}
	_length = length;
	if (width > length)
		_length = width;
	if (!(str = (char*)malloc(sizeof(*str) * (_length + 1))))
		return (NULL);
	i = 0;
	while (i < _length)
		str[i++] = ' ';
	str[_length] = 0;
	if (val == 0)
		str[_length - 1] = '0';
	i = _length - 1;
	tmp = val;
	while (tmp != 0)
	{
		_tmp = tmp;
		str[i--] = "0123456789"[ABS(_tmp) % 10];
		tmp /= 10;
	}
	if (val < 0)
		str[i] = '-';
	return (str);
}

char
	*copy_hex(int width, int precision, unsigned int val)
{
	char			*str;
	int				i;
	int				length;
	unsigned int	tmp;
	int				_length;

	(void) precision;
	tmp = val;
	length = 1;
	while (tmp > 15)
	{
		tmp /= 16;
		length++;
	}
	_length = length;
	if (width > length)
		_length = width;
	if (!(str = (char*)malloc(sizeof(*str) * (_length + 1))))
		return (NULL);
	i = 0;
	while (i < _length)
		str[i++] = ' ';
	str[_length] = 0;
	if (val == 0)
		str[_length - 1] = '0';
	i = _length - 1;
	while (val != 0)
	{
		str[i--] = "0123456789abcdef"[val % 16];
		val /= 16;
	}
	return (str);
}

char
	*copy_str(int width, int precision, char *str)
{
	int		i;
	int		j;
	int		start;
	int		length;
	int		_length;
	char	*cpy;

	if (!str)
		str = "(null)";
	length = ft_strlen(str);
	_length = length;
	if (width > length)
		_length = width;
	if (!(cpy = (char*)malloc(sizeof(*cpy) * (_length + 1))))
		return (NULL);
	i = 0;
	while (i < _length)
		cpy[i++] = ' ';
	cpy[_length] = 0;
	start = 0;
	if (width > length)
		start = width - length;
	j = 0;
	while (j < length && (precision < 0 || j < precision))
		cpy[start++] = str[j++];
	cpy[start] = 0;
	return (cpy);
}

t_str
	*add_format(t_str **res, char *flags, char format, va_list *args)
{
	int		width;
	int		precision;
	char	*str;

	if (!flags)
		return (0);
	if (!parse_flags(flags, &width, &precision))
		return (free_ptr(flags));
	free(flags);
	str = NULL;
	if (format == 'd')
		str = copy_signed(width, precision, va_arg(*args, int));
	else if (format == 'x')
		str = copy_hex(width, precision, va_arg(*args, unsigned int));
	else if (format == 's')
		str = copy_str(width, precision, va_arg(*args, char*));
	if (!str)
		return (0);
	return (str_add(res, str));
}

int
	clean_exit(t_str **str, va_list *args)
{
	va_end(*args);
	return (str_clear(str));
}

/*
** Written in ~1h40min... with Visual Studio Code after 1h so... under 2hours ? (I hope)
*/
int
	ft_printf(char const *str, ...)
{
	int		i;
	int		start;
	va_list	args;
	t_str	*res;
	int		length;

	res = NULL;
	length = ft_strlen(str);
	va_start(args, str);
	i = 0;
	while (i < length)
	{
		start = i;
		while (i < length && str[i] != '%')
			i++;
		if (i - start > 0 && !str_add(&res, ft_substr(str, start, i - start)))
			return (clean_exit(&res, &args));
		if (str[i] == '%' && (start = ++i) && i < length)
		{
			while (ft_in_set(str[i], ".0123456789"))
				i++;
			if (!add_format(&res, ft_substr(str, start, i - start), str[i], &args))
				return (clean_exit(&res, &args));
			i++;
		}
	}
	i = str_write(res);
	clean_exit(&res, &args);
	return (i);
}
