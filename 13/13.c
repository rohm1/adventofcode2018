#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

struct cart {
    int x;
    int y;
    int name;
    int direction;
    int state;
    int crashed;
};

int smaller(struct cart* a, struct cart* b)
{
    return
        a->crashed == 0 && b->crashed == 1
        || a->y < b->y
        || a->y == b->y && a->x < b->x
            ? 1 : 0;
}

// https://rosettacode.org/wiki/Sorting_algorithms/Quicksort#C
void sort(struct cart** carts, int* order, int len)
{
    if (len < 2) {
        return;
    }

    struct cart* pivot = carts[order[len / 2]];
    int i, j;

    for (i = 0, j = len - 1; ; i++, j--) {
        while (smaller(carts[order[i]], pivot))  {
            i++;
        }

        while (smaller(pivot, carts[order[j]])) {
            j--;
        }

        if (i >= j) {
            break;
        }

        int temp = order[i];
        order[i] = order[j];
        order[j] = temp;
    }

    sort(carts, order, i);
    sort(carts, order + i, len - i);
}

int main(int argc, char* argv[])
{
    int width = 0;
    int height = 0;
    int i, x, y;

    FILE* input = fopen("13.txt", "r");
    char* line = (char*) malloc(200 * sizeof(char));
    while (fgets(line, 200, input) != NULL) {
        i = strlen(line);

        if (i < 2) {
            continue;
        }

        if (i > width) {
            width = i;
        }

        height++;
    }
    free(line);

    char ** grid = malloc(height * sizeof(char*));
    int ** positions = malloc(height * sizeof(int*));

    fseek(input, 0, SEEK_SET);
    y = 0;
    line = (char*) malloc(width * sizeof(char));
    while (fgets(line, width, input) != NULL) {
        i = strlen(line);

        if (i < 2) {
            continue;
        }

        grid[y] = (char*) malloc(i * sizeof(char));
        strcpy(grid[y], line);
        if (grid[y][i - 1] == '\n') {
            grid[y][i - 1] = '\0';
        }

        positions[y] = (int*) malloc(width * sizeof(int));
        for (x = 0; x < width; x++) {
            positions[y][x] = -1;
        }

        y++;
    }
    fclose(input);
    free(line);

    int carts_count = 0;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            if (grid[y][x] == '<' || grid[y][x] == '>' || grid[y][x] == '^' || grid[y][x] == 'v') {
                carts_count++;
            }
        }
    }

    struct cart** carts = malloc(carts_count * sizeof(struct cart*));
    int* order = malloc(carts_count * sizeof(int));
    i = 0;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            if (grid[y][x] == '<' || grid[y][x] == '>' || grid[y][x] == '^' || grid[y][x] == 'v') {
                struct cart* n = malloc(sizeof(struct cart));
                n->x = x;
                n->y = y;
                if (grid[y][x] == '^') {
                    n->direction = 0;
                } else if (grid[y][x] == '>') {
                    n->direction = 1;
                } else if (grid[y][x] == 'v') {
                    n->direction = 2;
                } else if (grid[y][x] == '<') {
                    n->direction = 3;
                }
                n->state = 0;
                n->name = i++;
                n->crashed = 0;

                positions[y][x] = n->name;
                carts[n->name] = n;
                order[n->name] = n->name;
            }
        }
    }

    int collision = 0;
    int nx, ny;
    struct cart* current_cart;
    int remaining_carts = carts_count;
    do {
        for (i = 0; i < carts_count; i++) {
            current_cart = carts[order[i]];

            if (current_cart->crashed) {
                continue;
            }

            if (current_cart->direction == 0) {
                nx = current_cart->x;
                ny = current_cart->y - 1;
            } else if (current_cart->direction == 1) {
                nx = current_cart->x + 1;
                ny = current_cart->y;
            } else if (current_cart->direction == 2) {
                nx = current_cart->x;
                ny = current_cart->y + 1;
            } else if (current_cart->direction == 3) {
                nx = current_cart->x - 1;
                ny = current_cart->y;
            }

            positions[current_cart->y][current_cart->x] = -1;
            current_cart->x = nx;
            current_cart->y = ny;

            if (positions[ny][nx] != -1) {
                current_cart->crashed = 1;
                carts[positions[ny][nx]]->crashed = 1;

                if (!collision) {
                    printf("first collision position: <%d,%d>\n", nx, ny);
                    collision = 1;
                }

                remaining_carts -= 2;
                positions[ny][nx] = -1;
                continue;
            }

            positions[ny][nx] = current_cart->name;

            if (grid[ny][nx] == '/') {
                if (current_cart->direction == 0) {
                    current_cart->direction = 1;
                } else if (current_cart->direction == 1) {
                    current_cart->direction = 0;
                } else if (current_cart->direction == 2) {
                    current_cart->direction = 3;
                } else {
                    current_cart->direction = 2;
                }
            } else if (grid[ny][nx] == '\\') {
                if (current_cart->direction == 0) {
                    current_cart->direction = 3;
                } else if (current_cart->direction == 1) {
                    current_cart->direction = 2;
                } else if (current_cart->direction == 2) {
                    current_cart->direction = 1;
                } else {
                    current_cart->direction = 0;
                }
            } else if (grid[ny][nx] == '+') {
                if (current_cart->state == 0) {
                    current_cart->direction = current_cart->direction - 1;
                    if (current_cart->direction == -1) {
                        current_cart->direction = 3;
                    }
                } else if (current_cart->state == 2) {
                    current_cart->direction = current_cart->direction + 1;
                    if (current_cart->direction == 4) {
                        current_cart->direction = 0;
                    }
                }

                current_cart->state = (current_cart->state + 1) % 3;
            }
        }

        sort(carts, order, carts_count);
    } while (remaining_carts != 1);

    for (i = 0; i < carts_count; i++) {
        if (!carts[i]->crashed) {
            printf("last cart position: <%d,%d>\n", carts[i]->x, carts[i]->y);
        }
    }

    for (y = 0; y < height; y++) {
        free(positions[y]);
        free(grid[y]);
    }

    free(positions);
    free(grid);

    for (i = 0; i < carts_count; i++) {
        free(carts[i]);
    }

    free(carts);

    return 0;
}
