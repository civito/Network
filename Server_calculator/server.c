#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>

#define BUFFER 200

void error_handling(char* message){
	fputs(message,stderr);
	fputs("\n", stderr);
	exit(1);
}

int main(){

	int iNumberone, iNumbertwo, iValue, retval = 0;

/* 
   1.create socket

   socket(int domain,int type,int protocol) returns -1 when fail, number that
   means socket identifier.
   PF_INET, AF_INET -> IPv4 internet protocol
   PF_INET6 -> IPv6 internet protocol

   SOCK_STREAM -> TCP/IP
   SOCK_DGRAM -> UDP/IP

   protocol -> normally 0
*/

	int Socket = socket(AF_INET, SOCK_STREAM,0);
	if(Socket == -1)
		error_handling("socket()");



/*
   2.socket structure initialization
  
   #include <netinet/in.h>

   struct sockaddr_in {
     short            sin_family;   // e.g. AF_INET
     unsigned short   sin_port;     // e.g. htons(3490)
     struct in_addr   sin_addr;     // see struct in_addr, below
     char             sin_zero[8];  // zero this if you want to
   };

   struct in_addr {
     unsigned long s_addr;  // load with inet_aton()
   };
*/

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(11448);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

/*
   3.bind	

   bind() means define IP & PORT to socket
	
   int bind(int sockfd, struct sockaddr *myaddr, socklen_t addrlen);

   sockfd = socket descriptor
 
   struct sockaddr_in -> AF_INET
   struct sockaddr -> AF_UNIX

   socklen_t addrlen -> sizeof addr structure

   return 0 when succeed else -1
*/
 
	retval = bind(Socket, (struct sockaddr_in*)&addr, sizeof(addr));
	if(retval == -1)
		error_handling("bind()");

/*
   4.listen - waiting for client request

   int listen(int s, int backlog); 
   
   s = socket descriptor
   
   backlog = number of waiting message queue 

   return 0 when succeed else -1
*/

	retval = listen(Socket, 5);
	if(retval == -1)
		error_handling("listen()");
	int client_sock;
	struct sockaddr_in client_addr;
	int addrlen;
	char buf[BUFFER];
	char opcode[1];


	while(1){
		addrlen = sizeof(client_addr);
/*
   5.accpet

   int accept(int s, struct sockaddr *addr, socklen_t *addrlen); 

   int s = socket descriptor

   return number(socket descriptor) when succeed else -1
*/

		client_sock = accept(Socket,(struct sockaddr_in*)&client_addr, &addrlen);
		if(client_sock == -1){
			error_handling("accept()");
			continue;
		}
		while(1){
			printf("input data1 : ");
			scanf("%d",&iNumberone);
			fflush(stdin);
			printf("input data1 : %d\n", iNumberone);
			printf("input opcode(+-*/) : ");
			scanf("%s",opcode);
			fflush(stdin);
			printf("input opcode : %s\n", opcode);
			printf("input data2 : ");
                        scanf("%d",&iNumbertwo);
                        fflush(stdin);
                        printf("input data2 : %d\n", iNumbertwo);
		
			
			switch(opcode[0]){
				case '+':
					iValue = iNumberone + iNumbertwo;
					break;
                                case '-':
                                        iValue = iNumberone - iNumbertwo;
                                        break;
                                case '*':
                                        iValue = iNumberone * iNumbertwo;
                                        break;
                                case '/':
                                        iValue = (iNumberone / iNumbertwo);
					break;
                                default:
                                        break;
			}
			printf("Value : %d\n",iValue);

			sprintf(buf, "%s [1]%d, [2]%d = %d", opcode, iNumberone, iNumbertwo, iValue);
			write(client_sock, buf, BUFFER);
			memset(buf,0,BUFFER);

		}

	}














}
