# NAME: Nikola Samardzic
# EMAIL: nikola.s@ucla.edu
# ID: 904799099

build: add list

add: lab2_add.c
	gcc -Wall -Wextra -pthread lab2_add.c -o lab2_add	
list: lab2_list.c SortedList.c
	gcc -Wall -Wextra -pthread lab2_list.c SortedList.c -o lab2_list
clean:
	rm -rf lab2_list
	rm -rf lab2_add
	rm -rf lab2a-904799099.tar.gz
	rm -rf *.png
	rm -rf *.csv
dist: build graphs
	tar -cvzf lab2a-904799099.tar.gz lab2_add lab2_list README Makefile lab2_add.c SortedList.h SortedList.c lab2_list.c lab2_add.csv lab2_list.csv *.png testbed.sh

graphs: tests
	./lab2_add.gp
	./lab2_list.gp

tests: build testbed.sh
	./testbed.sh
