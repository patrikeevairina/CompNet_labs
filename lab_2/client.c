#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT   2509
#define MAXLEN 1024

int main()
{
	int sockfd;	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family 	 = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port 	 = htons(PORT);
	
	int flag  = -1;
	int count = 1;
	char serv_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(servaddr.sin_addr), serv_ip, INET_ADDRSTRLEN);
	while(flag != 0)
	{
		printf("Try to connect to server %s:%i. Try count %i\n",
			serv_ip, PORT, count);
		flag = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
		count++;
	}
	printf("Connected to server\n");
	
	
	char *client_msg = "SERVER please encrypt me completely";
	send(sockfd, (const char*)client_msg, strlen(client_msg), 0);
	
	char server_msg[MAXLEN];
	recv(sockfd, server_msg, MAXLEN, 0);
	printf("SERVER answer: %s\n", server_msg);
	close(sockfd);
	
	return 0;
}
