OPTIMIZE = -O2

CC = gcc
CFLAGS = $(OPTIMIZE) -g3 -Wall -Wextra -march=native -mtune=native -mrdrnd

randlibsw.so:
	$(CC) $(CFLAGS) -c -fPIC randlibsw.c -o randlibsw.o
	$(CC) $(CFLAGS) -O2 -shared randlibsw.o -o randlibsw.so

randlibhw.so:
	$(CC) $(CFLAGS) -c -fPIC randlibhw.c -o randlibhw.o
	$(CC) $(CFLAGS) -shared randlibhw.o -o randlibhw.so

randmain:
	$(CC) $(CFLAGS) -c randcpuid.c -o randcpuid.o
	$(CC) $(CFLAGS) -c randmain.c -o randmain.o
	$(CC) $(CFLAGS) randmain.o randcpuid.o -ldl -Wl,-rpath,$(PWD) -o randmain
