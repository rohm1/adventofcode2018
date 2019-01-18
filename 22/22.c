/**
 * Disclaimer:
 * I have really messed everything up with part 2
 * Many thank to daggerdragon for posting their code at reddit:
 * https://www.reddit.com/r/adventofcode/comments/a8i1cy/2018_day_22_solutions/eccituy/
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define LINE_LEN 50

#define TYPES_COUNT 3

#define X_0_FACTOR 16807
#define Y_0_FACTOR 48271
#define EROSION_LEVEL_MODULO 20183

// region index also is which item you cannot use there
// rocky  : 0, wet  : 1, narrow       : 2
// neither: 0, torch: 1, climbing gear: 2

#define TORCH 1

#define MARGIN 50

struct region {
    int type;
    int geologic_index;
    int erosion_level;
};

struct visit {
    int minutes;
    int x;
    int y;
    int equipped;
    struct visit *next;
    struct visit *prev;
};

struct region *mk_region(const int depth, const int geologic_index) {
    struct region *region = malloc(sizeof(struct region *));
    region->geologic_index = geologic_index;
    region->erosion_level = (region->geologic_index + depth) % EROSION_LEVEL_MODULO;
    region->type = region->erosion_level % TYPES_COUNT;
    return region;
}

void set_best_time(int *best_times, int target_x, int y, int x, int type, int minutes) {
    best_times[(y * (target_x + MARGIN) * TYPES_COUNT) + (x * TYPES_COUNT) + type] = minutes;
}

int get_best_time(const int *best_times, int target_x, int y, int x, int type) {
    return best_times[(y * (target_x + MARGIN) * TYPES_COUNT) + (x * TYPES_COUNT) + type];
}

struct visit *create_visit(struct visit *head, int new_y, int new_x, int minutes, int equipped) {
    struct visit *visit = malloc(sizeof(struct visit));
    visit->minutes = minutes;
    visit->y = new_y;
    visit->x = new_x;
    visit->equipped = equipped;

    if (head == NULL) {
        visit->prev = visit;
        visit->next = visit;
    } else {
        visit->next = head->next;
        visit->prev = head;
        head->next->prev = visit;
        head->next = visit;
    }

    return visit;
}

struct visit *
visit_neightboor(struct visit *current_visit, struct region ***grid, int *best_times, struct visit *head, int dy,
                 int dx, int target_y, int target_x, int minutes, int equipment) {
    int new_y = dy + current_visit->y;
    int new_x = dx + current_visit->x;

    // out of bounds
    if (new_y > target_y + MARGIN - 1 || new_y < 0 || new_x > target_x + MARGIN - 1 || new_x < 0) {
        return head;
    }

    // cannot go there with this equipment
    if (grid[new_y][new_x]->type == equipment) {
        return head;
    }

    // faster way there
    if (get_best_time(best_times, target_x, new_y, new_x, equipment) <= minutes) {
        return head;
    }

    set_best_time(best_times, target_x, new_y, new_x, equipment, minutes);

    return create_visit(head, new_y, new_x, minutes, equipment);
}

struct visit *
visit_neightboors(struct visit *current_visit, struct region ***grid, int *best_times, struct visit *head, int target_y,
                  int target_x, int minutes, int equipment) {
    head = visit_neightboor(current_visit, grid, best_times, head, -1, 0, target_y, target_x, minutes, equipment);
    head = visit_neightboor(current_visit, grid, best_times, head, 0, -1, target_y, target_x, minutes, equipment);
    head = visit_neightboor(current_visit, grid, best_times, head, 1, 0, target_y, target_x, minutes, equipment);
    return visit_neightboor(current_visit, grid, best_times, head, 0, 1, target_y, target_x, minutes, equipment);
}

int main(int argc, char *argv[]) {
    int depth;
    int target_x;
    int target_y;

    FILE *input = fopen("22.txt", "r");
    char *line = malloc(LINE_LEN * sizeof(char));
    fgets(line, LINE_LEN, input);
    sscanf(line, "depth: %d", &depth);
    fgets(line, LINE_LEN, input);
    sscanf(line, "target: %d,%d", &target_x, &target_y);
    fgets(line, LINE_LEN, input);
    free(line);
    fclose(input);

    int y, x;
    struct region ***grid = malloc((target_y + MARGIN) * sizeof(struct region **));
    for (y = 0; y < target_y + MARGIN; y++) {
        grid[y] = malloc((target_x + MARGIN) * sizeof(struct region *));
        for (x = 0; x < target_x + MARGIN; x++) {
            grid[y][x] = NULL;
        }
    }

    grid[0][0] = mk_region(depth, 0);
    grid[target_y][target_x] = mk_region(depth, 0);
    for (y = 0; y < target_y + MARGIN; y++) {
        for (x = 0; x < target_x + MARGIN; x++) {
            if (grid[y][x] == NULL) {
                if (x == 0) {
                    grid[y][x] = mk_region(depth, y * Y_0_FACTOR);
                } else if (y == 0) {
                    grid[y][x] = mk_region(depth, x * X_0_FACTOR);
                } else {
                    grid[y][x] = mk_region(depth, grid[y][x - 1]->erosion_level * grid[y - 1][x]->erosion_level);
                }
            }
        }
    }

    int sum = 0;
    for (y = 0; y <= target_y; y++) {
        for (x = 0; x <= target_x; x++) {
            sum += grid[y][x]->type;
        }
    }
    printf("sum: %d\n", sum);

    int i = 0;
    int visited_size = (target_y + MARGIN) * (target_x + MARGIN) * TYPES_COUNT;
    int *best_times = malloc(visited_size * sizeof(int));
    for (i = 0; i < visited_size; i++) {
        best_times[i] = INT_MAX;
    }

    struct visit *visit, *current_visit;
    struct visit *head = create_visit(NULL, 0, 0, 0, TORCH);
    int minutes, equipment;
    while (1) {
        // always get the shortest branch
        current_visit = head;
        visit = current_visit->next;
        while (visit != current_visit) {
            if (visit->minutes < current_visit->minutes) {
                current_visit = visit;
            }

            visit = visit->next;
        }

        if (current_visit->next == current_visit) {
            head = NULL;
        } else {
            // pop current_visit from list
            head = current_visit->next;
            current_visit->next->prev = current_visit->prev;
            current_visit->prev->next = current_visit->next;
        }

        // found!
        if (current_visit->x == target_x && current_visit->y == target_y && current_visit->equipped == TORCH) {
            break;
        }

        minutes = current_visit->minutes;
        minutes++;
        head = visit_neightboors(current_visit, grid, best_times, head, target_y, target_x, minutes,
                                 current_visit->equipped);

        minutes += 7;
        equipment = TYPES_COUNT - grid[current_visit->y][current_visit->x]->type - current_visit->equipped;
        head = visit_neightboors(current_visit, grid, best_times, head, target_y, target_x, minutes, equipment);

        free(current_visit);
    }

    printf("shortest time: %d minutes\n", current_visit->minutes);

    for (y = 0; y < target_y + MARGIN; y++) {
        for (x = 0; x < target_x + MARGIN; x++) {
            free(grid[y][x]);
        }

        free(grid[y]);
    }
    free(grid);

    free(best_times);
    free(current_visit);

    return EXIT_SUCCESS;
}