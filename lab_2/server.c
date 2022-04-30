#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 2509
#define MAXLEN 1024
	
void swap(char *x, char *y)
{
	char buf = *x;	
	*x = *y;
	*y = buf;
}

int main()
{
	printf("SERVER ready\n");
	
	int sockfd;
	struct sockaddr_in servaddr, clientaddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	memset(&clientaddr, 0, sizeof(clientaddr));
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family 	 = AF_INET;	
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port 	 = htons(PORT);
	
	bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr));
	
	listen(sockfd, 1);
	int addrlen = sizeof(struct sockaddr_in);
	while(1)
	{
	int connect_sock = accept(sockfd, (struct sockaddr*)&clientaddr, (socklen_t*)&addrlen);
	if (connect_sock > 0)
	{
		char client_ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(clientaddr.sin_addr), client_ip, INET_ADDRSTRLEN);
		
		printf("CLIENT connected %s\n", client_ip);
	}
	
	int n;
	char client_msg[MAXLEN];
	while((n = recv(connect_sock, client_msg, MAXLEN, 0)) != 0)

	{
		if(n == -1)
			return -2;
		//printf("%s\n", client_msg);
		char answer_msg[MAXLEN];
		for(int i = 0; i < strlen(client_msg); ++i)
		{
			answer_msg[i] = client_msg[strlen(client_msg) - i - 1];
		}
		answer_msg[strlen(client_msg)] = '\0';
		write(connect_sock, answer_msg, MAXLEN);
	}
	}
	close(sockfd);
	//close(connect_sock);
	
	return 0;
}
