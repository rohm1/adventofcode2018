#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char* argv[])
{
    int part = 2;

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

    if (part == 1) {
        for (y = 1; y < max_grid_size - 1; y++) {
            for (x = 1; x < max_grid_size - 1; x++) {
                power =
                      grid[y - 1][x - 1] + grid[y - 1][x] + grid[y - 1][x + 1]
                    + grid[y    ][x - 1] + grid[y    ][x] + grid[y    ][x + 1]
                    + grid[y + 1][x - 1] + grid[y + 1][x] + grid[y + 1][x + 1];

                if (power > max_power) {
                    max_power = power;
                    coord_y = y;
                    coord_x = x;
                }
            }
        }

        printf("<%d,%d>\n", coord_x, coord_y);
    }

    if (part == 2) {
        int grid_size = 0;
        int _grid_size;
        int _max_grid_size;
        int _x, _y;

        for (y = 0; y < max_grid_size - 1; y++) {
            for (x = 0; x < max_grid_size -1; x++) {
                _max_grid_size = y > x ? max_grid_size - y : max_grid_size - x;
                power = 0;

                for (_grid_size = 0; _grid_size < _max_grid_size; _grid_size++) {
                    for (_y = 0; _y < _grid_size; _y++) {
                        power += grid[_y + y][x + _grid_size];
                    }

                    for (_x = 0; _x <= _grid_size; _x++) {
                        power += grid[y + _grid_size][_x + x];
                    }

                    if (power > max_power) {
                        max_power = power;
                        coord_y = y;
                        coord_x = x;
                        grid_size = _grid_size;
                    }
                }
            }
        }

        printf("<%d,%d,%d>\n", coord_x + 1, coord_y + 1, grid_size + 1);
    }

    free(grid);
    return 0;
}
