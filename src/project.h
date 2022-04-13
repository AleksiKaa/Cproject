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

#endif //! _PROJECT__H_