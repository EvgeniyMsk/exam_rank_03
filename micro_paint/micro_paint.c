#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
	int	width;
	int	height;
	char *matrice;
}	drawing;

typedef struct
{
	char	type;
	float	x;
	float	y;
	float	width;
	float	height;
	char	color;
}	rectangle;

int		ft_strlen(char *str)
{
	int	i;

	i = 0;
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

int			is_in_rectangle(float x, float y, rectangle *rectangle)
{
	if (x < rectangle->x || rectangle->x + rectangle->width < x || y < rectangle->y || rectangle->y + rectangle->height < y)
		return (0);
	if (x - rectangle->x < 1.00000000 || rectangle->x + rectangle->width - x < 1.00000000 || y - rectangle->y < 1.00000000 || rectangle->y + rectangle->height - y < 1.00000000)
		return (2);
	return (1);
}

void		execute_one(rectangle *rectangle, drawing *drawing, int x, int y)
{
	int		is_in;

	is_in = is_in_rectangle((float)x, (float)y, rectangle);
	if (is_in == 2 || ((is_in == 1) && (rectangle->type == 'R')))
		drawing->matrice[x + y * drawing->width] = rectangle->color;
	return;
}

int			apply_op(rectangle *rectangle, drawing *drawing)
{
	int		i;
	int		j;

	if (rectangle->width <= 0.00000000 || rectangle->height <= 0.00000000 || ((rectangle->type != 'R') && (rectangle->type != 'r')))
		return (1);
	i = 0;
	while (i < drawing->width)
	{
		j = 0;
		while (j < drawing->height)
			execute_one(rectangle, drawing, i, j++);
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
	rectangle	rectangle;
	drawing		drawing;
	int			scan_ret;

	if (!(get_info(file, &drawing)))
	{
		scan_ret = fscanf(file, "%c %f %f %f %f %c\n", &rectangle.type, &rectangle.x, &rectangle.y, &rectangle.width, &rectangle.height, &rectangle.color);
		while (scan_ret == 6)
		{
			if (apply_op(&rectangle, &drawing))
				return (1);
			scan_ret = fscanf(file, "%c %f %f %f %f %c\n", &rectangle.type, &rectangle.x, &rectangle.y, &rectangle.width, &rectangle.height, &rectangle.color);
		}
		if (scan_ret == -1)
		{
			print_info(&drawing);
			return (0);
		}
	}
	return (1);
}

int		main(int argc, char **argv)
{
	FILE *file;
	int		i;

	if (argc == 2)
	{
		file = fopen(argv[1], "r");
		if (file && !(execute(file)))
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