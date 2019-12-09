/********************************************************************
 Project 1A
 lab1a.c
------------
 NAME: Nikola Samardzic
 EMAIL: nikola.s@ucla.edu
 ID: 904799099
********************************************************************/
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int SHELL_END = 0;
int TERMINAL_END = 0;
struct termios _old_termios;
struct pollfd poll_list[2];

static void SIGPIPE_handler(int signum) {
  if (signum == SIGPIPE) {
    close(poll_list[0].fd);
    close(poll_list[1].fd);
    SHELL_END = 1;
  }
}


void start_shell(struct termios _old_termios) {
	signal(SIGPIPE, *SIGPIPE_handler);
	int pipefd_in[2];
	int pipefd_out_err[2];
	if (pipe(pipefd_in) == -1) {
		int err_num = errno;
		fprintf(stderr, "%s\n", strerror(err_num));
		exit(1);
	}
	if (pipe(pipefd_out_err) == -1) {
		int err_num = errno;
		fprintf(stderr, "%s\n", strerror(err_num));
		exit(1);
	}
	pid_t new_pid = fork();
	if (new_pid == -1) {
		int err_num = errno;
		fprintf(stderr, "%s\n", strerror(err_num));
		exit(1);
	}
	if (new_pid == 0) {
		close(pipefd_in[1]);
		close(pipefd_out_err[0]);
		close(0);
		dup(pipefd_in[0]);		/* redirect STDIN to pipefd_in */
		close(1);
		dup(pipefd_out_err[1]);		/* redirect STDOUT to pipefd_out_err */
		close(2);
		dup(pipefd_out_err[1]);     /* redirect STDERR to pipefd_out_err */

		/* run bash */
		char **arguments = {NULL};
		if(execvp("/bin/bash", arguments) == -1) {
			int err_num = errno;
			fprintf(stderr, "%s\n", strerror(err_num));
			exit(1);
		}
	} else {
		struct pollfd keyboard;
		keyboard.fd = STDIN_FILENO;
		keyboard.events = POLLIN | POLLHUP | POLLERR;
		struct pollfd shell_output;
		shell_output.fd = pipefd_out_err[0];
		shell_output.events = POLLIN | POLLHUP | POLLERR;	

		poll_list[0] = keyboard;
		poll_list[1] = shell_output;	
		close(pipefd_in[0]);
		close(pipefd_out_err[1]);
		char *buff = (char*)malloc(256 * sizeof(char));
		size_t read_size;
		while(1) {
			poll(poll_list, 2, 0);
			/* if SIGPIPE raised (i.e. shell exited) */
			if (SHELL_END == 1) {
				close(pipefd_in[1]);	/* close write pipe to shell */
				poll(poll_list, 2, 0);  /* poll again */
			}
			if (poll_list[0].revents & POLLIN) {
				read_size = read(STDIN_FILENO, buff, 256);
				size_t i;
				for (i = 0; i < read_size; i++) {
					char c = buff[i];
					if (c == 0x03) {
						char *res = (char*)malloc(2 * sizeof(char));
						res = "^C";
						write(STDOUT_FILENO, res, 2);
						kill(new_pid, SIGINT);
					} else if (c == 0x04) {
						char *res = (char*)malloc(2 * sizeof(char));
						res = "^D";
						write(STDOUT_FILENO, res, 2);
						if (SHELL_END == 1) {
						  tcsetattr(STDIN_FILENO, TCSANOW, &_old_termios);
						  exit(0);
						}
						
						if (SHELL_END == 0) close(pipefd_in[1]);		/* close write pipe to shell */
						TERMINAL_END = 1;
						
					} else if (c == '\r' || c == '\n') {
						char *res = (char*)malloc(2 * sizeof(char));
						res = "\r\n";
						write(STDOUT_FILENO, res, 2);
						if (SHELL_END == 0) write(pipefd_in[1], res + 1, 1);
					} else {
					 	write(STDOUT_FILENO, &c, 1);
					 	if (SHELL_END == 0) write(pipefd_in[1], &c, 1);
					}
			    }
			}

			if (poll_list[1].revents & POLLIN) {
			  read_size = read(pipefd_out_err[0], buff, 256);
			  size_t i;
			  for (i = 0; i < read_size; i++) {
			    char c = buff[i];
			    if (c == 0x04 && TERMINAL_END == 1) {
			      close(pipefd_out_err[0]);	/* close read pipe from shell */
			      int status;
			      if (waitpid(new_pid, &status, 0) == -1) {
				int err_num = errno;
				fprintf(stderr, "%s\n", strerror(err_num));
				exit(1);
			      } else {
				fprintf(stderr, "\r\nSHELL EXIT SIGNAL=%d STATUS=%d\r\n", WTERMSIG(status), WEXITSTATUS(status));
				tcsetattr(STDIN_FILENO, TCSANOW, &_old_termios);
				exit(0);
			      }
			    } else if (c == 0x04 && TERMINAL_END == 0) {
			      SHELL_END = 1;
			    } else if (c == '\r' || c == '\n') {
			      char *res = (char*)malloc(2 * sizeof(char));
			      res = "\r\n";
			      write(STDOUT_FILENO, res, 2);
			    } else {
			      write(STDOUT_FILENO, &c, 1);
			    }
			  }
			}
			
			if (poll_list[1].revents & POLLHUP) {			  
			  int status;
			  close(pipefd_out_err[0]);	/* close read pipe from shell */
			  if (waitpid(new_pid, &status, 0) == -1) {
			    int err_num = errno;
			    fprintf(stderr, "%s\n", strerror(err_num));
			    exit(1);
			  } else {
			    fprintf(stderr, "\r\nSHELL EXIT SIGNAL=%d STATUS=%d\r\n", WTERMSIG(status), WEXITSTATUS(status));
			    SHELL_END = 1;
			    if (TERMINAL_END == 1) {
			      tcsetattr(STDIN_FILENO, TCSANOW, &_old_termios);
			      exit(0);
			    }
			  }
			}
			
			if (poll_list[0].revents & POLLERR || poll_list[0].revents & POLLHUP) {
				read_size = read(pipefd_out_err[0], buff, 256);
				if (read_size == 0) {
					close(pipefd_out_err[0]);	/* close read pipe from shell */
					int status;
					if (waitpid(new_pid, &status, 0) == -1) {
						int err_num = errno;
						fprintf(stderr, "%s\n", strerror(err_num));
						exit(1);
					} else {
						fprintf(stderr, "\r\nSHELL EXIT SIGNAL=%d STATUS=%d\r\n", WTERMSIG(status), WEXITSTATUS(status));
						tcsetattr(STDIN_FILENO, TCSANOW, &_old_termios);
						exit(0);
					}
				}
				size_t i;
				for (i = 0; i < read_size; i++) {
					char c = buff[i];
					if (c == 0x04 && TERMINAL_END == 1) {
						close(pipefd_out_err[0]);	/* close read pipe from shell */
						int status;
						if (waitpid(new_pid, &status, 0) == -1) {
							int err_num = errno;
							fprintf(stderr, "%s\n", strerror(err_num));
							exit(1);
						} else {
							fprintf(stderr, "\r\nSHELL EXIT SIGNAL=%d STATUS=%d\r\n", WTERMSIG(status), WEXITSTATUS(status));
							tcsetattr(STDIN_FILENO, TCSANOW, &_old_termios);
							exit(0);
						}
					}
					else if (c == '\r' || c == '\n') {
						char *res = (char*)malloc(2 * sizeof(char));
						res = "\r\n";
						write(STDOUT_FILENO, res, 2);
					} else {
					 	write(STDOUT_FILENO, &c, 1);
					}
			    }
			}
		}
	}
}

