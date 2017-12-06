#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>


char buffer[1024];
const int MAX_DIGITS = 10;
int i,j = 0;

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


long long mdc(long long a, long long b)
{
    long long c;
    while ( a != 0 )
    {
        c = a;
        a = b%a;
        b = c;
    }
    return b;
}


long long EuclidesExtendido(long long a, long long b)
{
    long long x = 0, y = 1, u = 1, v = 0, mdc = b, m, n, q, r;
    while (a!=0)
    {
        q = mdc/a;
        r = mdc % a;
        m = x-u*q;
        n = y-v*q;
        mdc = a;
        a = r;
        x = u;
        y = v;
        u = m;
        v = n;
    }
    return y;
}

long long rsa_modExp(long long b, long long e, long long m)
{
    b = b % m;
    if(e == 0) return 1;
    if(e == 1) return b;
    if( e % 2 == 0)
    {
        return ( rsa_modExp(b * b % m, e/2, m) % m );
    }
    if( e % 2 == 1)
    {
        return ( b * rsa_modExp(b, (e-1), m) % m );
    }

}


void rsa_gen_keys(struct chave_publica *pub, struct chave_privada *priv, char *PRIME_SOURCE_FILE)
{
    FILE *primes_list;
    if(!(primes_list = fopen(PRIME_SOURCE_FILE, "r")))
    {
        fprintf(stderr, "Problema na leitura %s\n", PRIME_SOURCE_FILE);
        exit(1);
    }

    long long prime_count = 0;
    do
    {
        int bytes_read = fread(buffer,1,sizeof(buffer)-1, primes_list);
        buffer[bytes_read] = '\0';
        for (i=0 ; buffer[i]; i++)
        {
            if (buffer[i] == '\n')
            {
                prime_count++;
            }
        }
    }
    while(feof(primes_list) == 0);

    char prime_buffer[MAX_DIGITS];
    long long max = 0;
    long long phi_max = 0;
    long long p = 0;
    long long q = 0;
    long long e = powl(2, 8) + 1;
    long long d = 0;
    srand(time(NULL));

    do
    {

        int a = rand()%prime_count;
        int b = rand()%prime_count;

        rewind(primes_list);

        for(i=0; i < a + 1; i++)
        {
            fgets(prime_buffer,sizeof(prime_buffer)-1, primes_list);
        }

        p = atol(prime_buffer);

        memset(prime_buffer,0,MAX_DIGITS);
        rewind(primes_list);

        for(i=0; i < b + 1; i++)
        {
            fgets(prime_buffer,sizeof(prime_buffer)-1, primes_list);
        }

        q = atol(prime_buffer);

        max = p*q;
        phi_max = (p-1)*(q-1);
    }
    while(!(p && q) || (p == q) || (mdc(phi_max, e) != 1));

    d = EuclidesExtendido(phi_max,e);
    while(d < 0)
    {
        d = d+phi_max;
    }

    //printf("os primos sao %lld e %lld\n\n",(long long)p, (long long )q);

    pub->modulo = max;
    pub->expoente = e;

    priv->modulo = max;
    priv->expoente = d;
}


long long *rsa_encrypt(const char *message, const unsigned long message_size,
                       const struct chave_publica *pub)
{
    long long *encrypted = malloc(sizeof(long long)*message_size);
    long long i = 0;

    for(i=0; i < message_size; i++)
    {
        encrypted[i] = rsa_modExp(message[i], pub->expoente, pub->modulo);
    }

    return encrypted;
}


char *rsa_decrypt(const long long *message,
                  const unsigned long message_size,
                  const struct chave_privada *priv)
{

    char *decrypted = malloc(message_size/sizeof(long long));
    char *temp = malloc(message_size);
    long long i = 0;

    for(i=0; i < message_size/8; i++)
    {
        temp[i] = rsa_modExp(message[i], priv->expoente, priv->modulo);
    }

    for(i=0; i < message_size/8; i++)
    {
        decrypted[i] = temp[i];
    }

    free(temp);
    return decrypted;
}
