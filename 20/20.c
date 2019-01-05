#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PART_2_MIN_LEN 1000

#define BUFFER_SIZE 20000

#define GRID_SIZE 500

#define DIR_NORTH 'N'
#define DIR_EAST 'E'
#define DIR_SOUTH 'S'
#define DIR_WEST 'W'

#define REGEX_START '^'
#define REGEX_END '$'
#define BRANCH_START '('
#define BRANCH_END ')'
#define BRANCH '|'

void parse_branch(const char* regex, int* regex_index, int** grid, int len, int* y, int* x)
{
    *regex_index = *regex_index + 1;

    if (regex[*regex_index] == DIR_NORTH || regex[*regex_index] == DIR_EAST || regex[*regex_index] == DIR_SOUTH || regex[*regex_index] == DIR_WEST) {
        len++;

        if (regex[*regex_index] == DIR_NORTH) {
            *y = *y - 2;
        } else if (regex[*regex_index] == DIR_EAST) {
            *x = *x + 2;
        } else if (regex[*regex_index] == DIR_SOUTH) {
            *y = *y + 2;
        }  else {
            *x = *x - 2;
        }

        if (grid[*y][*x] != -1) {
            if (grid[*y][*x] > len) {
                grid[*y][*x] = len;
            }
        } else {
            grid[*y][*x] = len;
        }
    } else if (regex[*regex_index] == BRANCH_START) {
        int branches = 1;
        int i = 0;
        do {
            i++;

            if (regex[*regex_index + i] == BRANCH_START) {
                branches++;
            } else if (regex[*regex_index + i] == BRANCH_END) {
                branches--;
            }
        } while (branches != 0);

        int index = *regex_index;
        int y2, x2;
        do {
            y2 = *y;
            x2 = *x;
            parse_branch(regex, regex_index, grid, len, &y2, &x2);
        } while (*regex_index < index + i);
    } else {
        return;
    }

    parse_branch(regex, regex_index, grid, len, y, x);
}

int main(int argc, char** argv)
{
    int x, y;

    int** grid = malloc(GRID_SIZE * sizeof(int*));
    for (y = 0; y < GRID_SIZE; y++) {
        grid[y] = malloc(GRID_SIZE * sizeof(int));
        for (x = 0; x < GRID_SIZE; x++) {
            grid[y][x] = -1;
        }
    }

    char* regex = malloc(BUFFER_SIZE * sizeof(char));
    char* buffer = malloc(BUFFER_SIZE * sizeof(char));
    FILE* input = fopen("20.txt", "r");
    while (fgets(buffer, BUFFER_SIZE, input) != NULL) {
        if (buffer[0] == REGEX_START) {
            strcpy(regex, buffer);
        }
    }
    fclose(input);

    y = GRID_SIZE / 2;
    x = GRID_SIZE / 2;
    grid[y][x] = 0;

    int regex_index = 0;
    parse_branch(regex, &regex_index, grid, 0, &y, &x);

    free(buffer);
    free(regex);

    int max = 0;
    int longest_len = 0;

    for (y = 0; y < GRID_SIZE; y++) {
        for (x = 0; x < GRID_SIZE; x++) {
            if (grid[y][x] > max) {
                max = grid[y][x];
            }

            if (grid[y][x] >= PART_2_MIN_LEN) {
                longest_len++;
            }
        }

        free(grid[y]);
    }
    free(grid);

    printf("max: %d\n", max);
    printf("more as %d doors: %d\n", PART_2_MIN_LEN, longest_len);

    return EXIT_SUCCESS;
}