/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfrants <vfrants@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 23:16:29 by vfrants           #+#    #+#             */
/*   Updated: 2024/03/17 22:53:54 by vfrants          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/retro_mfa.h"

t_data g_data = {NULL, 0ul, NULL, {NULL, 0}};

void	cleanExit(int status)
{
	deleteList(&g_data);
	free(g_data.input.content);
	mlx_destroy_display(g_data.mlx_ptr);
	free(g_data.mlx_ptr);
	exit(status);
}

int	hook_destroy(void)
{
	cleanExit(0);
	return 0;
}

void	fillImage888(char *image, const unsigned char *content, int width, int height)
{
	for (int step = 0; step < width * height * 3; step += 3)
	{
		uint32_t rgb32 = (content[step + 2]) << 16
			| (content[step + 1]) << 8
			| (content[step + 0]);

		*(unsigned int *)(image + (step / 3) * 4) = rgb32;
	}
}

void	appendImage888(const char *content, int width, int height)
{
	int bpp = 0, size_line = 0, endian = 0;

	void *img_ptr = mlx_new_image(g_data.mlx_ptr, width, height);
	if (!img_ptr)
	{
		printf("Error: Unable to create image\n");
		cleanExit(1);
	}

	char *data_ptr = mlx_get_data_addr(img_ptr, &bpp, &size_line, &endian);

	fillImage888(data_ptr, (unsigned char *) content, width, height);

	t_list *node = createNode(img_ptr, data_ptr, width, height);

	if (!node)
	{
		printf("Error: Unable to create node\n");
		mlx_destroy_image(g_data.mlx_ptr, img_ptr);
		cleanExit(1);
	}

	appendNode(&g_data, node);
}

void	fillImage555(char *image, const unsigned char *content, int width, int height)
{
	for (int step = 0; step < width * height * 2; step += 2)
	{
		uint32_t rgb16 = (content[step]) << 8 | (content[step + 1]);

		int r = (rgb16 >> 10) & 0x1F;
		int g = (rgb16 >> 5) & 0x1F;
		int b = rgb16 & 0x1F;

		uint32_t rgb32 = (r * 255) / 31 * 0x10000 | (g * 255) / 31 * 0x100 | (b * 255) / 31;
		*(unsigned int *)(image + step * 2) = rgb32;
	}
}

void	appendImage555(const char *content, int width, int height)
{
	int bpp = 0, size_line = 0, endian = 0;

	void *img_ptr = mlx_new_image(g_data.mlx_ptr, width, height);
	if (!img_ptr)
	{
		printf("Error: Unable to create image\n");
		cleanExit(1);
	}

	char *data_ptr = mlx_get_data_addr(img_ptr, &bpp, &size_line, &endian);

	fillImage555(data_ptr, (unsigned char *)content, width, height);

	t_list *node = createNode(img_ptr, data_ptr, width, height);

	if (!node)
	{
		printf("Error: Unable to create node\n");
		mlx_destroy_image(g_data.mlx_ptr, img_ptr);
		cleanExit(1);
	}

	appendNode(&g_data, node);
}

void	executeImages(void)
{
	t_list *iter = g_data.images;

	while (iter)
	{
		iter->content.win_ptr = mlx_new_window(
			g_data.mlx_ptr,
			iter->content.width,
			iter->content.height,
			"RetroMFA"
		);
		if (iter->content.win_ptr == NULL)
		{
			printf("Error: Unable to create window\n");
			cleanExit(1);
		}
		mlx_put_image_to_window(
			g_data.mlx_ptr,
			iter->content.win_ptr,
			iter->content.img_ptr,
			0,
			0
		);
		mlx_hook(iter->content.win_ptr, 2, 1L<<0, hook_destroy, NULL);
		mlx_hook(iter->content.win_ptr, 17, 0, hook_destroy, NULL);
		iter = iter->next;
	}
}

void singalHandler(int signum)
{
	if (signum == SIGINT)
		cleanExit(0);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Usage: ./%s <filename>\n", argv[0]);
		return 1;
	}

	g_data.mlx_ptr = mlx_init();
	if (!g_data.mlx_ptr)
	{
		printf("Error: Unable to initialize mlx\n");
		return 1;
	}

	readAllBytes(argv[1], &g_data.input);
	if (g_data.input.content == NULL)
	{
		printf("Error: Unable to read file\n");
		cleanExit(1);
	}

	int images = 0;
	for (size_t i = 3; i < g_data.input.size; ++i)
	{
		if (images >= 100)
		{
			break;
		}
		if ((g_data.input.content[i - 3] == 0 && g_data.input.content[i - 2] == 6)
			&& (g_data.input.content[i - 1] == 16 && g_data.input.content[i] == 0))
		{
			if (TO_INT(g_data.input.content[i - 6]) + (TO_INT(g_data.input.content[i - 5]) << 8) <= 0
				|| TO_INT(g_data.input.content[i - 4]) + (TO_INT(g_data.input.content[i - 3]) << 8) <= 0)
			{
				printf("Error: Invalid image size\n");
			}
			else
			{
				appendImage555(g_data.input.content + i + 17,
					TO_INT(g_data.input.content[i - 6]) + (TO_INT(g_data.input.content[i - 5]) << 8),
					TO_INT(g_data.input.content[i - 4]) + (TO_INT(g_data.input.content[i - 3]) << 8));
				images++;
			}
		}
		if ((g_data.input.content[i - 3] == 0 && g_data.input.content[i - 2] == 4)
			&& (g_data.input.content[i - 1] == 16 && g_data.input.content[i] == 0))
		{
			if (TO_INT(g_data.input.content[i - 6]) + (TO_INT(g_data.input.content[i - 5]) << 8) <= 0
				|| TO_INT(g_data.input.content[i - 4]) + (TO_INT(g_data.input.content[i - 3]) << 8) <= 0)
			{
				printf("Error: Invalid image size\n");
			}
			else
			{
				appendImage888(g_data.input.content + i + 17,
					TO_INT(g_data.input.content[i - 6]) + (TO_INT(g_data.input.content[i - 5]) << 8),
					TO_INT(g_data.input.content[i - 4]) + (TO_INT(g_data.input.content[i - 3]) << 8));
				images++;
			}
		}
	}

	executeImages();

	mlx_loop(g_data.mlx_ptr);

	cleanExit(0);
	return 0;
}
