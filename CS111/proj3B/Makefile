# NAME: Nikola Samardzic
# EMAIL: nikola.s@ucla.edu
# ID: 904799099

build: 
	echo "#!/bin/sh" > lab3b
	echo 'python lab3b.py "$$1"' >> lab3b
	chmod +x lab3b
clean:
	rm -rf lab3b
	rm -rf lab3b-904799099.tar.gz
dist:
	tar -cvzf lab3b-904799099.tar.gz README lab3b.py Makefile
