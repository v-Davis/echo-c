#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char const *argv[])
{
	int listen_socket;
	int port;
	int bind_val;
	int listen_val;
	int connfd;
	char buff[BUF_SIZE];
	char *client_ip;
	int mesg;
	struct sockaddr_in local_address;
	struct sockaddr_in client_address;

	if(argc != 2){
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

//1 - socket
	listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(socket < 0) {
		printf("Could not create socket\n");
		return(1);
	}

//2 - bind
	port = atoi(argv[1]);
	local_address.sin_family = AF_INET;
	local_address.sin_port = htons(port);
	bind_val = bind(listen_socket, (struct sockaddr *)&local_address, sizeof(local_address));

	if(bind_val < 0){
		printf("Could not bind socket\n");
		return(1);
	}

//3 - listen
	listen_val = listen(listen_socket, 1);
	if(listen_val < 0){
		printf("Could not listen on socket\n");
		return(1);
	}
//4 - accept
	socklen_t client_len = sizeof(client_address);
	printf("Waiting for connection..\n");
	while(1){
		connfd = accept(listen_socket, (struct sockaddr *)&client_address, &client_len);
		if (connfd < 0){
			printf("Could not accept connection\n");
			return(1);
		}
		client_ip = inet_ntoa(client_address.sin_addr);
		if(client_ip == NULL){
			printf("Could not obtain client IP\n");
		}
		printf("Accepted connection to %s\n", client_ip);

//5 - receive
		bzero(buff, BUF_SIZE);
		mesg = recv(connfd, buff, BUF_SIZE, 0);
		if(mesg < 0){
			printf("Error receiving message\n");
			break;
		}

		printf("Received %d bytes: %s\n", mesg, buff);

//6 - send
		mesg = send(connfd, buff, strlen(buff), 0);
		if(mesg < 0){
			printf("Error sending message\n");
			return(1);
		}
		close(connfd);
	}
	return(0);
}