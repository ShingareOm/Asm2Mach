#include "queue.h"

char *fetchOpcode(const char *mnemonic)
{
    for (int i = 0; i < opcodeTableSize; i++)
    {
        if (strcmp(opcodeTable[i].mnemonic, mnemonic) == 0)
        {
            return opcodeTable[i].opcode;
        }
    }
    return NULL;
}

int resolveLabel(const char *label)
{
    for (int i = 0; i < symbolTableSize; i++)
    {
        if (strcmp(symbolTable[i].label, label) == 0)
        {
            return symbolTable[i].address;
        }
    }
    return -1;
}

void convertInstructionToOpcode(const char *instruction) {
    char mnemonic[10], operand1[20], operand2[20];
    int numOperands = sscanf(instruction, "%s %s %s", mnemonic, operand1, operand2);

    char *opcode = fetchOpcode(mnemonic);
    if (!opcode) {
        printf("Error: Unknown mnemonic '%s'\n", mnemonic);
        return;
    }

    printf("Opcode: %s ", opcode);

    if (numOperands == 2) {
        if (strcmp(operand1, "END") == 0) {
            printf("00\n");
        } else if (operand1[0] == '[') {
            int address;
            sscanf(operand1, "[%d]", &address);
            printf("%04X\n", address);
        } else if (isdigit(operand1[0])) {
            int value;
            sscanf(operand1, "%d", &value);
            printf("%02X\n", value);
        } else {
            int labelAddress = resolveLabel(operand1);
            if (labelAddress != -1) {
                printf("%04X\n", labelAddress);
            } else {
                printf("Error: Unresolved operand '%s'\n", operand1);
            }
        }
    } else if (numOperands == 3) {
        if (isdigit(operand2[0])) {
            int value;
            sscanf(operand2, "%d", &value);
            printf("%s %02X\n", operand1, value);
        } else {
            printf("%s %s\n", operand1, operand2);
        }
    } else {
        printf("\n");
    }
}




void processLabels(FILE *asmFile, InstructionQueue *queue)
{
    char buffer[MAX_INSTRUCTION_LEN];
    int address = 0;

    while (fgets(buffer, MAX_INSTRUCTION_LEN, asmFile))
    {
        buffer[strcspn(buffer, "\n")] = 0; 
        removeComments(buffer);

        if (strlen(buffer) == 0)
        {
            continue;
        }

        if (strchr(buffer, ':'))
        {
            char label[20];
            sscanf(buffer, "%s", label);
            label[strlen(label) - 1] = 0;
            symbolTable[symbolTableSize].address = address;
            strcpy(symbolTable[symbolTableSize].label, label);
            symbolTableSize++;
            printf("Label found: %s at address %d\n", label, address);
        }
        else if (strstr(buffer, "DB") == NULL && strstr(buffer, "DW") == NULL &&
                 strstr(buffer, "RESB") == NULL && strstr(buffer, "RESW") == NULL)
        {
            enqueue(queue, buffer); 
            address++;
        }
    }
}



void stripLabel(char *line)
{
    char *colon = strchr(line, ':');
    if (colon != NULL)
    {
        memmove(line, colon + 1, strlen(colon + 1) + 1);
    }
}





void processDataDirectives(FILE *asmFile)
{
    char buffer[MAX_INSTRUCTION_LEN];

    fseek(asmFile, 0, SEEK_SET); 
    dataSegmentSize = 0;

    while (fgets(buffer, MAX_INSTRUCTION_LEN, asmFile))
    {
        buffer[strcspn(buffer, "\n")] = 0; 
        removeComments(buffer);

        if (strlen(buffer) == 0)
        {
            continue;
        }

        char *ptr = buffer;
        while (isspace(*ptr)) ptr++;

        if (strchr(ptr, ':'))
        {
            ptr = strchr(ptr, ':') + 1;
            while (isspace(*ptr)) ptr++;
        }

        if (strstr(ptr, "DB") == ptr)
        {
            char *token = strtok(ptr + 2, ",");
            while (token != NULL)
            {
                unsigned int value;
                if (sscanf(token, " 0x%x", &value) == 1) 
                {
                    dataSegment[dataSegmentSize++] = (unsigned char)value;
                }
                else if (sscanf(token, " %d", &value) == 1) 
                {
                    dataSegment[dataSegmentSize++] = (unsigned char)value;
                }
                else
                {
                    printf("Error: Invalid DB value '%s'\n", token);
                }
                token = strtok(NULL, ",");
            }
        }
        else if (strstr(ptr, "DW") == ptr)
        {
            char *token = strtok(ptr + 2, ",");
            while (token != NULL)
            {
                unsigned int value;
                if (sscanf(token, " 0x%x", &value) == 1) 
                {
                    dataSegment[dataSegmentSize++] = (unsigned char)(value & 0xFF);
                    dataSegment[dataSegmentSize++] = (unsigned char)((value >> 8) & 0xFF);
                }
                else if (sscanf(token, " %d", &value) == 1) 
                {
                    dataSegment[dataSegmentSize++] = (unsigned char)(value & 0xFF); 
                    dataSegment[dataSegmentSize++] = (unsigned char)((value >> 8) & 0xFF); 
                }
                else
                {
                    printf("Error: Invalid DW value '%s'\n", token);
                }
                token = strtok(NULL, ",");
            }
        }
        else if (strstr(ptr, "RESB") == ptr)
        {
            int bytesToReserve;
            if (sscanf(ptr + 4, "%d", &bytesToReserve) == 1) 
            {
                for (int i = 0; i < bytesToReserve; i++)
                {
                    dataSegment[dataSegmentSize++] = 0x00;
                }
            }
            else
            {
                printf("Error: Invalid RESB size\n");
            }
        }
        else if (strstr(ptr, "RESW") == ptr)
        {
            int wordsToReserve;
            if (sscanf(ptr + 4, "%d", &wordsToReserve) == 1) 
            {
                for (int i = 0; i < wordsToReserve * 2; i++)
                {
                    dataSegment[dataSegmentSize++] = 0x00; 
                }
            }
            else
            {
                printf("Error: Invalid RESW size\n");
            }
        }
    }
}



void removeComments(char *line)
{
    char *commentStart = strchr(line, ';');
    if (commentStart != NULL)
    {
        *commentStart = '\0';
    }
}

void displayQueue(InstructionQueue *queue)
{
    printf("Instruction Queue:\n");
    for (int i = queue->front; i < queue->rear; i++)
    {
        printf("%s\n", queue->instructions[i % MAX_QUEUE_SIZE]);
    }
}

void displayDataSegment()
{
    printf("\nData Segment:\n");
    if (dataSegmentSize == 0)
    {
        printf("Empty\n");
        return;
    }

    for (int i = 0; i < dataSegmentSize; i++)
    {
        printf("0x%02X ", (unsigned char)dataSegment[i]);
        if ((i + 1) % 16 == 0)
        {
            printf("\n"); 
        }
    }
    printf("\n");
}

