#include "main.h"
#include "irc_games.h"
#include "functions.h"

int gtn_setplayerstart() {
	struct gn guessnum;
	char *somestr = "0";
	memcpy(guessnum.player, somestr, strlen(somestr));
	return 0;
}

int gtn_init(char *nick) {
	int i = rand_int();
	char numstr[12];
	struct gn guessnum;
	
	if(strlen(nick)<1) {
		return 1;
	}
	
	if(strncmp(guessnum.player,"0", 1)!=0) {
		return 2;
	}
	
	sprintf(numstr, "%d", i);
	memcpy(guessnum.num, numstr, strlen(numstr));
	memcpy(guessnum.player, nick, strlen(nick));
	
	return 0;
}

int gtn_guess(int numguess, char *nick) {
	struct gn guessnum;
	
	if(strncmp(guessnum.player, "0", 1)==0) {
		return 2;
	}
	
	if(strncmp(guessnum.player, nick, strlen(nick))!=0) {
		return 1;
	}
	
	if(atoi(guessnum.num)==numguess) {
		// Keep the game environment clean
		char *somestr = "0";
		memcpy(guessnum.player, somestr, strlen(somestr));
		return 0;
	}
	
	if(atoi(guessnum.num)<numguess) {
		return 4;
	}
	
	if(atoi(guessnum.num)>numguess) {
		return 3;
	}
	
	return 6; // unknown, if this happens, game just continues.
}
