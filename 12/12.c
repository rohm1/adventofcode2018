#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

struct pot {
    int ps;
    int plant;
    long i;
    struct pot* next;
    struct pot* prev;
};

int main(int argc, char* argv[])
{
    long min_iterations = 20;
    long max_iterations = 50000000000;

    FILE* input = fopen("12.txt", "r");
    char* line = (char*) malloc(120 * sizeof(char*));

    struct pot* first_pot = NULL;
    struct pot* last_pot = NULL;
    struct pot* current_pot = NULL;

    int ranges = 100;
    long i;
    for (i = 0; i < ranges; i++) {
        struct pot* n = malloc(sizeof(struct pot));
        n->ps = 0;
        n->plant = 0;

        if (first_pot == NULL) {
            first_pot = n;
            first_pot->i = -ranges;
        } else {
            n->prev = current_pot;
            n->i = current_pot->i + 1;
            current_pot->next = n;
        }

        current_pot = n;
    }

    fgets(line, 120, input);
    int l = strlen(line);
    for (i = 0; i < l - 1; i++) {
        struct pot* n = malloc(sizeof(struct pot));
        n->ps = line[i] == '#' ? 1 : 0;
        n->plant = n->ps;
        n->prev = current_pot;
        n->i = current_pot->i + 1;
        current_pot->next = n;
        current_pot = n;
    }

    last_pot = current_pot;

    for (i = 0; i < ranges; i++) {
        struct pot* n = malloc(sizeof(struct pot));
        n->ps = 0;
        n->plant = 0;
        n->prev = current_pot;
        n->i = current_pot->i + 1;
        current_pot->next = n;
        current_pot = n;
    }

    int rules[32];
    memset(rules, -1, 32);
    while (fgets(line, 120, input) != NULL) {
        if (strlen(line) < 2) {
            continue;
        }

        rules[
            (line[0] == '#' ? 1 : 0) * 16
          + (line[1] == '#' ? 1 : 0) * 8
          + (line[2] == '#' ? 1 : 0) * 4
          + (line[3] == '#' ? 1 : 0) * 2
          + (line[4] == '#' ? 1 : 0)
        ] =  line[9] == '#' ? 1 : 0;
    }

    int j, k;
    long sum = 0;
    long sums[1000];
    memset(sums, 0, 1000);
    int lasts_same;
    long lasts_diff;

    for (i = 0; i < max_iterations; i++) {
        current_pot = first_pot->next->next;
        do {
            k =
                current_pot->prev->prev->ps * 16
              + current_pot->prev->ps       * 8
              + current_pot->ps             * 4
              + current_pot->next->ps       * 2
              + current_pot->next->next->ps;

            if (rules[k] != -1) {
                current_pot->plant = rules[k];
            }

            current_pot = current_pot->next;
        } while (current_pot->next->next != NULL);

        current_pot = first_pot;
        do {
            current_pot->ps = current_pot->plant;
        } while ((current_pot = current_pot->next) != NULL);

        current_pot = first_pot->next->next->next;
        if (current_pot->plant) {
            for (j = 0; j < ranges; j++) {
                struct pot* n = malloc(sizeof(struct pot));
                n->ps = 0;
                n->plant = 0;
                n->prev = NULL;
                n->next = first_pot;
                n->i = first_pot->i - 1;
                first_pot->prev = n;
                first_pot = n;
            }
        }

        if (last_pot->prev->prev->prev->plant) {
            for (j = 0; j < ranges; j++) {
                struct pot* n = malloc(sizeof(struct pot));
                n->ps = 0;
                n->plant = 0;
                n->prev = last_pot;
                n->next = NULL;
                n->i = last_pot->i + 1;
                last_pot->next = n;
                last_pot = n;
            }
        }

        sum = 0;
        current_pot = first_pot;
        do {
            if (current_pot->plant) {
                sum += current_pot->i;
            }
        } while ((current_pot = current_pot->next) != NULL);

        sums[i] = sum;

        if (i == min_iterations) {
            printf("%ld generations: %ld\n", min_iterations, sums[i - 1]);
        }

        if (i > 100) {
            lasts_same = 1;
            lasts_diff = sums[i] - sums[i - 1];

            for (j = i - 1; j > i - 10; j--) {
                if (sums[j] - sums[j - 1] != lasts_diff) {
                    lasts_same = 0;
                }
            }

            if (lasts_same) {
                break;
            }
        }
    }

    printf("%ld generations: %ld\n", max_iterations, sums[i - 1] + (sums[i] - sums[i - 1]) * (max_iterations - i));

    return 0;
}
