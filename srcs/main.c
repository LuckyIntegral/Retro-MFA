/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miheider <miheider@42>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 23:16:29 by vfrants           #+#    #+#             */
/*   Updated: 2024/03/16 12:49:52 by miheider         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/retro_mfa.h"

void	init(t_mfa *mfa)
{
	initialize mlx
}

int main(void) {
	t_mfa	mfa;

	FILE *file;

	file = fopen(../MFA/green.mfa, O_RDONLY);
	if(!file)
	{
		printf("Error: Opening file!\n");
		exit (1);
	}


/*	- read file into buffer.
	- parse for meta data. (but how?)
	- decode
	- display with mlx
	- get full points
*/

	fclose(file);


	init(&mfa);							//initialize mlx
	write(1, "Hello, World!\n", 14);
	return 0;
}