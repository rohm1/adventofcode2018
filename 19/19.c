#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0

///// Part 2:
///// The following constants are specific to my input, and need to bee adapted for every inputs
///// To determine them, you need to manually/visually (with DEBUG 1) determine the code loop
///// See comment in the run() function
#define TARGET_NUMBER 10551432

#define REGISTERS_COUNT 6
#define REGISTER_TO_READ 0
#define REGISTER_FACTORISATION_1 1
#define REGISTER_FACTORISATION_2 3
#define REGISTER_TO_FACTORISE 2

#define INSTRUCTION_ADD_FACTOR 7
///// End part 2 specific stuff

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

const char* code_to_opcode(int code)
{
    if (code == OPCODE_ADDR) { return "addr"; }
    if (code == OPCODE_ADDI) { return "addi"; }
    if (code == OPCODE_MULR) { return "mulr"; }
    if (code == OPCODE_MULI) { return "muli"; }
    if (code == OPCODE_BANR) { return "banr"; }
    if (code == OPCODE_BANI) { return "bani"; }
    if (code == OPCODE_BORR) { return "borr"; }
    if (code == OPCODE_BORI) { return "bori"; }
    if (code == OPCODE_SETR) { return "setr"; }
    if (code == OPCODE_SETI) { return "seti"; }
    if (code == OPCODE_GTIR) { return "gtir"; }
    if (code == OPCODE_GTRI) { return "gtri"; }
    if (code == OPCODE_GTRR) { return "gtrr"; }
    if (code == OPCODE_EQIR) { return "eqir"; }
    if (code == OPCODE_EQRI) { return "eqri"; }
    if (code == OPCODE_EQRR) { return "eqrr"; }

    printf("unknown code %i\n", code);
    return "";
}

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

void run(struct instruction** instructions, int instructions_count, int* registers, int ip_register, int ip)
{
    int i, s;
    struct instruction* instruction;

    do {
        registers[ip_register] = ip;

        if (DEBUG) {
            printf("#ip %d [", ip);
            for (i = 0; i < REGISTERS_COUNT; i++) {
                printf("%d ", registers[i]);
            }
            printf("]");
        }

        instruction = instructions[ip];

        if (DEBUG) {
            printf(" %s %d %d %d [", code_to_opcode(instruction->opcode), instruction->a, instruction->b, instruction->c);
        }

        execute(instruction, registers);

        ip = registers[ip_register];
        ip++;

        if (DEBUG) {
            for (i = 0; i < REGISTERS_COUNT; i++) {
                printf("%d ", registers[i]);
            }
            printf("]\n");
        }

        /**
         * the code runs into a loop, which can be written as:
         * R0 = 0
         * for R3 = 1; R3 < R2; R3++
         *   for R1 = 1; R1 < R2; R2++
         *     if R1 * R3 == R2
         *       R0 += R3
         * in tries to find all factors of R2 (TARGET_NUMBER) in a pretty non efficient way, which would
         * require R2*R2 loops
         * we can compute the sum a bit faster!
         */

        if (registers[REGISTER_TO_FACTORISE] == TARGET_NUMBER) {
            s = 0;
            for (i = 1; i <= TARGET_NUMBER; i++) {
                if (TARGET_NUMBER % i == 0) {
                    s += i;
                }
            }
            registers[REGISTER_TO_READ] = s;
            registers[REGISTER_FACTORISATION_1] = TARGET_NUMBER + 1;
            registers[REGISTER_FACTORISATION_2] = TARGET_NUMBER + 1;
            registers[ip_register] = INSTRUCTION_ADD_FACTOR;
        }

    } while (ip < instructions_count);

    printf("value of register %d: %d\n", REGISTER_TO_READ, registers[REGISTER_TO_READ]);
}

int main(int argc, char* argv[])
{
    int i;

    char* line = malloc(LINE_LEN * sizeof(char));
    int instructions_count = 0;
    FILE* input = fopen("19.txt", "r");
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

    for (i = 0; i < REGISTERS_COUNT; i++) {
        registers[i] = 0;
    }
    registers[0] = 1;
    run(instructions, instructions_count, registers, ip_register, ip);

    for (i = 0; i < instructions_count; i++) {
        free(instructions[i]);
    }
    free(instructions);
    free(registers);

    return EXIT_SUCCESS;
}