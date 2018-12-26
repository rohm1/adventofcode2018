#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN 50

#define REGISTERS_COUNT 4

#define OPCODES_COUNT 16

#define OPCODE_ADDR 0
#define OPCODE_ADDI 1
#define OPCODE_MULR 2
#define OPCODE_MULI 3
#define OPCODE_BANR 4
#define OPCODE_BANI 5
#define OPCODE_BORR 6
#define OPCODE_BORI 7
#define OPCODE_SETR 8
#define OPCODE_SETI 9
#define OPCODE_GTIR 10
#define OPCODE_GTRI 11
#define OPCODE_GTRR 12
#define OPCODE_EQIR 13
#define OPCODE_EQRI 14
#define OPCODE_EQRR 15

struct instruction {
    int opcode;
    int a;
    int b;
    int c;
};

void execute(struct instruction* instruction, int* registers)
{
    switch(instruction->opcode) {
        case OPCODE_ADDR: registers[instruction->c] = registers[instruction->a] + registers[instruction->b]; break;
        case OPCODE_ADDI: registers[instruction->c] = registers[instruction->a] + instruction->b; break;
        case OPCODE_MULR: registers[instruction->c] = registers[instruction->a] * registers[instruction->b]; break;
        case OPCODE_MULI: registers[instruction->c] = registers[instruction->a] * instruction->b; break;
        case OPCODE_BANR: registers[instruction->c] = registers[instruction->a] & registers[instruction->b]; break;
        case OPCODE_BANI: registers[instruction->c] = registers[instruction->a] & instruction->b; break;
        case OPCODE_BORR: registers[instruction->c] = registers[instruction->a] | registers[instruction->b]; break;
        case OPCODE_BORI: registers[instruction->c] = registers[instruction->a] | instruction->b; break;
        case OPCODE_SETR: registers[instruction->c] = registers[instruction->a]; break;
        case OPCODE_SETI: registers[instruction->c] = instruction->a; break;
        case OPCODE_GTIR: registers[instruction->c] = instruction->a > registers[instruction->b] ? 1 : 0; break;
        case OPCODE_GTRI: registers[instruction->c] = registers[instruction->a] > instruction->b ? 1 : 0; break;
        case OPCODE_GTRR: registers[instruction->c] = registers[instruction->a] > registers[instruction->b] ? 1 : 0; break;
        case OPCODE_EQIR: registers[instruction->c] = instruction->a == registers[instruction->b] ? 1 : 0; break;
        case OPCODE_EQRI: registers[instruction->c] = registers[instruction->a] == instruction->b ? 1 : 0; break;
        case OPCODE_EQRR: registers[instruction->c] = registers[instruction->a] == registers[instruction->b] ? 1 : 0; break;
        default: printf("unknown opcode %d\n", instruction->opcode); break;
    }
}

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

void registers_copy(int *destination, const int *source)
{
    int i;
    for (i = 0; i < REGISTERS_COUNT; i++) {
        destination[i] = source[i];
    }
}

int registers_same(int* a, int* b)
{
    int i;
    for (i = 0; i < REGISTERS_COUNT; i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }

    return 1;
}

int test_opcode(struct instruction* instruction, int* registers_before, int* registers_after, int opcode, int** opcodes_test_map)
{
    int* registers_tmp = malloc(REGISTERS_COUNT * sizeof(int));
    int original_opcode = instruction->opcode;
    int same;

    instruction->opcode = opcode;
    registers_copy(registers_tmp, registers_before);
    execute(instruction, registers_tmp);
    instruction->opcode = original_opcode;
    same = registers_same(registers_after, registers_tmp);

    if (opcodes_test_map[opcode][original_opcode] == 1) {
        opcodes_test_map[opcode][original_opcode] = same;
    }

    free(registers_tmp);

    return same;
}

