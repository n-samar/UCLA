# NAME: Nikola Samardzic
# EMAIL: nikola.s@ucla.edu
# ID: 904799099

main: client server

client:
	gcc -Wall -Wextra lab1b-client.c -o lab1b-client -lmcrypt

server:
	gcc -Wall -Wextra lab1b-server.c -o lab1b-server -lmcrypt

clean:
	rm -rf lab1b-client lab1b-server
	rm -rf lab1b-904799099.tar.gz

dist: Makefile README main
	tar -czvf lab1b-904799099.tar.gz lab1b-client.c lab1b-server.c Makefile README my.key
