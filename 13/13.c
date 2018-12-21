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
    int moved;
    int crashed;
};

int main(int argc, char* argv[])
{
    int part = 2;

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
                n->moved = 0;
                n->crashed = 0;

                positions[y][x] = n->name;
                carts[n->name] = n;
            }
        }
    }

    int collision = 0;
    int nx, ny;
    struct cart* current_cart;
    int remaining_cars;
    do {
        remaining_cars = 0;
        for (i = 0; i < carts_count; i++) {
            carts[i]->moved = 0;

            if (!carts[i]->crashed) {
                remaining_cars++;
            }
        }

        if (remaining_cars == 1 && part == 2) {
            break;
        }

        for (y = 0; y < height; y++) {
            for (x = 0; x < width; x++) {
                if (positions[y][x] != -1) {
                    current_cart = carts[positions[y][x]];
                    if (current_cart->moved) {
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
                    current_cart->moved = 1;

                    if (positions[ny][nx] != -1) {
                        current_cart->crashed = 1;
                        carts[positions[ny][nx]]->crashed = 1;

                        if (part == 1) {
                            collision = 1;
                        } else {
                            positions[ny][nx] = -1;
                        }

                        break;
                    }

                    current_cart->x = nx;
                    current_cart->y = ny;

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
            }

            if (collision) {
                break;
            }
        }
    } while (!collision);

    if (part == 1) {
        printf("<%d,%d>\n", nx, ny);
    } else {
        for (i = 0; i < carts_count; i++) {
            if (!carts[i]->crashed) {
                printf("<%d,%d>\n", carts[i]->x, carts[i]->y);
            }
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
