#include "main.h"
#include "irc.h"
#include "functions.h"
#include "dictionary.h"
#define BUF 2048
#define LINEBUF 512

char *owner, *nick;

char *process_string(char *in, int n) {
	int ii = -1, o, i, e;
	char *nothing = "0";
	char *buf = malloc(513);
	
	for(i = 0; i < n; i++) {

		ii++;
		buf[ii] = in[i];

		if((i > 0 && in[i] == '\n' && in[i-1] == '\r') || ii == 512) {

			buf[ii + 1] = '\0';
			o = ii;
			ii = -1;

			printf("<< %s", buf);

			in[n] = 0;
			if (!strncmp(buf, "PING", 4)) {
				buf[1] = 'O';
				return buf;
			}

			if(buf[0] == ':') {
				char *name;
				char *cmd;
				char *chan;
				char *e;
				char *msg;
				char *b = malloc(512);

				name = buf+1;
				e = strchr(name,'!');
				if (!e)
					return nothing;
				*e = 0;

				cmd = strchr(e+1,' ');
				if (!cmd)
					return nothing;
				cmd++;
				e = strchr(cmd,' ');
				if (!e)
					return nothing;
				*e = 0;

				chan = e+1;
				e = strchr(chan,' ');
				if (!e)
					e = strchr(chan,'\r');
				if (!e)
					e = strchr(chan,'\n');
				if (e)
					*e = 0;

				if (!strcmp(cmd, "JOIN")) {
					if(strncmp(name, nick, strlen(nick))==0) {
						return nothing;
					} else {
						sprintf(b,"PRIVMSG %s :Welcome %s\r\n",chan,name);
						return b;
					}
				} else if(!strcmp(cmd, "PRIVMSG")) {
							
					msg = strchr(e+1, ':');
					if(!msg)
						return nothing;
					if(msg)
						msg++;
						
					int k;	
					
					char *usestr = NULL;
					char *searchstr;
					for(k=0;dictionary[k].quote != 0;k++) {
						if(strncmp(bot_nick_exists(dictionary[k].quote, nick), "0", 1)) {
							usestr = str_replace(dictionary[k].quote,
											"*bot*", nick);
						}
						
						if(usestr==NULL) {
							searchstr = dictionary[k].quote;
						} else {
							searchstr = usestr;
						}
						
						if(strncmp(msg, searchstr, strlen(searchstr))==0) {
							sprintf(b,"PRIVMSG %s :%s, %s\r\n",chan,
								name,dictionary[k].reply);
							return b;	
						}
					}
					
					
					if(strncmp(msg, "@join", 5)==0) {
						e = strchr(msg, ' ');
						if(!e)
							return nothing;
						if(e)
							e++;
						
						sprintf(b,"JOIN %s\r\nPRIVMSG %s :Hi!\r\n",
							remove_creturn(e),remove_creturn(e));
						return b;
						*e = 0;
					}
					
					if(strncmp(msg, "@die", 4)==0) {
						if(strncmp(name, owner, strlen(owner))==0) {
							sprintf(b,"QUIT :Died\r\n");
							return b;
						} else {
							sprintf(b,"PRIVMSG %s :You are not my owner, you cannot kill me.\r\n", name);
							return b;
						}
					}
					
					if(strncmp(msg, "@op", 3)==0) {
						
						e = strchr(msg, ' ');
						if(!e)
							return nothing;
						if(e)
							e++;
						
						if(strncmp(name, owner, strlen(owner))==0) {
							sprintf(b,"MODE %s +o %s\r\n", chan, remove_creturn(e));
							return b;
						} else {
							sprintf(b,"PRIVMSG %s :You are not my owner, you cannot OP others.\r\n", name);
							return b;
						}
					}
					
					if(strncmp(msg, "@deop", 5)==0) {
						
						e = strchr(msg, ' ');
						if(!e)
							return nothing;
						if(e)
							e++;
						
						if(strncmp(name, owner, strlen(owner))==0) {
							sprintf(b,"MODE %s -o %s\r\n", chan, remove_creturn(e));
							return b;
						} else {
							sprintf(b,"PRIVMSG %s :You are not my owner, you cannot DeOP others.\r\n", name);
							return b;
						}
					}
					
					if(strncmp(msg, "@leave", 6)==0) {
						e = strchr(msg, ' ');
						if(!e)
							return nothing;
						if(e)
							e++;
						
						if(strncmp(name, owner, strlen(owner))==0) {
							sprintf(b,"PART %s :Bye!\r\n",remove_creturn(e));
							return b;
						} else {
							return nothing;
						}
						
						*e = 0;
					}
					return nothing;
				} else {
					return nothing;
				}
			}
		}
	} // for loop
}

int main(int argc, char **argv) {
	int socketfd, n;
	char in[BUF+1], out[BUF+1], c[512];
	char *pos, *action;

	if(argc != 5) {
		printf("Usage: %s <address> <port> <nick> <owner>\n", argv[0]);
		exit(1);
	} else if(atoi(argv[2]) < 1 || atoi(argv[2]) > 50000) {
		printf("Invalid port specified.\n");
		exit(1);
	} else if(irc_connect(argv[1], atoi(argv[2]), &socketfd) == 0) {
		printf("Failed to connect to %s.\n", argv[1]);
		exit(1);
	}

	nick = argv[3];
	owner = argv[4];

	if(strlen(nick)>48) {
		printf("Error: irc bot nickname too long\n");
		exit(1);
	}
	
	if(strlen(owner)>48) {
		printf("Error: bot owner nickname too long\n");
		exit(1);
	}

	/* write to buffer */
	sprintf(c, "NICK %s\r\n", nick);
	irc_send(socketfd, c);
	sprintf(c, "USER %s %s %s :%s\r\n", nick, nick, nick, nick);
	irc_send(socketfd, c);

	while(1) {
		in[0] = 0;
		n = irc_read(socketfd, in);

		if (n > 0) {
			char *str = process_string(in, n);
			if(strncmp(str, "0", 1)!=0) {
				irc_send(socketfd, str);
			}
			if(strncmp(str, "QUIT", 4)==0) {
				break;
			}
		} // if(n > 0)
	} // while(1)
	return 0;
}
