#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REGISTERS_COUNT 6

#define LINE_LEN 50

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

int opcode_to_code(char *opcode)
{
    if (strcasecmp(opcode, "addr") == 0) { return OPCODE_ADDR; }
    if (strcasecmp(opcode, "addi") == 0) { return OPCODE_ADDI; }
    if (strcasecmp(opcode, "mulr") == 0) { return OPCODE_MULR; }
    if (strcasecmp(opcode, "muli") == 0) { return OPCODE_MULI; }
    if (strcasecmp(opcode, "banr") == 0) { return OPCODE_BANR; }
    if (strcasecmp(opcode, "bani") == 0) { return OPCODE_BANI; }
    if (strcasecmp(opcode, "borr") == 0) { return OPCODE_BORR; }
    if (strcasecmp(opcode, "bori") == 0) { return OPCODE_BORI; }
    if (strcasecmp(opcode, "setr") == 0) { return OPCODE_SETR; }
    if (strcasecmp(opcode, "seti") == 0) { return OPCODE_SETI; }
    if (strcasecmp(opcode, "gtir") == 0) { return OPCODE_GTIR; }
    if (strcasecmp(opcode, "gtri") == 0) { return OPCODE_GTRI; }
    if (strcasecmp(opcode, "gtrr") == 0) { return OPCODE_GTRR; }
    if (strcasecmp(opcode, "eqir") == 0) { return OPCODE_EQIR; }
    if (strcasecmp(opcode, "eqri") == 0) { return OPCODE_EQRI; }
    if (strcasecmp(opcode, "eqrr") == 0) { return OPCODE_EQRR; }

    printf("unknown opcode %s\n", opcode);
    return -1;
}

/**
 * I didn't want to change the code of part 19 too much, so here are some global vars..
 *
 * about this: register 0 is only read once, in my case "eqrr 3 0 5"
 * which means that the program ends iff both registers 0 and 3 are equal.
 * from there, part 1 expects the first value of R3 that we meet, and part 2
 * expects the last unique value of R3.
 */
int values[100000000];
int values_count = 0;
int loop_found = 0;

void execute(struct instruction* instruction, int* registers)
{
    int i;

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
        case OPCODE_EQRR:
            for (i = 0; i < values_count; i++) {
                if (values[i] == registers[instruction->a]) {
                    loop_found = 1;
                    return;
                }
            }
            values[values_count] = registers[instruction->a];
            values_count++;
            registers[instruction->c] = registers[instruction->a] == registers[instruction->b] ? 1 : 0; break;
        default: printf("unknown opcode %d\n", instruction->opcode); break;
    }
}

void run(struct instruction** instructions, int instructions_count, int* registers, int ip_register, int ip)
{
    struct instruction* instruction;

    do {
        registers[ip_register] = ip;

        instruction = instructions[ip];

        execute(instruction, registers);

        if (loop_found) {
            break;
        }

        ip = registers[ip_register];
        ip++;
    } while (ip < instructions_count);
}

int main(int argc, char* argv[])
{
    int i;

    char* line = malloc(LINE_LEN * sizeof(char));
    int instructions_count = 0;
    FILE* input = fopen("21.txt", "r");
    int ip = 0;
    int ip_register = -1;
    while (fgets(line, LINE_LEN, input) != NULL) {
        if (strlen(line) < 2) {
            continue;
        }

        if (ip_register == -1) {
            sscanf(line, "#ip %d", &ip_register);
            continue;
        }

        instructions_count++;
    }

    rewind(input);

    struct instruction** instructions = malloc(instructions_count * sizeof(struct instruction*));
    struct instruction* instruction;

    char* opcode = malloc(4 * sizeof(char));
    i = 0;
    while (fgets(line, LINE_LEN, input) != NULL) {
        if (strlen(line) < 8) {
            continue;
        }

        instruction = malloc(sizeof(struct instruction));

        sscanf(line, "%s %d %d %d", opcode, &instruction->a, &instruction->b, &instruction->c);
        instruction->opcode = opcode_to_code(opcode);

        instructions[i] = instruction;
        i++;
    }

    fclose(input);
    free(line);
    free(opcode);

    int* registers = malloc(REGISTERS_COUNT * sizeof(int));
    for (i = 0; i < REGISTERS_COUNT; i++) {
        registers[i] = 0;
    }
    run(instructions, instructions_count, registers, ip_register, ip);

    printf("min: %d\n", values[0]);
    printf("max: %d\n", values[values_count - 1]);

    for (i = 0; i < instructions_count; i++) {
        free(instructions[i]);
    }
    free(instructions);
    free(registers);

    return EXIT_SUCCESS;
}