
#ifndef RETRO_MFA_H
# define RETRO_MFA_H


# include <fcntl.h>
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_data;

typedef struct s_mfa
{
	void	*mlx_connection;
	void	*mlx_window;
	char	*name;
	t_data	image;
}	t_mfa

#endif // RETRO_MFA_H