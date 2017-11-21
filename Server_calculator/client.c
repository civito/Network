#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 200

void error_handling(char* message){
        fputs(message,stderr);
        fputs("\n", stderr);
        exit(1);
}

int main(int argc, char* argv[]){
	char a;
	int sock;
	char message[BUF_SIZE];

	struct sockaddr_in serv_adr;
	if(argc != 3){
		printf("Usage : %s<IP><PORT>\n", argv[0]);
		exit(1);
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1) error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
/*
   2. connect - request to server

   int connect(int sockfd, const struct sockaddr_in *serv_addr, socklen_t addrlen); 

   sockfd = socket descriptor

   return 0 if succeed else -1


*/
	if(connect(sock,(const struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error");
	else
		puts("Connected.....");


	while(1){
/*
   3. read() write()

   ssize_t read (int fd, void *buf, size_t nbytes)

   ssize_t write (int fd, const void *buf, size_t n) 

   return bytes when succeed else -1
*/

		read(sock, message, BUF_SIZE);

		printf("[opcode] %s\n", message);

		memset(message,0,BUF_SIZE);
		scanf("%s",&a);
		fflush(stdin);

		write(sock, a, BUF_SIZE);

	}
	
	close(sock);
	return 0;
}

