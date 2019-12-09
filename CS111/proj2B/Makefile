# NAME: Nikola Samardzic
# EMAIL: nikola.s@ucla.edu
# ID: 904799099

build: lab2_list.c SortedList.c
	gcc -Wall -Wextra -pthread lab2_list.c SortedList.c -o lab2_list
profile:
	gcc -lprofiler -g -Wall -Wextra -pthread lab2_list.c SortedList.c -o lab2_list_prof
	CPUPROFILE=/tmp/prof.out ./lab2_list_prof --iterations=1000 --threads=12 --sync=s
	pprof lab2_list_prof /tmp/prof.out --text > profile.out
	pprof lab2_list_prof /tmp/prof.out --list=thread_start >> profile.out
clean:
	rm -rf profile.out
	rm -rf lab2_list
	rm -rf lab2_add
	rm -rf lab2a-904799099.tar.gz
	rm -rf *.png
	rm -rf *.csv
dist: build graphs profile
	tar -cvzf lab2b-904799099.tar.gz lab2_list lab2_list_prof README Makefile lab2_list.c SortedList.h SortedList.c lab2b_list.csv *.png testbed.sh profile.out *.gp

graphs: tests
	./lab2_list.gp

tests: build testbed.sh
	./testbed.sh