int main(int argc, char *argv[]) {
	struct termios _termios;

	tcgetattr(STDIN_FILENO, &_termios);
	tcgetattr(STDIN_FILENO, &_old_termios);	/* store old values of tcflags */

	_termios.c_iflag = ISTRIP;	/* only lowercase 7 bits */
	_termios.c_oflag = 0;		/* no processing		 */
	_termios.c_lflag = 0;		/* no processing 		 */

	tcsetattr(STDIN_FILENO, TCSANOW, &_termios);	/* set new termios falgs */

	static struct option long_options[] = {
		{"shell", no_argument, 0, 's'},
        {0, 0, 0, 0}
	};

	char c = getopt_long(argc, argv, "s", long_options, NULL);
	if (c == 's') start_shell(_old_termios);
	else if (c != -1) {
	  fprintf(stderr, "./lab1a: Unrecognized argument!\n");
	  fprintf(stderr, "Usage: ./lab1a --shell\n");
          exit(1);
	}
	char *buff = (char*)malloc(256 * sizeof(char));
	size_t read_size;
	while((read_size = read(STDIN_FILENO, buff, 256)) != 0) {
	          size_t i;
	          for (i = 0; i < read_size; i++) {
			char c = buff[i];
			if (c == 0x04) {
				tcsetattr(STDIN_FILENO, TCSANOW, &_old_termios);
				exit(0);
			} else if (c == '\r' || c == '\n') {
				char *res = (char*)malloc(2 * sizeof(char));
				res = "\r\n";
				write(STDOUT_FILENO, res, 2);
			} else write(STDOUT_FILENO, &c, 1);
	    }
	}
	return 0;
}
