#include "project.h"
#include <stdio.h>
#include "stdlib.h"
#include "string.h"

Nation *addNation(char *input, Nation *database)
{
    char nation[1000];
    sscanf(input, "%*c %s", nation);

    // check for duplicates and get size of database
    int size = 0;
    while (database[size].name[0] != '\0')
    {
        if (!strcmp(nation, database[size].name))
        {
            printf("Nation is already in database. No entry added.\n");
            return NULL;
        }
        size++;
    }
    size++;

    // Initialize new nation
    Nation newNation;
    newNation.name = malloc(strlen(nation) + 1);
    strcpy(newNation.name, nation);
    newNation.gold = 0;
    newNation.silver = 0;
    newNation.bronze = 0;

    // Reallocate memory and append to database
    database = realloc(database, sizeof(Nation) * (size + 1));
    free(database[size - 1].name);
    database[size - 1] = newNation;
    database[size].name = calloc(1, sizeof(char));

    return database;
}

void addMedals(char *input, Nation *database)
{
    char name[1000];
    unsigned int g, s, b;

    sscanf(input, "%*c %s %u %u %u", name, &g, &s, &b);

    // Find correct entry and append values
    int i = 0;
    while (database[i].name[0] != '\0')
    {
        Nation *cur = &database[i];
        if (!strcmp(name, cur->name))
        {
            cur->gold += g;
            cur->silver += s;
            cur->bronze += b;
            return;
        }
        i++;
    }
    printf("No entry named %s\n", name);
}

// Comparison function to determine the ordering of elements when sorted
int cmp_(Nation a, Nation b, int cmp)
{   
    switch (cmp)
    {
    case 2:
        return a.gold > b.gold;
    case 1: 
        return a.silver > b.silver;
    case 0:
        return a.bronze > b.bronze;
    }
    return 0;
}

// Helper function to swap memory addresses' values
void swap(Nation *a, Nation *b)
{
    Nation temp = *a;
    *a = *b;
    *b = temp;
}

// Simple bubblesort
// The database is sorted 3 times, once for each medal, resulting in correct ordering
Nation *bubblesort(Nation *database)
{
    int size = 0;
    while (database[size].name[0] != '\0') size++;

    int cmp, i, j;
    for (cmp = 0; cmp < 3; cmp++)
    {
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size - i - 1; j++)
            {
                if (!cmp_(database[j], database[j + 1], cmp)) 
                    swap(&database[j], &database[j + 1]);
            }
        }
    }
    return database;
}

void printDatabase(Nation *database)
{
    bubblesort(database);
    int i = 0;
    // Iterate through array and print values
    while (database[i].name[0] != '\0')
    {
        Nation cur = database[i];
        printf("%s %u %u %u\n", cur.name, cur.gold, cur.silver, cur.bronze);
        i++;
    }
    printf("SUCCESS\n");
}

void saveToFile(char *input, Nation *database)
{
    char filename[1000];
    sscanf(input, "%*c %s", filename);

    FILE *f = fopen(filename, "w");
    if (!f)
    {
        printf("Error. Could not write to file.\n");
        return;
    }

    // Iterate through the database by index and write each entry on its own line
    int i = 0;
    while (database[i].name[0] != '\0')
    {
        Nation cur = database[i];
        fprintf(f, "%s %u %u %u\n", cur.name, cur.gold, cur.silver, cur.bronze);
        i++;
    }
    fclose(f);
}

Nation *loadFromFile(char *input)
{
    char filename[1000];
    sscanf(input, "%*c %s", filename);

    FILE *f = fopen(filename, "r");
    if (!f)
        return NULL;

    // Get line count
    int len = 0;
    char c = getc(f);
    if (!feof(f)) 
        len++;
    while (c != EOF)
    {
        if (c == '\n')
            len++;
        c = getc(f);
    }
    rewind(f);

    // Add entries to database
    Nation *database = calloc(len, sizeof(Nation));
    Nation *cur;
    for (int i = 0; i < len - 1; i++)
    {
        cur = &database[i];
        char name[1000];
        unsigned int g, s, b;

        fscanf(f, "%s %u %u %u", name, &g, &s, &b);

        cur->name = malloc(strlen(name) + 1);
        strcpy(cur->name, name);
        cur->gold = g;
        cur->silver = s;
        cur->bronze = b;
    }
    cur = &database[len - 1];
    cur->name = calloc(1, sizeof(char));

    fclose(f);
    return database;
}

void freeAndQuit(Nation *database)
{
    // Iterate through the database, freeing the allocated names
    int i = 0;
    while (database[i].name[0] != '\0')
    {
        free(database[i].name);
        i++;
    }
    free(database[i].name);
    free(database);
}

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