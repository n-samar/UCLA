/********************************************************************
* Project 1B
* lab1b-server.c
* ------------
* NAME: Nikola Samardzic
* EMAIL: nikola.s@ucla.edu
* ID: 904799099
********************************************************************/

#include <mcrypt.h>
#include <netdb.h>
#include <fcntl.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <termios.h>
#include <stdio.h>
#include <getopt.h>
#include <errno.h>
#include <unistd.h>
#include <poll.h>
#include <signal.h>

int SHELL_END = 0;
int TERMINAL_END = 0;
struct pollfd poll_list[2];
size_t key_len = 0;
MCRYPT encrfd, decrfd;
char *IV, *IV2;
char* key;
pid_t new_pid;
void mcrypt_close () {
  mcrypt_generic_deinit(encrfd);
  mcrypt_module_close(encrfd);
  mcrypt_generic_deinit(decrfd);
  mcrypt_module_close(decrfd);    
}

void error_exit() {
  exit(1);
}

int translate_me(char *msg, int str_len) {
  char result[256];
  bzero(result, 256);
  int index = 0;
  for (int i = 0; i < str_len; i++) {
    if (msg[i] != '\n' && msg[i] != '\r') {
      result[index] = msg[i];
    } else {
      result[index++] = '\r';
      result[index] = '\n';
    }
    index++;
  }
  for (int i = 0; i < index; i++) msg[i] = result[i];
  return index;
}

int read_from(int fd, char* buff, int max_read) {
    bzero(buff, max_read);
    int read_size;
    if ((read_size = read(fd, buff, max_read)) == -1) {
        fprintf(stderr, "ERROR reading from file descriptor %d", fd);
        error_exit();
    }
    return read_size;
}

int write_to(int fd, char *msg, size_t length) {
    int count;
    if ((count = write(fd, msg, length)) == -1) {
            fprintf(stderr, "ERROR writing to file descriptor %d", fd);
            error_exit();
    }
    return count;
}

void decrypt (char *buff, size_t read_size) {
  if (mdecrypt_generic(decrfd, buff, read_size)) {
    fprintf (stderr, "ERROR while decrypting\n");
    error_exit();
  }
}

void encrypt (char *buff, size_t read_size) {
  if(mcrypt_generic(encrfd, buff, read_size)) {
    fprintf (stderr, "ERROR while encrypting\n");
    error_exit();
  }
}

void get_key(char *file) {
  struct stat key_data;
  int fd = open(file, O_RDONLY);
  if (fstat(fd, &key_data) == -1) {
    fprintf(stderr, "ERROR reading encryption key\n");
    error_exit();
  }
  key = (char*)malloc(key_data.st_size);
  read_from(fd, key, key_data.st_size);
  key_len = key_data.st_size;
}


void mcrypt_setup () {
  if ((encrfd = mcrypt_module_open("blowfish", NULL, "cfb", NULL)) == MCRYPT_FAILED) {
    fprintf(stderr, "ERROR initializing the encryption module\n");
    error_exit();
  }
  IV = (char*)malloc(mcrypt_enc_get_iv_size(encrfd));
  char vector[5] = "hello";
  bzero(IV, mcrypt_enc_get_iv_size(encrfd));
  for (int i = 0; i < mcrypt_enc_get_iv_size(encrfd); i++) IV[i] = vector[i % 5];
  if (mcrypt_generic_init(encrfd, key, key_len, IV) == -1) {
    fprintf(stderr, "ERROR initializing the decryption module\n");
    error_exit();
  }
  if ((decrfd = mcrypt_module_open("blowfish", NULL, "cfb", NULL)) == MCRYPT_FAILED) {
    fprintf(stderr, "ERROR initializing the encryption module\n");
    error_exit();
  }
  IV2 = (char*)malloc(mcrypt_enc_get_iv_size(decrfd));
  bzero(IV2, mcrypt_enc_get_iv_size(decrfd));
  for (int i = 0; i < mcrypt_enc_get_iv_size(decrfd); i++) IV2[i] = vector[i % 5];
  if (mcrypt_generic_init(decrfd, key, key_len, IV2) == -1) {
    fprintf(stderr, "ERROR initializing the decryption module\n");
    error_exit();
  }  
}

void report_exit() {
    int status;
    if (waitpid(new_pid, &status, 0) == -1) {
        int err_num = errno;
        fprintf(stderr, "%s\n", strerror(err_num));
        exit(1);
    } else {
        char temp_buff[256];
        bzero(temp_buff, 256);
        fprintf(stderr, "\r\nSHELL EXIT SIGNAL=%d STATUS=%d\r\n",
                WTERMSIG(status), WEXITSTATUS(status));
        exit(0);
    }
}

void printUsage() {
    fprintf (stderr, "Usage: ./lab1b-server --port=number\n");
}

void SIGPIPE_handler(int signum) {
    if (signum == SIGPIPE) {
        close(poll_list[0].fd);
        close(poll_list[1].fd);
        SHELL_END = 1;
    }
}



