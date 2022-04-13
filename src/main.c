#include "project.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    // Create and initialize the database
    Nation *database;
    database = calloc(1, sizeof(Nation));
    database[0].name = calloc(1, sizeof(char));

    // Infinite loop that reads input from command line
    while (1)
    {
        char input[1000];
        fgets(input, sizeof(input), stdin);

        switch (input[0])
        {
        case 'A':
        {
            Nation *temp = addNation(input, database);
            if (temp != NULL)
            {
                database = temp;
            }
            break;
        }
        case 'L':
        {
            printDatabase(database);
            break;
        }
        case 'M':
            addMedals(input, database);
            break;
        case 'O':
        {
            freeAndQuit(database);
            database = loadFromFile(input);
            break;
        }
        case 'W':
        {
            saveToFile(input, database);
            break;
        }
        case 'Q':
        {
            freeAndQuit(database);
            return 1;
        }
        default:
            printf("Invalid command %c\n", input[0]);
        }
    }
}