#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN 50

#define MAX_WATERFALLS 10000

#define COORD_X 0
#define COORD_Y 1

#define WATER -1
#define SPRING -2
#define CLAY -3
#define SAND -4

struct line_def {
    int first_coord_type;
    int first_coord_value;
    int second_coord_value_min;
    int second_coord_value_max;
    struct line_def* next;
};

struct waterfall {
    int x;
    int min_y;
    int max_y;
};

int get_int(char* line, int* offset)
{
    char* str = (char*) malloc(LINE_LEN * sizeof(char*));

    while (line[*offset] < '0' || line[*offset] > '9') {
        (*offset)++;
    }

    int i = 0;
    do {
        str[i] = line[*offset];
        i++;
        (*offset)++;
    } while (line[*offset] >= '0' && line[*offset] <= '9');

    i = atoi(str);
    free(str);

    return i;
}

struct line_def* parse_line(char* line)
{
    struct line_def* def = malloc(sizeof(struct line_def));
    int i = 2;
    def->first_coord_type = line[0] == 'x' ? COORD_X : COORD_Y;
    def->first_coord_value = get_int(line, &i);
    def->second_coord_value_min = get_int(line, &i);
    def->second_coord_value_max = get_int(line, &i);
    def->next = NULL;
    return def;
}

struct waterfall* fall(int** grid, int grid_max_y, int min_y, int x, int crt);
void fill(int** grid, int grid_max_y, int grid_max_x, struct waterfall* waterfall, struct waterfall** waterfalls, int* waterfalls_count);
void handle_waterfall(int** grid, int grid_max_y, int grid_max_x, struct waterfall** waterfalls, int* waterfalls_count, int y, int x);

int main(int argc, char* argv[])
{
    int max_x = 0;
    int max_y = 0;
    int min_y = 100000;
    struct line_def* first_line_def = NULL;
    struct line_def* current_line_def = NULL;
    struct line_def* line_def = NULL;

    FILE* input = fopen("17.txt", "r");
    char* line = malloc(LINE_LEN * sizeof(char));
    while(fgets(line, LINE_LEN, input) != NULL) {
        if (sizeof(line) < 2) {
            continue;
        }

        line_def = parse_line(line);

        if (line_def->first_coord_type == COORD_X && line_def->first_coord_value > max_x) {
            max_x = line_def->first_coord_value;
        } else if (line_def->first_coord_type == COORD_Y && line_def->second_coord_value_max > max_x) {
            max_x = line_def->second_coord_value_max;
        }

        if (line_def->first_coord_type == COORD_Y && line_def->first_coord_value > max_y) {
            max_y = line_def->first_coord_value;
        } else if (line_def->first_coord_type == COORD_X && line_def->second_coord_value_max > max_y) {
            max_y = line_def->second_coord_value_max;
        }

        if (line_def->first_coord_type == COORD_Y && line_def->first_coord_value < min_y) {
            min_y = line_def->first_coord_value;
        } else if (line_def->first_coord_type == COORD_X && line_def->second_coord_value_min < min_y) {
            min_y = line_def->second_coord_value_min;
        }

        if (first_line_def == NULL) {
            first_line_def = line_def;
        } else {
            current_line_def->next = line_def;
        }

        current_line_def = line_def;
    }
    fclose(input);

    max_x += 2;
    max_y += 2;

    int x, y;
    int** grid = malloc(max_y * sizeof(int*));
    for (y = 0; y < max_y; y++) {
        grid[y] = malloc(max_x * sizeof(int));
        for (x = 0; x < max_x; x++) {
            grid[y][x] = SAND;
        }
    }

    grid[0][500] = SPRING;
    current_line_def = first_line_def;
    do {
        if (current_line_def->first_coord_type == COORD_X) {
            for (y = current_line_def->second_coord_value_min; y <= current_line_def->second_coord_value_max; y++) {
                grid[y][current_line_def->first_coord_value] = CLAY;
            }
        } else {
            for (x = current_line_def->second_coord_value_min; x <= current_line_def->second_coord_value_max; x++) {
                grid[current_line_def->first_coord_value][x] = CLAY;
            }
        }
    } while((current_line_def = current_line_def->next) != NULL);

    current_line_def = first_line_def;
    do {
        line_def = current_line_def->next;
        current_line_def->next = NULL;
        free(current_line_def);
        current_line_def = line_def;
    } while (current_line_def != NULL);

    int i;
    struct waterfall** waterfalls = malloc(MAX_WATERFALLS * sizeof(struct waterfall*));
    int waterfalls_count = 0;
    for (i = 0; i < MAX_WATERFALLS; i++) {
        waterfalls[i] = NULL;
    }

    handle_waterfall(grid, max_y, max_x, waterfalls, &waterfalls_count, 1, 500);

    int tiles_reached = 0;
    int water_count = 0;
    for (y = 0; y < max_y - 1; y++) {
        for (x = 0; x < max_x; x++) {
            if (grid[y][x] >= WATER && y >= min_y) {
                tiles_reached++;
            }
            if (grid[y][x] == CLAY) {
                printf("#");
            } else if (grid[y][x] == SAND) {
                printf(".");
            } else if (grid[y][x] == SPRING) {
                printf("+");
            } else if (grid[y][x] == WATER) {
                printf("~");
                water_count++;
            } else {
                printf("|");
            }
        }
        printf("\n");
    }
    printf("tiles reached: %d\n", tiles_reached);
    printf("tiles left: %d\n", water_count);

    for (i = 0; i < MAX_WATERFALLS; i++) {
        if (waterfalls[i] != NULL) {
            free(waterfalls[i]);
        }
    }
    free(waterfalls);

    for (y = 0; y < max_y; y++) {
        free(grid[y]);
    }
    free(grid);

    return 0;
}

