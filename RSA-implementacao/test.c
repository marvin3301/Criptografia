#include <stdio.h>
#include "rsa.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    struct chave_publica pub[1];
    struct chave_privada priv[1];
    rsa_gen_keys(pub, priv, PRIME_SOURCE_FILE);

    printf("Chave Privada:\n Mod = %lld\n Expoente = %lld\n\n", (long long)priv->modulo, (long long) priv->expoente);
    printf("Chave Publica:\n Mod = %lld\n Expoente = %lld\n\n", (long long)pub->modulo, (long long) pub->expoente);

    char message[] = "Although any given solution to an NP-complete problem can be verified quickly (in polynomial time), there is no known efficient way to locate a solution in the first place; indeed, the most notable characteristic of NP-complete problems is that no fast solution to them is known. That is, the time required to solve the problem using any currently known algorithm increases very quickly as the size of the problem grows.";

    long long *encrypted = rsa_encrypt(message, sizeof(message), pub);

    char *decrypted = rsa_decrypt(encrypted, 8*sizeof(message), priv);

    if(0 == memcmp(decrypted, message, sizeof(message)))
    {
        printf("SUCCESS!\n");
    }
    else
    {
        printf("FAILURE!\n");
    }

    free(encrypted);
    free(decrypted);

    return 0;
}
