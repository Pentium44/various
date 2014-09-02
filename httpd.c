// Stripped down web server based from chttpd
// Chris Dorman 2014 (cddo@riseup.net) - LGPLv2+
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 4096

// mimetype struct, searched through when checking mimetype
struct {
	char *ext;
	char *filetype;
} extensions [] = {
	{"gif", "image/gif"	},  
	{"jpg", "image/jpeg"}, 
	{"jpeg","image/jpeg"},
	{"png", "image/png"	},  
	{"zip", "image/zip"	},  
	{"gz",  "image/gz" 	},  
	{"tar", "image/tar"	},  
	{"htm", "text/html"	},  
	{"html","text/html"	},   
	{"cgi", "text/cgi" 	},  
	{"asp","text/asp"  	},   
	{"xml", "text/xml" 	},  
	{"js", "text/js"   	},
	{"css", "text/css" 	}, 
	{"c", "text/plain" 	},
	{"h", "text/plain" 	},
	{"txt", "text/plain"},
	{"sh", "text/plain"	},
	{"ogg", "audio/ogg"	},
	{"mp3", "audio/mpeg"},
	{0,0} 
};


// Check if a file exists (index.html)
int file_exists(char *fname) {
	struct stat   buffer;   
	return (stat (fname, &buffer) == 0);
}

void web(int fd, int hit)
{
	int j, file_fd, buflen, len;
	long i, filesize;
	char * fstr;
	static char buffer[BUFSIZE+1];

	// Check to see if file is corrupted
	filesize = read(fd,buffer,BUFSIZE); 
	if(filesize == 0 || filesize == -1) {
		sprintf(buffer,"HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n");
		write(fd,buffer,strlen(buffer));
		sprintf(buffer,"<html><h2>Error reading requested file.</h2></html>");
		write(fd,buffer,strlen(buffer));
		exit(1);
	}
	
	if(filesize > 0 && filesize < BUFSIZE) {
		buffer[filesize]=0;	
	} else {
		buffer[0]=0;
	}
	
	for(i=0;i<filesize;i++)	{
		if(buffer[i] == '\r' || buffer[i] == '\n') {
			buffer[i]='*';
		}
	}

	if(strncmp(buffer,"GET ",4) && strncmp(buffer,"get ",4)) {
		sprintf(buffer,"HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n");
		write(fd,buffer,strlen(buffer));
		sprintf(buffer,"<html><h2>Simple HTTP get support only.</h2></html>");
		write(fd,buffer,strlen(buffer));
		exit(1);
	}
		

	// This gets the file name from "GET /..."
	for(i=4;i<BUFSIZE;i++) { 
		if(buffer[i] == ' ') { 
			buffer[i] = 0;
			break;
		}
	}
	
	// Check if connection is trying to view in directories behind webserver root
	for(j=0;j<i-1;j++) {
		if(buffer[j] == '.' && buffer[j+1] == '.') {
			sprintf(buffer,"HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n");
			write(fd,buffer,strlen(buffer));
			sprintf(buffer,"<html><h2>Parent directories (..) not supported</h2></html>");
			write(fd,buffer,strlen(buffer));
			exit(1);
		}
	}
			
	// If client is connecting with GET / (root of website) then send index
	if( !strncmp(&buffer[0],"GET /\0",6) || !strncmp(&buffer[0],"get /\0",6) ) {
		if(file_exists("index.html")) {
			strcpy(buffer,"GET /index.html");
		} else {
			sprintf(buffer,"HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n");
			write(fd,buffer,strlen(buffer));
			sprintf(buffer,"<html><h2>Index not found</h2></html>");
			write(fd,buffer,strlen(buffer));
			exit(1);
		}
	}
	
	// Get file mimetype
	buflen=strlen(buffer);
	fstr = (char *)0;
	for(i=0;extensions[i].ext != 0;i++) {
		len = strlen(extensions[i].ext);
		if(!strncmp(&buffer[buflen-len], extensions[i].ext, len)) {
			fstr = extensions[i].filetype;
			break;
		}
	}
	
	
	if(atoi(fstr) == 0) {
		fstr = "application/octet-stream";
	}

	// See if the web server can open requested file from browser
	if(( file_fd = open(&buffer[5],O_RDONLY)) == -1) {
		sprintf(buffer,"HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n");
		write(fd,buffer,strlen(buffer));
		sprintf(buffer,"<html><h2>Error, file cound not be retrieved</h2></html>");
		write(fd,buffer,strlen(buffer));
		exit(1);
	}

	sprintf(buffer,"HTTP/1.0 200 OK\r\nContent-Type: %s\r\n\r\n", fstr);
	write(fd,buffer,strlen(buffer));

	while ((filesize = read(file_fd, buffer, BUFSIZE)) > 0) {
		write(fd,buffer,filesize);
	}
#ifdef LINUX
	sleep(1);
#endif
	exit(1);
}


int main(int argc, char **argv)
{
	// Intergers for ports, sockets, etc
	int i, port, pid, listenfd, socketfd, hit;
	socklen_t length;
	// structs for socket addresses, these are always needed!
	static struct sockaddr_in cli_addr; 
	static struct sockaddr_in serv_addr;

	if(argc < 3  || argc > 3 || !strcmp(argv[1], "-?")) {
		printf("usage: server [port] [server directory]"
	"\nExample: httpd 80 ./ &\n\n");
		exit(0);
	}
	if(chdir(argv[2]) == -1){ 
		printf("ERROR: Can't Change to directory %s\n",argv[2]);
		exit(1);
	}

	// This forks the httpd process into the background
	// remove it if you want it to run in terminal
	/*
	if(fork() != 0) {
		return 0; 	
	}
	signal(SIGCLD, SIG_IGN); 
	signal(SIGHUP, SIG_IGN); 
	for(i=0;i<32;i++) {
		close(i);	
	}
	*/
	
	setpgrp();	

	printf("Starting httpd...\n");

	// Check if socket succeeded to open
	if((listenfd = socket(AF_INET, SOCK_STREAM,0)) <0) {
		printf("Failed to open socket\n");
		exit(1);
	}
		
	// check if valid port number
	port = atoi(argv[1]);
	if(port < 0 || port >60000) {
		printf("Incorrect port value, try ports 1 - 60000\n");
		exit(1);
	}
	
	// binds the socket to a specific port and address
	// in this case, 0.0.0.0 and the port you choose
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);
	if(bind(listenfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) <0) {
		printf("Failed to bind to socket\n");
		exit(1);
	}
	if( listen(listenfd,64) <0) {
		printf("Failed to listen on socket\n");
		exit(1);
	}

	for(hit=1; ;hit++) {
		length = sizeof(cli_addr);
		
		printf("Got connection...\n");
		
		// Check if server succeeded to accept connection
		if((socketfd = accept(listenfd, (struct sockaddr *)&cli_addr, &length)) < 0) {
			printf("Failed to accept connection!\n");
			exit(1);
		}
		
		// Fork, and die if server fails to fork process
		if((pid = fork()) < 0) {
			printf("Failed to fork connection!\n");
			exit(1);
		}
		else {
			if(pid == 0) {
				close(listenfd);
				web(socketfd,hit);
			} else {
				close(socketfd);
			}
		}
	}
}
