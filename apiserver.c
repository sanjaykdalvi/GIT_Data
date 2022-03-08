#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

extern  OpenDataFiles();

int main(int argc, char *argv[])
{
	int		sockfd, newsockfd, portno;
     	socklen_t 	clilen;
     	char 		buffer[256]={0};
     	int 		n, len;
	int		i=0;
	char		RequestMsg[25]={0};
	OpenDataFiles();

     	struct 	sockaddr_in serv_addr, cli_addr;
     	if (argc < 2) {
         	fprintf(stderr,"ERROR, no port provided\n");
         	exit(1);
     	}
	sockfd = socket( AF_INET, SOCK_STREAM, 0 );
	if (sockfd < 0)
	{ 
		printf("ERROR opening socket"); 
		return 0;
	}
	printf("Socket Opened \n");
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);

	serv_addr.sin_family      = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port        = htons( portno );

	if( bind(sockfd, (struct sockaddr *) &serv_addr,
		      sizeof(serv_addr)) < 0)
	{ 
              	printf("ERROR on binding\n"); 
		return 0;
	}
   	printf("Socket bind \n");
	listen( sockfd, 5 );
	
	while(1)
	{	
		do
		{
			clilen = sizeof( cli_addr );
			newsockfd = accept( sockfd, (struct sockaddr *)(&serv_addr), &clilen );
			if( newsockfd < 0 )
				return 0;

			SpawnChild( newsockfd );

		} while ( newsockfd < 0  );
	}
	close(sockfd);
	return 0; 
}

