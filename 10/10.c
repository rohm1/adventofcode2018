#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct point {
    int x;
    int y;
    int vx;
    int vy;
    struct point* next;
};

int get_int(char** line, int* offset, int print)
{
    char* str = (char*) malloc(20 * sizeof(char*));

    while (*(*line + *offset) == ' ') {
        (*offset)++;
    }

    int i = 0;
    do {
        str[i] = *(*line + *offset);
        i++;
        (*offset)++;
    } while ((*(*line + *offset) >= '0' && *(*line + *offset) <= '9') || *(*line + *offset) == '-');

    i = atoi(str);
    free(str);

    return i;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

void move(struct point* current, int direction)
{
    do {
        current->x += current->vx * direction;
        current->y += current->vy * direction;
    } while ((current = current->next) != NULL);
}

int main(int argc, char* argv[])
{
    struct point* first = NULL;

    FILE* f = fopen("10.txt", "r");
    char* line = (char*) malloc(100 * sizeof(char*));
    struct point* current = NULL;
    while(fgets(line, 100, f) != NULL) {
        if (strlen(line) < 2) {
            continue;
        }

        struct point* n = malloc(sizeof(struct point));
        int i = 10;
        n->x = get_int(&line, &i, 0);
        i++;
        n->y = get_int(&line, &i, 1);
        i += 12;
        n->vx = get_int(&line, &i, 0);
        i++;
        n->vy = get_int(&line, &i, 0);

        if (first == NULL) {
            first = n;
        } else {
            current->next = n;
        }

        current = n;
    }

    int now = 0;
    int max_height = INT_MAX;
    int height;
    int width;
    int minx;
    int maxx;
    int miny;
    int maxy;

    do {
        minx = INT_MAX;
        maxx = INT_MIN;
        miny = INT_MAX;
        maxy = INT_MIN;

        current = first;
        do {
            minx = min(current->x, minx);
            miny = min(current->y, miny);
            maxx = max(current->x, maxx);
            maxy = max(current->y, maxy);
        } while ((current = current->next) != NULL);

        height = maxy - miny + 1;
        width = maxx - minx + 1;

        if (max_height > height) {
            max_height = height;
            move(first, 1);
            now++;
            continue;
        }
        break;
    } while (1);

    move(first, -1);
    now--;
    printf("message will appear in %d seconds:\n", now);

    char* grid = (char*) malloc((width * height) * sizeof(char));
    memset(grid, '.', width * height);

    current = first;
    do {
        grid[(current->y - miny) * width + (current->x - minx)] = '#';
    } while ((current = current->next) != NULL);

    int y;
    for (y = 0; y < height; y++) {
        char* line = (char*) malloc(width * sizeof(char));
        strncpy(line, grid + y * width, width);
        printf("%s\n", line);
        free(line);
    }

    free(grid);

    return 0;
}
