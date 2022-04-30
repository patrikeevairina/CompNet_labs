#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT   2509
#define MAXLEN 1024

int main()
{
	int sockfd;
	struct sockaddr_in servaddr;
	char *client_msg = "CLIENT lookup";
	char server_msg[MAXLEN];

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("error");
		exit(EXIT_FAILURE);
	}
	memset(&servaddr, 0, sizeof(servaddr));
	
	servaddr.sin_family	 = AF_INET;
	servaddr.sin_port  	 = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	
	int n, len;
	int broadcast = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
	
	int res = 0;
	while(!res)
	{	
		struct timeval tval;

	        tval.tv_sec  = 10;
        	tval.tv_usec = 0;

		fd_set recset;
        	FD_ZERO(&recset);
        	FD_SET(sockfd, &recset);

		res = select(sockfd + 1, &recset, NULL, NULL, &tval);
		switch(res)
		{
		case -1:
			perror("select error");
			return -2;
			break;
		case 0:
			sendto(sockfd, (const char*)client_msg, strlen(client_msg),
                        MSG_CONFIRM, (const struct sockaddr*)&servaddr,
                        sizeof(servaddr));
			break;
		default:
			break;
		}
	}	
	n = recvfrom(sockfd, (char*)server_msg, MAXLEN, MSG_WAITALL,
			(struct sockaddr*)&servaddr, &len);
	server_msg[n] = '\0';
	
	char server_str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(servaddr.sin_addr), server_str, INET_ADDRSTRLEN);
	printf("SERVER found %s\n", server_str);
	
	close(sockfd);

	return 0;
}
