#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include"sha1.h"
#include"rsa.h"

long long e=0,n=0;

int main(int argc, char *argv[])
{
    int socket_desc,i, client_sock, c, read_size,r2,r3,r4;
    int flag=1;
    struct sockaddr_in server, client;
    unsigned char client_message[2000];
    char sign[2000];
    char hash[2000];
    char res[2000];
    int length;
    SHA1_CTX obj;
    long long m1, m2, ci[50];
    char server_message[2000];
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket criado");
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    if( bind(socket_desc,(struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
    listen(socket_desc, 3);
    puts("Aguardando por conexao...");
    c = sizeof(struct sockaddr_in);
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Conexão aceita");

    while( ((read_size = recv(client_sock, client_message, 2000, 0)) >0) && (r2=recv(client_sock, ci, sizeof(ci), 0)>0) && (r3=recv(client_sock, &e, sizeof(e), 0)>0) && (r4=recv(client_sock, &n, sizeof(n), 0)>0))
    {
        flag=1;
        printf("Mensagem do cliente\n");
        puts(client_message);
        length=strlen(client_message);
        sha1_init(&obj);
        sha1_update(&obj,client_message,length);
        sha1_final(&obj,hash);

        for(int i = 0; i<strlen(hash); i++)
        {
            res[i] = rsa_modExp(ci[i],e,n);
        }

        print_hash(res);

        if(0 == memcmp(res,hash,strlen(hash)))
            strcpy(server_message,"Authentication successful!");
        else
            strcpy(server_message,"Authentication failed!");
        n=0;
        e=0;
        write(client_sock, server_message, strlen(server_message));

    }

    if(read_size == 0)
    {
        puts("Cliente Disconectou");
        fflush(stdout);
    }

    return 0;
}
