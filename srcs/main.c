/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfrants <vfrants@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 23:16:29 by vfrants           #+#    #+#             */
/*   Updated: 2024/03/17 00:57:05 by vfrants          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/retro_mfa.h"

void	cleanExit(t_data *data, int status)
{
	deleteList(data);
	free(data->input.content);
	printf("BEFORE\n");
	printf("ptr = %p\n", data->mlx_ptr);
	mlx_destroy_display(data->mlx_ptr);
	printf("AFTER\n");
	free(data->mlx_ptr);
	exit(status);
}

char	*extractImage565(const char *content, int width, int height)
{
	// printf("BREAKPOINT 2\n");
	// printf("width = %d, height = %d\n", width, height);
	char *image = calloc(width * height * 2, sizeof (int));

	if (!image)
	{
		return NULL;
	}

	int iterated = 0;

	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			unsigned int color = 0;
			uint16_t rgb16 = (content[(row * width + col) * 2 + 1] << 8) | content[(row * width + col) * 2];

	    	color = (((rgb16 & 0xF800) >> 11) * 255) / 31;
	    	color |= (((rgb16 & 0x07E0) >> 5) * 255) / 63;
	    	color |= (rgb16 & 0x001F * 255) / 31;

			*(unsigned int *)(image + iterated) = color;
			iterated += 4;
		}
	}

	// for (int i = 0; i < width * height * 2; i += 2)
	// {
	// 	unsigned int color = 0;
	// 	uint16_t rgb16 = (content[i + 1] << 8) | content[i];

    // 	color = (((rgb16 & 0xF800) >> 11) * 255) / 31;
    // 	color |= (((rgb16 & 0x07E0) >> 5) * 255) / 63;
    // 	color |= (rgb16 & 0x001F * 255) / 31;

	// 	*(unsigned int *)(image + iterated) = color;
	// 	iterated += 4;
	// }

	// char	*dst;

	// if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
	// {
	// 	dst = data->img.addr + (y * data->img.l_l + x * (data->img.bpp / 8));
	// 	*(unsigned int *)dst = color;
	// }

	return image;
}

void	appendImage565(t_data *data, const char *content, int width, int height)
{
	int bpp = 0, size_line = 0, endian = 0;

	char *image = extractImage565(content, width, height);
	if (!image)
	{
		printf("Error: Unable to extract image\n");
		cleanExit(data, 1);
	}

	void *img_ptr = mlx_new_image(data->mlx_ptr, width, height);
	if (!img_ptr)
	{
		printf("Error: Unable to create image\n");
		free(image);
		cleanExit(data, 1);
	}

	char *data_ptr = mlx_get_data_addr(img_ptr, &bpp, &size_line, &endian);
	if (!data_ptr)
	{
		printf("Error: Unable to get image data\n");
		free(image);
		mlx_destroy_image(data->mlx_ptr, img_ptr);
		cleanExit(data, 1);
	}

	t_list *node = createNode(img_ptr, image, width, height);
	free(image);

	if (!node)
	{
		printf("Error: Unable to create node\n");
		mlx_destroy_image(data->mlx_ptr, img_ptr);
		cleanExit(data, 1);
	}

	memcpy(data_ptr, content, width * height * 2);
	appendNode(data, node);
}

void	executeImage(t_data *data, t_list *node)
{
	data->win_ptr = mlx_new_window(data->mlx_ptr, node->content.width, node->content.height, "RetroMFA");

	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, node->content.img_ptr, 0, 0);
	mlx_loop(data->mlx_ptr);
}

int main(int argc, char **argv)
{
	t_data data = {NULL, NULL, 0ul, NULL, {NULL, 0}};

	if (argc != 2)
	{
		printf("Usage: ./%s <filename>\n", argv[0]);
		return 1;
	}

	data.mlx_ptr = mlx_init();
	if (!data.mlx_ptr)
	{
		printf("Error: Unable to initialize mlx\n");
		return 1;
	}

	readAllBytes(argv[1], &data.input);
	if (data.input.content == NULL)
	{
		printf("Error: Unable to read file\n");
		cleanExit(&data, 1);
	}

	// for (size_t i = 0; i < data.input.size; i++)
	// {
	// 	printf("%c%c ", HEXA[ABS(data.input.content[i] >> 4)],
	// 		HEXA[ABS((data.input.content[i] & 0xF))]);
	// 	printf("%d ", (ABS(data.input.content[i] >> 4) << 4) +
	// 		ABS((data.input.content[i] & 0xF)));
	// 	if (i % 16 == 15)
	// 	{
	// 		printf("\n");
	// 	}
	// }

	// printf("BREAKPOINT 1\n");

	for (size_t i = 3; i < data.input.size; ++i)
	{
		if ((data.input.content[i - 3] == 0 && data.input.content[i - 2] == 6)
			&& (data.input.content[i - 1] == 16 && data.input.content[i] == 0))
		{

			// printf("Image size: width %d, height %d ", data.input.content[i - 6],
				// data.input.content[i - 4]);

			// printf("6 last: %d %d %d %d %d %d\n", data.input.content[i - 6],
			// 	data.input.content[i - 5], data.input.content[i - 4],
			// 	data.input.content[i - 3], data.input.content[i - 2],
			// 	data.input.content[i - 1]);

			appendImage565(&data, data.input.content + i - 6,
				(ABS(data.input.content[i - 6] >> 4) << 4) +
					ABS((data.input.content[i - 6] & 0xF)),
				(ABS(data.input.content[i - 4] >> 4) << 4) +
					ABS((data.input.content[i - 4] & 0xF)));
			// printf("Found header %lu, width %d, height %d\n", i, data.input.content[i - 6], data.input.content[i - 4]);
		}
	}


	for (t_list *tmp = data.images; tmp; tmp = tmp->next)
	{
		executeImage(&data, tmp);
	}

	cleanExit(&data, 0);
	return 0;
}
