#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include"sha1.h"
#include"rsa.h"

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[2000];
    long long int c[50];
    char  server_reply[2000];
    char hash[2000];
    long long int sign[2000];
    int length;
    SHA1_CTX obj;
    long long i,j,s,l1;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("Erro na criacao do socket");
    }
    puts("Socket criado");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Erro na conexao");
        return 1;
    }

    puts("Conectado\n");
    while(1)
    {
        printf("Entre com a mensagem : ");
        gets(message);
        length=strlen(message);
        sha1_init(&obj);
        sha1_update(&obj,message,length);
        sha1_final(&obj,hash);
        printf("HASH: \n");
        print_hash(hash);

        struct chave_publica pub[1];
        struct chave_privada priv[1];
        rsa_gen_keys(pub, priv, PRIME_SOURCE_FILE);

        for(int i=0; i<strlen(hash); i++)
        {
            c[i]=rsa_modExp(((long long)hash[i]),pub->expoente,pub->modulo);
        }

        if( send(sock, message, sizeof(message),0)  < 0)
        {
            puts("Send failed");
            return 1;
        }
        if( send(sock, c, sizeof(c), 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        if( send(sock, &priv->expoente, sizeof(priv->expoente), 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        if( send(sock, &priv->modulo, sizeof(priv->modulo), 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        if( recv(sock, server_reply, 2000, 0) < 0)
        {
            puts("recv failed");
            break;
        }

        puts("\nServer reply :");
        puts(server_reply);
    }

    close(sock);
    return 0;
}
