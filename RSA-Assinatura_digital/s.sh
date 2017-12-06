gcc -c server.c
gcc -c rsa.c -o rsa.o
gcc -c sha1.c -o sha1.o
gcc server.o sha1.o rsa.o -o servidor
./servidor
