#include "main.h"
#include "functions.h"

/* Remove carriage return from string */
const char *remove_creturn(char *str) {
	str = strtok(str, "\r");
	return str;
}
