#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<pthread.h>

#define BUFFER 200

void error_handling(char* message){
        fputs(message,stderr);
        fputs("\n", stderr);
        exit(1);
}


void* Server(void* arg){
        int Socket = socket(AF_INET, SOCK_STREAM,0);
        if(Socket == -1)
                error_handling("socket()");

        char  message[BUFFER];


        struct sockaddr_in addr;

        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons((int)arg);
        addr.sin_addr.s_addr = htonl(INADDR_ANY);

	
        int retval = bind(Socket, (struct sockaddr_in*)&addr, sizeof(addr));
        if(retval == -1)
                error_handling("bind()");

        retval = listen(Socket, 5);
        if(retval == -1)
                error_handling("listen()");
        int client_sock;
        struct sockaddr_in client_addr;
        int addrlen;
        char buf[BUFFER];
//	char * token;
	int numbytes;

       while(1){
                addrlen = sizeof(client_addr);                  
                client_sock = accept(Socket,(struct sockaddr_in*)&client_addr, &addrlen);
                if(client_sock == -1){
                        error_handling("accept()");
                        continue;
                }

                while(1){
                        numbytes = recv(client_sock, message, sizeof(message), 0);	
//			token = strtok(message, "\n");
			puts(message);
             		memset(message,0,strlen(message));
		}
	}
}


int main(){
	pthread_t server;
	int err;
	int myport, yourport;
	char IP[100];
        int sock;
        char message[BUFFER];
	char buf[BUFFER];
	char ID[BUFFER];

        struct sockaddr_in serv_adr;

        printf("My port # // Connecting server IP \n");
        scanf("%d", &myport);
        scanf("%s", IP);

        printf("Server port # : \n");
        scanf("%d", &yourport);


	err = pthread_create(&server, NULL, Server, (void *)myport);

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock == -1) error_handling("socket() error");

        memset(&serv_adr, 0, sizeof(serv_adr));
        serv_adr.sin_family=AF_INET;
        serv_adr.sin_addr.s_addr = inet_addr(IP);
        serv_adr.sin_port=htons(yourport);


        while(connect(sock,(const struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1);
        puts("Connected.....");
	
	printf("Insert your ID : ");
	scanf("%s", ID);
	fflush(stdin);

        while(1){
                scanf("%s",message);
                fflush(stdin);
		sprintf(buf, "%s : %s",ID, message);
                send(sock, buf, strlen(buf), 0);
		memset(buf,0,strlen(buf));
	}

	pthread_join(server, NULL);
	close(sock);

	return 0;

}