void start_shell(int socketfd) {
  signal(SIGPIPE, *SIGPIPE_handler);
  int pipefd_in[2];
  int pipefd_out_err[2];
  if (pipe(pipefd_in) == -1) {
    fprintf(stderr, "ERROR in creating pipe\n");
    exit(1);
  }
  if (pipe(pipefd_out_err) == -1) {
    fprintf(stderr, "ERROR in creating pipe\n");
    exit(1);
  }
  new_pid = fork();
  if (new_pid == -1) {
    fprintf(stderr, "ERROR in forking process\n");
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
      fprintf(stderr, "ERROR in executing bash\n");
      exit(1);
    }
  } else {
    struct pollfd sock;
    sock.fd = socketfd;
    sock.events = POLLIN | POLLHUP | POLLERR;
    struct pollfd shell_output;
    shell_output.fd = pipefd_out_err[0];
    shell_output.events = POLLIN | POLLHUP | POLLERR;	

    poll_list[0] = sock;
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
      /* if there is something to read from the socket */
      if (poll_list[0].revents & POLLIN) {
	read_size = read_from(socketfd, buff, 256);
	if (key_len != 0) {
	  for(size_t i = 0; i < read_size; i++)
	    decrypt(buff + i, 1);
	}

	int should_i_write = 1;
	for (size_t i = 0; i < read_size; i++) {
	  char c = buff[i];
	  if (c == 0x03) {
	    kill(new_pid, SIGINT);
	  } else if (c == 0x04) {
	    should_i_write = 0;
	    if (SHELL_END == 1) exit(0);
	    if (SHELL_END == 0) close(pipefd_in[1]);		/* close write pipe to shell */
	    TERMINAL_END = 1;
	  }
	}
	if (should_i_write) write_to(pipefd_in[1], buff, read_size);
      }

      /* if there is some output from the shell */
      if (poll_list[1].revents & POLLIN) {
	read_size = read_from(pipefd_out_err[0], buff, 256);
	read_size = translate_me(buff, read_size);
	int close_the_store = 0;
	for (size_t i = 0; i < read_size; i++) {
	  char c = buff[i];
	  if (c == EOF) {
	    close_the_store = 1;
	  }
	}
	for (size_t i = 0; i < read_size; i++) {
	  char ch = buff[i];
	  if(key_len != 0) {
	    encrypt(&ch, 1);
	  }
	  write_to(socketfd, &ch, 1);
	}
	if (close_the_store == 1) {
	    close(pipefd_out_err[0]);	/* close read pipe from shell */
	    close(socketfd);
	}
      }

      /* if shell is hung-up */
      if (poll_list[1].revents & POLLHUP) {			  
	close(pipefd_out_err[0]);	/* close read pipe from shell */
	report_exit();
      }

      /* if there is an issue with the socket */
      if (poll_list[0].revents & POLLERR || poll_list[0].revents & POLLHUP) {
	read_size = read_from(pipefd_out_err[0], buff, 256);
	report_exit();
            
	for (size_t i = 0; i < read_size; i++) {
	  char c = buff[i];
	  if (c == 0x04 && TERMINAL_END == 1) {
	    close(pipefd_out_err[0]);	/* close read pipe from shell */
	    report_exit();
	  }
	  else if (c == '\r' || c == '\n') {
	    char *res = (char*)malloc(2 * sizeof(char));
	    res = "\r\n";
	    write_to(socketfd, res, 2);
	  } else {
	    write_to(socketfd, &c, 1);
	  }
	}
      }
    }
  }
}

char* encrypt_file;
 int main (int argc, char *argv[]) {
   encrypt_file = (char*)malloc(256);
   bzero(encrypt_file, 256);
   /* set long option arguments to command line */
   static struct option long_options[] = {
     {"port", required_argument, 0, 'p'},
     {"encrypt", required_argument, 0, 'e'},
     {0, 0, 0, 0}
   };
    
   int port = -1;
   char opt;
   /* process command line arguments */
   while ((opt =
	   getopt_long(argc, argv, "p:e:", long_options, NULL)) != -1) {
     switch (opt) {
     case 'p':
       if (optarg) {
	 port = atoi(optarg);
       } else {
	 fprintf(stderr, "./lab1b-server: `--port' option passed \
                                 with no argument\n");
	 printUsage();
	 error_exit();
       }
       break;
     case 'e':
       if (optarg) {
	 get_key(optarg);
	 mcrypt_setup();
       } else {
	 fprintf(stderr, "./lab1b-server: `--encrypt' option passed \
                                 with no argument\n");
	 printUsage();
	 error_exit();
       }
       break;	    
     default:
       fprintf(stderr, "./lab1b-server: Unrecognized argument.\n");
       printUsage();
       error_exit();
     }
   }

   if (port < 2000) {
     fprintf (stderr, "./lab1b-server: `--port=number' option must be \
                              specified with a value greater than 2000. The port value was %d\n", port);
     printUsage();
     error_exit();
   }

   /* Server setup */
   int sockfd, newsockfd;
   socklen_t clilen;
   struct sockaddr_in serv_addr, cli_addr;
   sockfd = socket (AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0) {
     fprintf(stderr, "ERROR opening socket\n");
     error_exit();
   }
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(port);
   if (bind(sockfd,
	    (struct sockaddr *) &serv_addr,
	    sizeof(serv_addr)) < 0) {
     fprintf(stderr, "ERROR on binding\n");
     error_exit();
   }
   listen(sockfd, 5);
   clilen = sizeof(cli_addr);
   newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
   if (newsockfd < 0) {
     fprintf(stderr, "ERROR on accept\n");
     error_exit();
   }

   start_shell(newsockfd);
   return 0;
 }
