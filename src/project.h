#ifndef _PROJECT__H_
#define _PROJECT__H_

//Database terminates with entry named \0
typedef struct nation
{
    char *name;
    unsigned int gold;
    unsigned int silver;
    unsigned int bronze;
} Nation;

Nation *addNation(char *input, Nation *database);

void addMedals(char *input, Nation *database);

int cmp_(Nation a, Nation b, int cmp);

void swap(Nation *a, Nation *b);

Nation *bubblesort(Nation *database);

void printDatabase(Nation *database);

void saveToFile(char *input, Nation *database);

Nation *loadFromFile(char *input);

void freeAndQuit(Nation *database);

#endif //! _PROJECT__H_