struct waterfall* fall(int** grid, int grid_max_y, int min_y, int x, int crt)
{
    int max_y = min_y;
    while (max_y < grid_max_y - 1 && grid[max_y + 1][x] == SAND) {
        max_y++;
    }

    int y;
    for (y = min_y; y <= max_y; y++) {
        grid[y][x] = crt;
    }

    struct waterfall* waterfall = malloc(sizeof(struct waterfall));
    waterfall->x = x;
    waterfall->min_y = min_y;
    waterfall->max_y = max_y;

    return waterfall;
}

void fill(int** grid, int grid_max_y, int grid_max_x, struct waterfall* waterfall, struct waterfall** waterfalls, int* waterfalls_count)
{
    if (waterfall->max_y == grid_max_y - 1) {
        return;
    }

    int x, min_x, max_x;
    do {
        min_x = waterfall->x;
        max_x = waterfall->x;

        while (min_x > 1 && (grid[waterfall->max_y][min_x - 1] == SAND || grid[waterfall->max_y][min_x - 1] > WATER) && (grid[waterfall->max_y + 1][min_x - 1] == CLAY || grid[waterfall->max_y + 1][min_x - 1] == WATER)) {
            min_x--;
        }

        if (grid[waterfall->max_y][min_x - 1] == SAND && grid[waterfall->max_y + 1][min_x - 1] == SAND && grid[waterfall->max_y + 1][min_x] <= WATER) {
            handle_waterfall(grid, grid_max_y, grid_max_x, waterfalls, waterfalls_count, waterfall->max_y, min_x -1);
            continue;
        }

        while (max_x < grid_max_x - 1 && (grid[waterfall->max_y][max_x + 1] == SAND || grid[waterfall->max_y][max_x + 1] > WATER) && (grid[waterfall->max_y + 1][max_x + 1] == CLAY || grid[waterfall->max_y + 1][max_x + 1] == WATER)) {
            max_x++;
        }

        if (grid[waterfall->max_y][max_x + 1] == SAND && grid[waterfall->max_y + 1][max_x + 1] == SAND && grid[waterfall->max_y + 1][max_x] <= WATER) {
            handle_waterfall(grid, grid_max_y, grid_max_x, waterfalls, waterfalls_count, waterfall->max_y, max_x + 1);
            continue;
        }

        if ((grid[waterfall->max_y][min_x - 1] == CLAY || grid[waterfall->max_y][min_x - 1] > WATER) && (grid[waterfall->max_y + 1][min_x - 1] == CLAY || grid[waterfall->max_y + 1][min_x - 1] == WATER) && (grid[waterfall->max_y + 1][min_x] == CLAY || grid[waterfall->max_y + 1][min_x] == WATER)
            && (grid[waterfall->max_y][max_x + 1] == CLAY || grid[waterfall->max_y][max_x + 1] > WATER) && (grid[waterfall->max_y + 1][max_x + 1] == CLAY || grid[waterfall->max_y + 1][max_x + 1] == WATER) && (grid[waterfall->max_y + 1][max_x] == CLAY || grid[waterfall->max_y + 1][max_x] == WATER)) {
            for (x = min_x; x <= max_x; x++) {
                grid[waterfall->max_y][x] = WATER;
            }

            if (waterfall->max_y > waterfall->min_y + 1) {
                waterfall->max_y--;
            } else {
                break;
            }
        } else {
            break;
        }
    } while (1);
}

void handle_waterfall(int** grid, int grid_max_y, int grid_max_x, struct waterfall** waterfalls, int* waterfalls_count, int y, int x)
{
    if (grid[y][x] != SAND) {
        return;
    }

    struct waterfall* waterfall = fall(grid, grid_max_y, y, x, *(waterfalls_count));
    waterfalls[*(waterfalls_count)] = waterfall;
    *(waterfalls_count) = *(waterfalls_count) + 1;
    fill(grid, grid_max_y, grid_max_x, waterfall, waterfalls, waterfalls_count);

    if (waterfall->max_y != grid_max_y - 1) {
        if (grid[waterfall->max_y + 1][waterfall->x] > WATER) {
            return;
        }

        if (x > 0 && grid[waterfall->max_y][x - 1] == SAND) {
            handle_waterfall(grid, grid_max_y, grid_max_x, waterfalls, waterfalls_count, waterfall->max_y, x - 1);
        }

        if (x < grid_max_x - 1 && grid[waterfall->max_y][x + 1] == SAND) {
            handle_waterfall(grid, grid_max_y, grid_max_x, waterfalls, waterfalls_count, waterfall->max_y, x + 1);
        }
    }
}
