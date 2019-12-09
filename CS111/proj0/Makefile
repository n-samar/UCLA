main: lab0.c
	gcc -Wall -Wextra lab0.c -o lab0
clean:
	rm -rf lab0;
	rm -rf lab0-904799099.tar.gz;

dist: lab0.c Makefile backtrace.png breakpoint.png README
	tar -czvf lab0-904799099.tar.gz lab0.c Makefile backtrace.png breakpoint.png README

check: OK_exit_code standard input_redirect output_redirect segfault_test segflt

OK_exit_code: main
	echo "Hello" | ./lab0 &>/dev/null;
	if [[ $$? -ne 0 ]]; then \
		echo "Error: Exit status incorrect on success"; \
	fi;

standard: main
	echo "Hello" 1>test2.txt;
	echo "Hello" | ./lab0 1>test.txt;
	cmp test.txt test2.txt;
	if [[ $$? -ne 0 ]]; then \
		echo "Error: Incorrect output"; \
	fi; \
	rm -f test.txt

input_redirect: main
	echo "Wassup?" 1>test.txt; \
	./lab0 --input=test.txt 1>test2.txt; \
	cmp test.txt test2.txt; \
	if [[ $$? -ne 0 ]]; then  \
		echo "Error: Input redirection failed"; \
	fi; \
	rm -f test.txt test2.txt 

output_redirect: main
	echo "Wassup?" 1>test.txt; \
	./lab0 --input=test.txt --output=test2.txt; \
	cmp test.txt test2.txt; \
	if [[ $$? -ne 0 ]]; then \
		echo "Error: Output redirection failed"; \
	fi; \
	rm -f test.txt test2.txt

segfault_test: main
	./lab0 --segfault --catch &>/dev/null; \
	if [[ $$? -ne 4 ]]; then \
		echo "Error: SIGSEGV not adequately handled"; \
	fi;

segflt: main
	./lab0 --segfault; \
	if [[ $$? -ne 139 ]]; then \
		echo "Error: SIGSEGV exit status incorrect."; \
	fi;
