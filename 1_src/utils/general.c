#include <string.h>
#include <common.h>
#include <utils/general.h>

char* copyString(const char* string)
{
    char* newString = allocatev(char, (*string == NULL_CHAR ? 0 : strlen(string)) + 1);
    return strcpy(newString, string);
}
