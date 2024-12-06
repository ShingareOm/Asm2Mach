#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INSTRUCTION_LEN 100
#define MAX_QUEUE_SIZE 100
#define MAX_LABELS 50
#define MAX_DATA_SIZE 100

typedef struct {
    char instructions[MAX_QUEUE_SIZE][MAX_INSTRUCTION_LEN];
    int front;
    int rear;
} InstructionQueue;

typedef struct {
    char label[20];
    int address;
} SymbolTableEntry;

SymbolTableEntry symbolTable[MAX_LABELS];
int symbolTableSize = 0;

unsigned char dataSegment[MAX_DATA_SIZE];
int dataSegmentSize = 0;

typedef struct {
    char mnemonic[10];
    char opcode[5];
} OpcodeTable;

OpcodeTable opcodeTable[] = {
    {"MOV", "89"},
    {"ADD", "01"},
    {"SUB", "29"},
    {"MUL", "F7"},
    {"DIV", "F7"},
    {"PUSH", "50"},
    {"POP", "58"},
    {"HLT", "F4"},
    {"JMP", "E9"},
    {"END", "00"}
};

int opcodeTableSize = sizeof(opcodeTable) / sizeof(OpcodeTable);

