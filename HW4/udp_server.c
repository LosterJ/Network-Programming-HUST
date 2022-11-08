/*UDP Echo Server*/
#include <stdio.h>
#include <stdlib.h>          /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 1024

int separate(char *string, char *char_str, char *num_str){
	memset(char_str, '\0', sizeof(char_str));
    memset(num_str, '\0', sizeof(num_str));

	for(int i=0;i<strlen(string);i++){
		if(('a'<=string[i] && 'z'>=string[i])||('a'<=string[i] && 'z'>=string[i])){
			strncat(char_str,string+i,1);
		}else if('0'<=string[i] && '9'>=string[i]){
			strncat(num_str,string+1,1);
		}else return 0; //neu co ky tu trai phep ham separate tra ve 0
	}
	char_str[strlen(char_str)]='\0';
	num_str[strlen(num_str)]='\0';
	return 1;
}

int main(int argc, char *argv[]){
	int port_num;
	int server_sock; /* file descriptors */
	char buff[BUFF_SIZE];
	int bytes_sent, bytes_received;
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in client, client1, client2; /* client's address information */
	int sin_size = sizeof(struct sockaddr_in);;
	char ok_mes[10],err_mes[10];

	char char_string[BUFF_SIZE];
	char int_string[BUFF_SIZE];
	sin_size=sizeof(struct sockaddr_in);

	//Step 1: Construct a UDP socket
	printf("Server is running...\n");

	strcpy(ok_mes,"ok");
	ok_mes[strlen(ok_mes)] = '\0';
	strcpy(err_mes,"error");
	err_mes[strlen(err_mes)] = '\0';

	if(argc ==2){
		port_num = atoi(argv[1]);
	}else{
		printf("Syntax: ./server <port_number>\n");
		exit(0);
	}

	if ((server_sock=socket(AF_INET, SOCK_DGRAM, 0)) == -1 ){  /* calls socket() */
		perror("\nError: ");
		exit(0);
	}
	
	
	//Step 2: Bind address to socket
	server.sin_family = AF_INET;         
	server.sin_port = htons(port_num);   /* Remember htons() from "Conversions" section? =) */
	server.sin_addr.s_addr = INADDR_ANY;  /* INADDR_ANY puts your IP address automatically */   
	memset(&server,'\0',sizeof(server));

  
	if(bind(server_sock,(struct sockaddr*)&server,sizeof(struct sockaddr))==-1){ /* calls bind() */
		perror("\nError: ");
		exit(0);
	}     
	
	//Step 3: Communicate with clients
	memset(&client,'\0',sizeof(client));
	memset(&client1,'\0',sizeof(client1));
	memset(&client2,'\0',sizeof(client2));

	bytes_received = recvfrom(server_sock, buff, BUFF_SIZE-1, 0, (struct sockaddr *) &client1, &sin_size);
	bytes_received = recvfrom(server_sock, buff, BUFF_SIZE-1, 0, (struct sockaddr *) &client2, &sin_size);
	

	
	while(1){
		
		bytes_received = recvfrom(server_sock, buff, BUFF_SIZE-1, 0, (struct sockaddr *) &client, &sin_size);
		
		if (bytes_received < 0){
			perror("\nError: ");
			exit (0);
		}else{
			buff[bytes_received] = '\0';
			printf("[%s:%d]: %s", inet_ntoa(client.sin_addr), ntohs(client.sin_port), buff);
		}

		if(ntohs(client.sin_port)==ntohs(client1.sin_port)){
						
			//client1 sent

			//tin hieu dung la "stop"
			//neu client nay gui tin hieu "stop" thi server chuyen tiep sang client con lai va client con lai cung dung
			if(strcmp(buff,"stop")==0){
				bytes_sent = sendto(server_sock, buff, bytes_received, 0,
                    (struct sockaddr *) &client2, sin_size);
                printf("Server is stopped\n");
                break;
			}
			//separate received string to num and char string
			if(separate(buff,char_string,int_string)){

				printf("Client1: %s\t",char_string);
				printf("Client1: %s\n",int_string);

				//truong hop thanh cong
				bytes_sent = sendto(server_sock, ok_mes, strlen(ok_mes), 0, (struct sockaddr *) &client2, sizeof(struct sockaddr));
				if (bytes_sent < 0) perror("\nError: ");
                bytes_sent = sendto(server_sock, int_string, strlen(int_string), 0, (struct sockaddr *) &client2, sizeof(struct sockaddr));
				if (bytes_sent < 0) perror("\nError: ");
                bytes_sent = sendto(server_sock, char_string, strlen(char_string), 0, (struct sockaddr *) &client2, sizeof(struct sockaddr));
				if (bytes_sent < 0) perror("\nError: ");
			}else{
				//truong hop khong thanh cong gui thong bao error
				bytes_sent = sendto(server_sock, err_mes, strlen(err_mes), 0, (struct sockaddr *) &client2, sin_size );
				if (bytes_sent < 0)
				perror("\nError: ");
			}	
		}else{

			printf("Client2 send\n");

			//client2 send
			if(strcmp(buff,"stop")==0){
				bytes_sent = sendto(server_sock, buff, bytes_received, 0,
                    (struct sockaddr *) &client1, sin_size);
                printf("Server is stopped\n");
                break;
			}
			//separate received string to num and char string
			if(separate(buff,char_string,int_string)){

				printf("Client2: %s\t",char_string);
				printf("Client2: %s\n",char_string);

				//truong hop thanh cong
				bytes_sent = sendto(server_sock, ok_mes, strlen(ok_mes), 0, (struct sockaddr *) &client1, sizeof(struct sockaddr));
				if (bytes_sent < 0) perror("\nError: ");
                bytes_sent = sendto(server_sock, int_string, strlen(int_string), 0, (struct sockaddr *) &client1, sizeof(struct sockaddr));
				if (bytes_sent < 0) perror("\nError: ");
                bytes_sent = sendto(server_sock, char_string, strlen(char_string), 0, (struct sockaddr *) &client1, sizeof(struct sockaddr));
				if (bytes_sent < 0) perror("\nError: ");
			}else{
				//truong hop khong thanh cong gui thong bao error
				bytes_sent = sendto(server_sock, err_mes, strlen(err_mes), 0, (struct sockaddr *) &client1, sin_size );
				if (bytes_sent < 0)
				perror("\nError: ");
			}	
							
		}
		
		close(server_sock);
		return 0;
	}

}
