#ifndef __RSA_H__
#define __RSA_H__

#include <stdint.h>

//arquivo de primos
char *PRIME_SOURCE_FILE = "primos.txt";


struct chave_publica
{
    long long modulo;
    long long expoente;
};

struct chave_privada
{
    long long modulo;
    long long expoente;
};

//gera as chaves
void rsa_gen_keys(struct chave_publica *pub, struct chave_privada *priv, const char *PRIME_SOURCE_FILE);

//Funcao de encriptacao
long long *rsa_encrypt(const char *message, const unsigned long message_size, const struct chave_publica *pub);

//funcao de decriptacao
char *rsa_decrypt(const long long *message, const unsigned long message_size, const struct chave_privada *pub);

#endif
