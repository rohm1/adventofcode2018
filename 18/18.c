#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PART_1_MINUTES 10
#define PART_2_MINUTES 1000000000

#define LINE_LEN 100

#define OPEN '.'
#define TREES '|'
#define LUMBERYARD '#'

struct counts {
    int open;
    int trees;
    int lumberyard;
};

void count_acre(struct counts* counts, const char* grid, int y, int x, int width)
{
    if (x < 0 || x >= width) {
        return;
    }

    int index = y * width + x;

    if (grid[index] == OPEN) {
        counts->open++;
    } else if (grid[index] == TREES) {
        counts->trees++;
    } else if (grid[index] == LUMBERYARD) {
        counts->lumberyard++;
    }
}

void count_adjacents(struct counts* counts, const char* grid, int y, int x, int height, int width, int offset_prev)
{
    counts->open = 0;
    counts->trees = 0;
    counts->lumberyard = 0;

    int y_prev = y + offset_prev;

    if (y > 0) {
        count_acre(counts, grid, y_prev - 1, x - 1, width);
        count_acre(counts, grid, y_prev - 1, x    , width);
        count_acre(counts, grid, y_prev - 1, x + 1, width);
    }
    count_acre(counts, grid, y_prev    , x - 1, width);
    count_acre(counts, grid, y_prev    , x + 1, width);
    if (y < height - 1) {
        count_acre(counts, grid, y_prev + 1, x - 1, width);
        count_acre(counts, grid, y_prev + 1, x    , width);
        count_acre(counts, grid, y_prev + 1, x + 1, width);
    }
}

int main(int argc, char* argv[])
{
    int width = 0;
    int height = 0;
    int x, y, i, j;

    char* line = malloc(LINE_LEN * sizeof(char));
    FILE* input = fopen("18.txt", "r");
    while (fgets(line, LINE_LEN, input) != NULL) {
        i = strlen(line);

        if (line[i - 1] == '\n') {
            i--;
        }

        if (i < 2) {
            continue;
        }

        if (i > width) {
            width = i;
        }

        height++;
    }

    rewind(input);

    char* grid = malloc(2 * width * height * sizeof(char));

    y = 0;
    while (fgets(line, LINE_LEN, input) != NULL) {
        if (strlen(line) < 2) {
            continue;
        }

        for (x = 0; x < width; x++) {
            grid[y * width + x] = line[x];
            grid[(y + height) * width + x] = line[x];
        }

        y++;
    }

    fclose(input);
    free(line);

    int minute;
    int offset_current;
    int offset_prev;
    struct counts* counts = malloc(sizeof(struct counts));
    int* prev_results = malloc(1000000 * sizeof(int));
    int loop_start = -1;
    int loop_size = 10;
    for (minute = 1; minute <= PART_2_MINUTES; minute++) {
        offset_current = minute % 2 == 1 ? height : 0;
        offset_prev = minute % 2 == 1 ? 0 : height;

        for (y = 0; y < height; y++) {
            for (x = 0; x < width; x++) {
                count_adjacents(counts, grid, y, x, height, width, offset_prev);

                if (grid[(y + offset_prev) * width + x] == OPEN && counts->trees > 2) {
                    grid[(y + offset_current) * width + x] = TREES;
                } else if (grid[(y + offset_prev) * width + x] == TREES && counts->lumberyard > 2) {
                    grid[(y + offset_current) * width + x] = LUMBERYARD;
                } else if (grid[(y + offset_prev) * width + x] == LUMBERYARD && (counts->lumberyard == 0 || counts->trees == 0)) {
                    grid[(y + offset_current) * width + x] = OPEN;
                } else {
                    grid[(y + offset_current) * width + x] = grid[(y + offset_prev) * width + x];
                }
            }
        }

        counts->open = 0;
        counts->trees = 0;
        counts->lumberyard = 0;

        offset_current = PART_1_MINUTES % 2 == 1 ? height : 0;
        for (y = 0; y < height; y++) {
            for (x = 0; x < width; x++) {
                count_acre(counts, grid, y + offset_current, x, width);
            }
        }

        prev_results[minute] = counts->trees * counts->lumberyard;

        if (minute > 200) {
            for (i = minute; i > minute / 2; i--) {
                for (loop_size = 10; loop_size < 50; loop_size++) {
                    if (prev_results[i] == prev_results[i - loop_size] && prev_results[i - loop_size] == prev_results[i - 2 * loop_size]) {
                        loop_start = i - 2 * loop_size;
                        break;
                    }
                }

                if (loop_start != -1) {
                    break;
                }
            }

            if (loop_start != -1) {
                break;
            }
        }
    }

    printf("after %d minutes: %d\n", PART_1_MINUTES, prev_results[PART_1_MINUTES]);
    printf("after %d minutes: %d\n", PART_2_MINUTES, prev_results[loop_start + (PART_2_MINUTES - loop_start) % loop_size]);

    free(grid);
    free(prev_results);
    free(counts);

    return 0;
}
