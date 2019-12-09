# NAME: Nikola Samardzic
# EMAIL: nikola.s@ucla.edu
# ID: 904799099

build: lab4b.c
	gcc -Wall -Wextra lab4b.c -o lab4b -lmraa -lm
clean:
	rm -rf lab4b
	rm -rf lab4b-904799099.tar.gz

dist: build 
	tar -cvzf lab4b-904799099.tar.gz README lab4b.c Makefile

check: build
	echo "" > temp.txt
	./lab4b --log=temp2.txt --scale=F --period=3 <temp.txt &
	sleep 2.5
	echo "SCALE=C" >> temp.txt
	sleep 3
	echo "\nOFF" >> temp.txt
	sleep 2
	echo "Log after session: "
	cat temp2.txt
	rm temp2.txt temp.txt
