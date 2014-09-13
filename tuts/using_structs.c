#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct somedata
{
    char somestr[128];
};

int main() 
{
    struct somedata alias;
    char *writedata = "This will be written to somestr.";
    strncpy(alias.somestr, writedata, strlen(writedata));
    printf("%s\n", alias.somestr);
    return 0;
}
