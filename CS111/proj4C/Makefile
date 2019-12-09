# NAME: Nikola Samardzic
# EMAIL: nikola.s@ucla.edu
# ID: 904799099

build: 
	gcc -Wall -Wextra lab4c_tcp.c -o lab4c_tcp -lmraa -lm
	gcc -Wall -Wextra lab4c_tls.c -o lab4c_tls -lmraa -lcrypto -lssl -lm -pthread
clean:
	rm -rf lab4c_tls
	rm -rf lab4c_tcp
	rm -rf lab4c-904799099.tar.gz

dist: build 
	tar -cvzf lab4c-904799099.tar.gz README lab4c_tls.c lab4c_tcp.c Makefile
