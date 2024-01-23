/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyferrei <cyferrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 09:19:02 by cyferrei          #+#    #+#             */
/*   Updated: 2023/12/06 14:27:48 by cyferrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*ft_strdup(const char *s)
{
	char	*dest;
	int		size;
	int		i;

	i = 0;
	size = 0;
	while (s[size])
		size++;
	dest = malloc((sizeof(char) * (size + 1)));
	if (!dest)
		return (NULL);
	while (s[i])
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

static char	*read_file(char *final_buffer, int fd)
{
	int		bytes_read;
	char	*buffer;

	buffer = ft_calloc(sizeof(char), BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (bytes_read != 0 && !(ft_strchr(buffer, '\n')))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buffer);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		if (!final_buffer)
			final_buffer = ft_strdup("");
		final_buffer = ft_strjoin(final_buffer, buffer);
	}
	free(buffer);
	return (final_buffer);
}

char	*extract_line(char *buffer)
{
	int		len;
	char	*line;

	len = 0;
	if (!*buffer)
		return (NULL);
	while (buffer[len] != '\n' && buffer[len] != '\0')
		len++;
	line = ft_calloc((len + 2), sizeof(char));
	if (!line)
		return (NULL);
	ft_memcpy(line, buffer, len + 1);
	line[len + 1] = '\0';
	return (line);
}

char	*obtain_rest(char *buffer)
{
	int		len;
	int		rest_len;
	char	*rest;

	len = 0;
	if (!buffer)
		return (NULL);
	while (buffer[len] != '\n' && buffer[len])
		len++;
	if (buffer[len] == '\n')
		len++;
	rest_len = ft_strlen(buffer) - len;
	rest = ft_calloc((rest_len + 2), sizeof(char));
	if (!rest)
		return (NULL);
	ft_memcpy(rest, buffer + len, rest_len);
	rest[rest_len + 1] = '\0';
	free(buffer);
	return (rest);
}

char	*get_next_line(int fd)
{
	static char	*final_buffer[1024];
	char		*line;

	if (fd < 0 || read(fd, NULL, 0) < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!final_buffer[fd] || !ft_strchr(final_buffer[fd], '\n'))
		final_buffer[fd] = read_file(final_buffer[fd], fd);
	if (!final_buffer[fd])
		return (NULL);
	line = extract_line(final_buffer[fd]);
	final_buffer[fd] = obtain_rest(final_buffer[fd]);
	if (final_buffer[fd][0] == '\0')
	{
		free(final_buffer[fd]);
		final_buffer[fd] = NULL;
	}
	return (line);
}
