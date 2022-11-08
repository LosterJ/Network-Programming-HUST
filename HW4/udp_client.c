/*UDP Echo Client*/
#include <stdio.h>
#include <stdlib.h>          /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 1024

int main(int argc, char *argv[]){
	int client_sock;
	char buff[BUFF_SIZE];
	struct sockaddr_in server_addr;
	int bytes_sent,bytes_received, sin_size;
	char server_ip[20];
	int port_num;
	
	//Step 1: Construct a UDP socket
	printf("Client is running...\n");

	sin_size = sizeof(struct sockaddr);

	if(argc == 3){
		strcpy(server_ip,argv[1]);
		port_num = atoi(argv[2]);
	}else{
		printf("Syntax: ./client {server_ip} {port_number}\n");
		exit(0);
	}

	if ((client_sock=socket(AF_INET, SOCK_DGRAM, 0)) < 0 ){  /* calls socket() */
		perror("\nError: ");
		exit(0);
	}

	//Step 2: Define the address of the server
	memset(&server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port_num);
	server_addr.sin_addr.s_addr = inet_addr(server_ip);
	
	//Step 3: Communicate with server
	strcpy(buff, "start");
    bytes_sent = sendto(client_sock, buff, strlen(buff), 0,
        (struct sockaddr *) &server_addr, sizeof(struct sockaddr));
	
	while(1){
		printf("Insert string to send:");
		fgets(buff, sizeof(buff), stdin);
		buff[strlen(buff)-1] = '\0';
		
		//neu khong nhap gi ma chi Enter thi gui tin hieu dung
		if (strlen(buff) == 0) {
				strcpy(buff, "stop");
				bytes_sent = sendto(client_sock, buff, strlen(buff), 0,
					(struct sockaddr *) &server_addr, sin_size);
				printf("Client is stopped\n");
				break;
			}
		// send to server
		bytes_sent = sendto(client_sock, buff, strlen(buff), 0, (struct sockaddr *) &server_addr, sin_size);
		if(bytes_sent < 0){
			perror("Error: ");
			close(client_sock);
			return 0;
		}
		//reveive from server
		bytes_received = recvfrom(client_sock, buff, BUFF_SIZE - 1, 0, (struct sockaddr *) &server_addr, &sin_size);
		if(bytes_received < 0){
			perror("Error: ");
			close(client_sock);
			return 0;
		}
		buff[bytes_received] = '\0';

		if (strcmp(buff, "stop") == 0) {
				printf("Client is stopped\n");
				break;
		}
		if(strcmp(buff,"error")==0){
			printf("Error!!:Received message has some invalid character\n");
			continue;
		}
		if(strcmp(buff,"ok")==0){
			bytes_received = recvfrom(client_sock,buff,BUFF_SIZE - 1, 0, (struct sockaddr *) &server_addr, &sin_size);
			if(bytes_received > 0){
				buff[bytes_received]='\0';
				printf("%s\n",buff);
			}
			bytes_received = recvfrom(client_sock,buff,BUFF_SIZE - 1, 0, (struct sockaddr *) &server_addr, &sin_size);
			if(bytes_received > 0){
				buff[bytes_received]='\0';
				printf("%s\n",buff);
			}
		}
	
	}
		
	close(client_sock);
	return 0;
}
