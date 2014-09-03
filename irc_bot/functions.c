#include "main.h"
#include "functions.h"

/* Remove carriage return from string */
const char *remove_creturn(char *str) {
	str = strtok(str, "\r");
	return str;
}

/* str_replace function - from stakoverflow */
char *str_replace(char *orig, char *rep, char *with) {
    char *result, *buf, *ins;
    int replen, withlen, frontlen, count;

	// check arguments
    if (!orig) {
        return NULL;
    }
    
    if (!rep) {
        rep = "";
    }
    replen = strlen(rep);
    
    if (!with) {
        with = "";
    }
    withlen = strlen(with);

    ins = orig;
    for (count = 0; buf = strstr(ins, rep); ++count) {
        ins = buf + replen;
    }

    buf = result = malloc(strlen(orig) + (withlen - replen) * count + 1);

    if (!result) {
        return NULL;
	}
	
    while (count--) {
        ins = strstr(orig, rep);
        frontlen = ins - orig;
        buf = strncpy(buf, orig, frontlen) + frontlen;
        buf = strcpy(buf, with) + withlen;
        orig += frontlen + replen; // move to next "end of rep"
    }
    
    strcpy(buf, orig);
    return result;
}

char *bot_nick_exists(char *str, char *nick) {
	char *searchstr = strstr(str, nick);
	if(searchstr) {
		return "0";
	}
	return "1";
}

int rand_int() {
	int r = rand() % 50;
	return r+1;
}
