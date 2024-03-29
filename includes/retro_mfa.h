/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retro_mfa.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@42>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 22:25:45 by miheider          #+#    #+#             */
/*   Updated: 2024/03/17 22:25:47 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RETRO_MFA_H
# define RETRO_MFA_H


# include <mlx.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <signal.h>
# include <unistd.h>

typedef struct s_list t_list;

typedef struct s_image
{
	size_t	width;
	size_t	height;
	void	*win_ptr;
	void	*img_ptr;
	char	*data;
}	t_image;

typedef struct s_input
{
	char 	*content;
	size_t	size;
}	t_input;

typedef struct s_data // instead of thousands variables
{
	void	*mlx_ptr;
	size_t	nb_images;
	t_list	*images;
	t_input	input;
}	t_data;

typedef struct s_list
{
	t_image			content;
	struct s_list	*next;
}	t_list;

# define HEXA "0123456789ABCDEF"
# define ABS(x) (x < 0 ? -1 * x : x)
# define TO_INT(x) ((255 + x) % 255)

# define BUFFER_SIZE 1024

// list utils
t_list	*createNode(void *mlx_img, char *data, size_t width, size_t height);
void	appendNode(t_data *data, t_list *node);
void	deleteList(t_data *data);

// file reader
void	readAllBytes(const char *filename, t_input *input);


#endif // RETRO_MFA_H