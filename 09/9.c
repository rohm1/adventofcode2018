#include <stdio.h>
#include <stdlib.h>

struct marble {
	int value;
	struct marble* next;
	struct marble* prev;
};

int main(int argc, char* argv[])
{
	int players = 471;
	int marbles = 72026 * 100;

	long* scores = (long*) calloc(players, sizeof(long));

	struct marble* first = (struct marble*) malloc(sizeof(struct marble));
	first->value = 0;

	first->next = first;
	first->prev = first;

	struct marble* crtMarble = NULL;
	crtMarble = first;

	int maxValue = 0;
	int crtPlayer = 0;

	do {
		struct marble* m = (struct marble*) malloc(sizeof(struct marble));
		m->value = ++maxValue;

		if (m->value % 23 == 0) {
			crtMarble = crtMarble->prev->prev->prev->prev->prev->prev->prev;

			scores[crtPlayer] += m->value;
			scores[crtPlayer] += crtMarble->value;

			crtMarble->prev->next = crtMarble->next;
			crtMarble->next->prev = crtMarble->prev;

			crtMarble = crtMarble->next;
		} else {
			crtMarble->next->next->prev = m;
			m->next = crtMarble->next->next;
			m->prev = crtMarble->next;
			m->prev->next = m;

			crtMarble = m;
		}

		crtPlayer++;
		if (crtPlayer == players) {
			crtPlayer = 0;
		}

	} while (maxValue < marbles);

	long max = 0;
	int i;
	for (i = 0; i < players; i++) {
		if (max < scores[i]) {
			max = scores[i];
		}
	}

	printf("max score: %ld\n", max);

	return 0;
}
