#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/functionHeader.h"
#include "../include/utils.h"

int main()
{
    FILE *asmFile;
    char filename[100];
    InstructionQueue queue;

    initQueue(&queue);

    printf("Enter the ASM file name: ");
    scanf("%s", filename);

    asmFile = fopen(filename, "r");
    if (asmFile == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    processLabels(asmFile, &queue);
    fseek(asmFile, 0, SEEK_SET);
    processDataDirectives(asmFile);
    fclose(asmFile);

    printf("\nCurrent Queue after processing labels:\n");
    displayQueue(&queue);

    displayDataSegment();

    printf("\nProcessing Instructions:\n");
    while (!isQueueEmpty(&queue))
    {
        char *instruction = dequeue(&queue);
        printf("\nInstruction: %s\n", instruction);
        convertInstructionToOpcode(instruction);
    }

    return 0;
}


