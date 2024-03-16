/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileReader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfrants <vfrants@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 15:31:34 by vfrants           #+#    #+#             */
/*   Updated: 2024/03/16 22:59:12 by vfrants          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/retro_mfa.h"

static char	*my_realloc(char *ptr, size_t size, char *buffer, size_t readSize)
{
	char *result = calloc(size + readSize, sizeof (char));

	if (!result)
	{
		free(ptr);
		return NULL;
	}

	if (ptr)
	{
		memcpy(result, ptr, size);
	}
	memcpy(result + size, buffer, readSize);
	free(ptr);
	return result;
}

void	readAllBytes(const char *filename, t_input *input)
{
	const int fd = open(filename, O_RDONLY);

	if (fd == -1)
	{
		return ;
	}

	size_t size = 0, readSize = 0;
	char buffer[BUFFER_SIZE + 1];
	char *result = NULL;

	bzero(buffer, BUFFER_SIZE + 1);
	while ((readSize = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		result = my_realloc(result, size, buffer, readSize);
		size += readSize;
	}
	close(fd);
	input->content = result;
	input->size = size;
}