int run_sample(char* line, FILE* input, int** opcodes_test_map)
{
    int i = 0;
    int sum = 0;
    int* registers_before = malloc(REGISTERS_COUNT * sizeof(int));
    int* registers_after = malloc(REGISTERS_COUNT * sizeof(int));
    struct instruction* instruction = malloc(sizeof(struct instruction));

    registers_before[0] = get_int(line, &i);
    registers_before[1] = get_int(line, &i);
    registers_before[2] = get_int(line, &i);
    registers_before[3] = get_int(line, &i);

    fgets(line, LINE_LEN, input);

    i = 0;
    instruction->opcode = get_int(line, &i);
    instruction->a = get_int(line, &i);
    instruction->b = get_int(line, &i);
    instruction->c = get_int(line, &i);

    fgets(line, LINE_LEN, input);

    i = 0;
    registers_after[0] = get_int(line, &i);
    registers_after[1] = get_int(line, &i);
    registers_after[2] = get_int(line, &i);
    registers_after[3] = get_int(line, &i);

    sum += test_opcode(instruction, registers_before, registers_after, OPCODE_ADDR, opcodes_test_map);
    sum += test_opcode(instruction, registers_before, registers_after, OPCODE_ADDI, opcodes_test_map);
    sum += test_opcode(instruction, registers_before, registers_after, OPCODE_MULR, opcodes_test_map);
    sum += test_opcode(instruction, registers_before, registers_after, OPCODE_MULI, opcodes_test_map);
    sum += test_opcode(instruction, registers_before, registers_after, OPCODE_BANR, opcodes_test_map);
    sum += test_opcode(instruction, registers_before, registers_after, OPCODE_BANI, opcodes_test_map);
    sum += test_opcode(instruction, registers_before, registers_after, OPCODE_BORR, opcodes_test_map);
    sum += test_opcode(instruction, registers_before, registers_after, OPCODE_BORI, opcodes_test_map);
    sum += test_opcode(instruction, registers_before, registers_after, OPCODE_SETR, opcodes_test_map);
    sum += test_opcode(instruction, registers_before, registers_after, OPCODE_SETI, opcodes_test_map);
    sum += test_opcode(instruction, registers_before, registers_after, OPCODE_GTIR, opcodes_test_map);
    sum += test_opcode(instruction, registers_before, registers_after, OPCODE_GTRI, opcodes_test_map);
    sum += test_opcode(instruction, registers_before, registers_after, OPCODE_GTRR, opcodes_test_map);
    sum += test_opcode(instruction, registers_before, registers_after, OPCODE_EQIR, opcodes_test_map);
    sum += test_opcode(instruction, registers_before, registers_after, OPCODE_EQRI, opcodes_test_map);
    sum += test_opcode(instruction, registers_before, registers_after, OPCODE_EQRR, opcodes_test_map);

    free(registers_before);
    free(registers_after);
    free(instruction);

    return sum > 2 ? 1 : 0;
}

int main(int argc, char* argv[])
{
    FILE* input = fopen("16.txt", "r");
    char* line = malloc(LINE_LEN * sizeof(char));
    int empty_lines = 0;
    int similars = 0;
    int i, j;;
    int** opcodes_test_map = malloc(OPCODES_COUNT * sizeof(int*));
    for (i = 0; i < OPCODES_COUNT; i++) {
        opcodes_test_map[i] = malloc(OPCODES_COUNT * sizeof(int));
        for (j = 0; j < OPCODES_COUNT; j++) {
            opcodes_test_map[i][j] = 1;
        }
    }
    i = 0;
    while (fgets(line, LINE_LEN, input) != NULL) {
        if (strlen(line) < 2) {
            empty_lines++;
            if (empty_lines == 3) {
                break;
            }
            continue;
        }

        empty_lines = 0;

        similars += run_sample(line, input, opcodes_test_map);
        i++;
    }

    printf("similars: %d\n", similars);

    int* opcodes_map = malloc(OPCODES_COUNT * sizeof(int));
    int* opcodes_map_reversed = malloc(OPCODES_COUNT * sizeof(int));
    for (i = 0; i < OPCODES_COUNT; i++) {
        opcodes_map[i] = -1;
        opcodes_map_reversed[i] = -1;
    }

    int found, k, loop_found;
    do {
        loop_found = 0;

        for (i = 0; i < OPCODES_COUNT; i++) {
            if (opcodes_map_reversed[i] != -1) {
                continue;
            }

            found = 0;
            for (j = 0; j < OPCODES_COUNT; j++) {
                if (opcodes_test_map[i][j] == 1) {
                    found++;
                }
            }

            if (found == 1) {
                loop_found = 1;
                for (j = 0; j < OPCODES_COUNT; j++) {
                    if (opcodes_test_map[i][j] == 1) {
                        opcodes_map[j] = i;
                        opcodes_map_reversed[i] = j;
                        break;
                    }
                }

                for (k = 0; k < OPCODES_COUNT; k++) {
                    opcodes_test_map[k][j] = 0;
                }

                break;
            }
        }
    } while (loop_found);

    int* registers = malloc(REGISTERS_COUNT * sizeof(int));
    for (i = 0; i < REGISTERS_COUNT; i++) {
        registers[i] = 0;
    }

    struct instruction* instruction = malloc(sizeof(struct instruction));
    while (fgets(line, LINE_LEN, input) != NULL) {
        if (strlen(line) < 2) {
            continue;
        }

        i = 0;
        instruction->opcode = opcodes_map[get_int(line, &i)];
        instruction->a = get_int(line, &i);
        instruction->b = get_int(line, &i);
        instruction->c = get_int(line, &i);

        execute(instruction, registers);
    }

    printf("value of register 0: %d\n", registers[0]);

    fclose(input);

    for (i = 0; i < OPCODES_COUNT; i++) {
        free(opcodes_test_map[i]);
    }
    free(opcodes_test_map);
    free(opcodes_map);
    free(opcodes_map_reversed);
    free(line);
    free(registers);
    free(instruction);

    return 0;
}
