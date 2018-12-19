#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int read_square(int** grid, int y, int x, int size)
{
    int sum = 0;
    int _y, _x;
    for (_y = y; _y < y + size; _y++) {
        for (_x = x; _x < x + size; _x++) {
            sum += grid[_y][_x];
        }
    }

    return sum;
}

int main(int argc, char* argv[])
{
    int serial_numer = 4842;
    int max_grid_size = 300;
    int y, x;
    int** grid = malloc(max_grid_size * sizeof(int*));
    for (y = 0; y < max_grid_size; y++) {
        grid[y] = malloc(max_grid_size * sizeof(int));
    }

    int rack_id;
    int power;
    for (y = 0; y < max_grid_size; y++) {
        for (x = 0; x < max_grid_size; x++) {
            rack_id = x + 1 + 10;
            power = rack_id * (y + 1);
            power += serial_numer;
            power *= rack_id;
            if (power > 100) {
                power = (power / 100) % 10;
            } else {
                power = 0;
            }
            grid[y][x] = power - 5;
        }
    }

    int max_power = INT_MIN;
    int coord_y = 0;
    int coord_x = 0;

    // part 1
    if (0) {
        for (y = 0; y < max_grid_size - 2; y++) {
            for (x = 0; x < max_grid_size - 2; x++) {
                power = read_square(grid, y, x, 3);

                if (power > max_power) {
                    max_power = power;
                    coord_y = y;
                    coord_x = x;
                }
            }
        }

        printf("<%d,%d>\n", coord_x + 1, coord_y + 1);
    }

    // part 2
    if (1) {
        int grid_size = 0;
        int _grid_size;
        for (_grid_size = 1; _grid_size < max_grid_size; _grid_size++) {
            for (y = 0; y < max_grid_size - _grid_size + 1; y++) {
                for (x = 0; x < max_grid_size - _grid_size + 1; x++) {
                    power = read_square(grid, y, x, _grid_size);

                    if (power > max_power) {
                        max_power = power;
                        coord_y = y;
                        coord_x = x;
                        grid_size = _grid_size;
                    }
                }
            }
        }

        printf("<%d,%d,%d>\n", coord_x + 1, coord_y + 1, grid_size);
    }

    free(grid);
    return 0;
}
