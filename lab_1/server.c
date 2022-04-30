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
	char *server_msg = "SERVER answer\n";
	char client_msg[MAXLEN];
	struct sockaddr_in servaddr, clientaddr;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&clientaddr, 0, sizeof(clientaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
	
	bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr));	
	int len, n;
	len = sizeof(clientaddr);
	while(1)
	{
	n = recvfrom(sockfd, (char*)client_msg, MAXLEN, MSG_WAITALL, (struct sockaddr*)&clientaddr, &len);
	client_msg[n] = '\0';
	//printf("client msg: %s\n", client_msg);
	char client_str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(clientaddr.sin_addr), client_str, INET_ADDRSTRLEN);
	printf("CLIENT request IP %s\n", client_str);
	
	//sendto(sockfd, (const char*)server_msg, strlen(server_msg), MSG_CONFIRM, (const struct sockaddr*)&clientaddr, len);
	//printf("answer sent\n");
	}
	close(sockfd);
	return 0;
}
