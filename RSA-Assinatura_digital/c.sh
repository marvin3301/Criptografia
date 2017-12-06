gcc -c client.c -o client.o 
gcc -c rsa.c -o rsa.o
gcc -c sha1.c -o sha1.o
gcc client.o sha1.o rsa.o -o cliente
./cliente