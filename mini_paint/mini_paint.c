#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
	int		width;
	int		height;
	char	*matrice;
}	drawing;

typedef	struct
{
	char	type;
	float	x;
	float	y;
	float	radius;
	char	color;
}	circle;

int		ft_strlen(char *str)
{
	int	i;

	while (str[i])
		i++;
	return (i);
}

int		get_info(FILE *file, drawing *drawing)
{
	char	background;
	int		i;

	if (fscanf(file, "%d %d %c\n", &drawing->width, &drawing->height, &background) == 3)
	{
		if (drawing->width < 1 || drawing->width > 300 || drawing->height < 1 || drawing->height > 300)
			return (1);
		drawing->matrice = (char *)malloc(sizeof(char) * drawing->width * drawing->height);
		if (!(drawing->matrice))
			return (1);
		i = 0;
		while (i < drawing->width * drawing->height)
			drawing->matrice[i++] = background;
		return (0);
	}
	return (1);
}

float		distance(float x1, float y1, float x2, float y2)
{
	return sqrtf(powf(x1 - x2, 2.) + powf(y1 - y2, 2.));
}

int			is_in_circle(float x, float y, circle *circle)
{
	float	dist;

	dist = distance(x, y, circle->x, circle->y) - circle->radius;
	if (dist <= 0.00000000)
	{
		if (dist <= -1.00000000)
			return (1);
		return (2);
	}
	return (0);
}

void		execute_one(circle *circle, drawing *drawing, int x, int y)
{
	int		is_in;

	is_in = is_in_circle((float)x, (float)y, circle);
	if (is_in == 2 || ((is_in == 1) && (circle->type == 'C')))
		drawing->matrice[x + y * drawing->width] = circle->color;
	return;
}

int			apply_op(circle *circle, drawing *drawing)
{
	int		i;
	int		j;

	if (circle->radius <= 0.00000000 || ((circle->type != 'C') && (circle->type != 'c')))
		return (1);
	i = 0;
	while (i < drawing->width)
	{
		j = 0;
		while (j < drawing->height)
			execute_one(circle, drawing, i, j++);
		i++;
	}
	return (0);
}

void		print_info(drawing *drawing)
{
	int		i;

	i = 0;
	while (i < drawing->height)
	{
		write(1, drawing->matrice + i++ * drawing->width, drawing->width);
		write(1, "\n", 1);
	}
}

int			execute(FILE *file)
{
	circle	circle;
	drawing	drawing;
	int		scan_ret;

	if (!(get_info(file, &drawing)))
	{
		scan_ret = fscanf(file, "%c %f %f %f %c\n", &circle.type, &circle.x, &circle.y, &circle.radius, &circle.color);
		while (scan_ret == 5)
		{
			if (apply_op(&circle, &drawing))
				return (1);
			scan_ret = fscanf(file, "%c %f %f %f %c\n", &circle.type, &circle.x, &circle.y, &circle.radius, &circle.color);
		}
		if (scan_ret == -1)
		{
			print_info(&drawing);
			return (0);
		}
	}
	return (1);
}

int			main(int argc, char **argv)
{
	FILE	*file;
	int		i;

	if (argc == 2)
	{
		file = fopen(argv[1], "r");
		if (file && !execute(file))
			return (0);
		i = ft_strlen("Error: Operation file corrupted\n");
		write(1, "Error: Operation file corrupted\n", i);
	}
	else
	{
		i = ft_strlen("Error: argument\n");
		write(1, "Error: argument\n", 1);
	}
	return (1);
}