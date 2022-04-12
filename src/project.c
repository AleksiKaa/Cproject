#include "project.h"
#include <stdio.h>
#include "stdlib.h"
#include "string.h"

Nation *addNation(char *input, Nation *database, int size)
{
    char nation[1000];
    sscanf(input, "%*c %s", nation);

    // check for duplicates
    for (int i = 0; i < size; i++)
    {
        if (!strcmp(nation, database[i].name))
        {
            printf("Nation is already in database. No entry added.\n");
            return NULL;
        }
    }

    // Initialize new nation
    Nation newNation;
    newNation.name = malloc(strlen(nation) + 1);
    strcpy(newNation.name, nation);
    newNation.gold = 0;
    newNation.silver = 0;
    newNation.bronze = 0;

    // Reallocate memory and append to database
    database = realloc(database, sizeof(Nation) * (size + 1));
    database[size] = newNation;

    return database;
}

void addMedals(char *input, Nation *database, int size)
{
    char name[1000];
    unsigned int g;
    unsigned int s;
    unsigned int b;

    sscanf(input, "%*c %s %u %u %u", name, &g, &s, &b);

    for (int i = 0; i < size; i++)
    {
        Nation *cur = &database[i];
        if (!strcmp(name, cur->name))
        {
            cur->gold += g;
            cur->silver += s;
            cur->bronze += b;
            return;
        }
    }
    printf("No entry named %s\n", name);
}

void printDatabase(Nation *database, int size)
{
    // implement ordering
    for (int i = 0; i < size; i++)
    {
        Nation cur = database[i];
        printf("%s %u %u %u\n", cur.name, cur.gold, cur.silver, cur.bronze);
    }
    printf("SUCCESS\n");
}

void saveToFile(Nation *database);

void loadFromFile(Nation *database);

int main(void)
{
    Nation *database;
    database = calloc(1, sizeof(Nation));
    int size = 0;

    while (1)
    {
        char input[1000];
        fgets(input, sizeof(input), stdin);

        switch (input[0])
        {
        case 'A':
        {
            Nation *temp = addNation(input, database, size);
            if (temp != NULL)
            {
                database = temp;
                size++;
            }
            break;
        }
        case 'L':
        {
            printDatabase(database, size);
            break;
        }
        case 'M':
            addMedals(input, database, size);
            break;
        case 'O':
            break;
        case 'W':
            break;
        case 'Q':
        {
            printf("Quitting...\n");
            return 1;
        }
        default:
            printf("Invalid command %c\n", input[0]);
        }
    }
}