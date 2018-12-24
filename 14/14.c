#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int after = 765071;
    int after_len = 6;
    int after_digits[6] = {7, 6, 5, 0, 7, 1};

    int players = 2;
    int current_player = 0;

    int* recipes = malloc(100 * after * sizeof(int));
    recipes[0] = 3;
    recipes[1] = 7;

    int recipes_count = 2;

    int* current_recipes = malloc(players * sizeof(int));
    current_recipes[0] = 0;
    current_recipes[1] = 1;

    int i;
    int steps;
    int sum;
    int start_index = -1;
    int after_index = -1;

    do {
        sum = 0;
        for (current_player = 0; current_player < players; current_player++) {
            steps = recipes[current_recipes[current_player]] + 1;
            current_recipes[current_player] = (current_recipes[current_player] + steps) % recipes_count;
            sum += recipes[current_recipes[current_player]];
        }

        if (sum > 9) {
            recipes[recipes_count++] = 1;
        }

        recipes[recipes_count++] = sum % 10;

        if (start_index != -1 && after_index == -1) {
            for (i = start_index; i < recipes_count; i++) {
                if (i - start_index < after_len && recipes[i] != after_digits[i - start_index]) {
                    start_index = -1;
                    break;
                }
            }

            if (after_index == -1 && start_index != -1 && recipes_count - start_index >= after_len) {
                after_index = start_index;
            }
        }

        if (start_index == -1 ) {
            if (recipes[recipes_count - 2] == after_digits[0] && recipes[recipes_count - 1] == after_digits[1]) {
                start_index = recipes_count - 2;
            } else if (recipes[recipes_count - 1] == after_digits[0]) {
                start_index = recipes_count - 1;
            }
        }
    } while (after_index == -1);

    printf("score 10 recipes after recipe %d: ", after);
    for (i = after; i < after + 10; i++) {+
        printf("%d", recipes[i]);
    }
    printf("\n");

    printf("%d appears after %d recipes\n", after, after_index);

    return 0;
}
