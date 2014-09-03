struct gn 
{
	char num[10];
	char *player;
};

int gtn_setplayerstart();
int gtn_init(char *nick);
int gtn_guess(int numguess, char *nick);
