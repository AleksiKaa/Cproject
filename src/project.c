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
    database[size].name = malloc(1);
    database[size].name[0] = '\0';

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
int cmp_(Nation a, Nation b)
{
    if (a.gold > b.gold)
        return 1;
    else if (a.gold < b.gold)
        return 0;
    else
    {
        if (a.silver > b.silver)
            return 1;
        else if (a.silver < b.silver)
            return 0;
        else
        {
            if (a.bronze >= b.bronze)
                return 1;
            else
                return 0;
        }
    }
}

void swap(Nation *a, Nation *b)
{
    Nation temp = *a;
    *a = *b;
    *b = temp;
}

Nation *bubblesort(Nation *database)
{
    int size = 0;
    while (database[size].name[0] != '\0')
        size++;
    Nation *sorted = calloc(size, sizeof(Nation));

    int i, j;
    for (i = 0; i < size; i++)
    {
        for (j = 1; j < size - i - 1; j++)
        {
            if (cmp_(database[i], database[j]))
                swap(&database[i], &database[j]);
        }
    }

    return sorted;
}

void printDatabase(Nation *database)
{
    // TODO: implement ordering, at the moment it modifies the original array, need to make a copy of the original
    bubblesort(database);
    int i = 0;
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
    // TODO: last element does not terminate the database

    fclose(f);
    return database;
}

void freeAndQuit(Nation *database)
{
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
    Nation *database;
    database = calloc(1, sizeof(Nation));
    database[0].name = malloc(1);
    database[0].name[0] = '\0';

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
            Nation *fromFile = loadFromFile(input);
            free(database);
            database = malloc(sizeof(fromFile));
            database = fromFile;
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