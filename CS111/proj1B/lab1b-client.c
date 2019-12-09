/********************************************************************
 * Project 1B
 * lab1b-client.c
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
#include <stdlib.h>
#include <termios.h>
#include <stdio.h>
#include <getopt.h>
#include <errno.h>
#include <unistd.h>
#include <poll.h>


struct termios _old_termios;   /* used to store original terminal modes */
int log_file_fd = -1;
size_t key_len = 0;
char* key;
MCRYPT encrfd, decrfd;
char *IV, *IV2;

void mcrypt_close () {
  mcrypt_generic_deinit(encrfd);
  mcrypt_module_close(encrfd);
  mcrypt_generic_deinit(decrfd);
  mcrypt_module_close(decrfd);    
}

/* Restores normal terminal modes, and exits */
void error_exit() {
  if(key_len != 0) mcrypt_close();
  tcsetattr(STDIN_FILENO, TCSANOW, &_old_termios);  /* restores termios */
  exit(1);
}

int read_from(int fd, char* buff, int max_read) {
  bzero(buff, max_read);
  int read_size;
  if ((read_size = read(fd, buff, max_read)) == -1) {
    fprintf(stderr, "ERROR reading from file descriptor %d\n\r", fd);
    error_exit();
  }
  if (read_size == 0) {
    if(key_len != 0) mcrypt_close();
    tcsetattr(STDIN_FILENO, TCSANOW, &_old_termios);  /* restores termios */
    exit(0);
  }
  return read_size;
}

int write_to(int fd, char *msg, size_t length) {
  int count;
  if ((count = write(fd, msg, length)) == -1) {
    fprintf(stderr, "ERROR writing to file descriptor %d\n\r", fd);
    error_exit();
  }
  return count;
}


void get_key(char *file) {
  struct stat key_data;
  int fd = open(file, O_RDONLY);
  if (fstat(fd, &key_data) == -1) {
    fprintf(stderr, "ERROR reading encryption key\n\r");
    error_exit();
  }
  key = (char*)malloc(key_data.st_size);
  read_from(fd, key, key_data.st_size);
  key_len = key_data.st_size;
}

int translate_me_echo(char *msg, int str_len) {
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
  for (size_t i = 0; i < strlen(result); i++) msg[i] = result[i];
  return index;
}

void translate_me(char *msg, int str_len) {
  char result[256];
  bzero(result, 256);
  int index = 0;
  for (int i = 0; i < str_len; i++) {
    if (msg[i] != '\r') {
      result[index] = msg[i];
    } else {
      result[index] = '\n';
    }
    index++;
  }
  for (size_t i = 0; i < strlen(result); i++) msg[i] = result[i];
}

void printUsage() {
  fprintf (stderr, "\rUsage: ./lab1b-client --port=number [ --log=filename ] [ --encrypt=filename ]\n\r");
}



void decrypt (char *buff, size_t read_size) {
  if (mdecrypt_generic(decrfd, buff, read_size)) {
    fprintf (stderr, "ERROR while decrypting\n\r");
    error_exit();
  }
}
void encrypt (char *buff, size_t read_size) {
  if(mcrypt_generic(encrfd, buff, read_size)) {
    fprintf (stderr, "ERROR while encrypting\n\r");
    error_exit();
  }
}

void mcrypt_setup () {
  if ((encrfd = mcrypt_module_open("blowfish", NULL, "cfb", NULL)) == MCRYPT_FAILED) {
    fprintf(stderr, "ERROR initializing the encryption module\n\r");
    error_exit();
  }
  IV = (char*)malloc(mcrypt_enc_get_iv_size(encrfd));
  char vector[5] = "hello";
  bzero(IV, mcrypt_enc_get_iv_size(encrfd));
  for (int i = 0; i < mcrypt_enc_get_iv_size(encrfd); i++) IV[i] = vector[i % 5];
  if (mcrypt_generic_init(encrfd, key, key_len, IV) == -1) {
    fprintf(stderr, "ERROR initializing the decryption module\n\r");
    error_exit();
  }
  if ((decrfd = mcrypt_module_open("blowfish", NULL, "cfb", NULL)) == MCRYPT_FAILED) {
    fprintf(stderr, "ERROR initializing the encryption module\n\r");
    error_exit();
  }
  IV2 = (char*)malloc(mcrypt_enc_get_iv_size(decrfd));
  bzero(IV2, mcrypt_enc_get_iv_size(decrfd));
  for (int i = 0; i < mcrypt_enc_get_iv_size(decrfd); i++) IV2[i] = vector[i % 5];

  if (mcrypt_generic_init(decrfd, key, key_len, IV2) == -1) {
    fprintf(stderr, "ERROR initializing the decryption module\n\r");
    error_exit();
  }  
}

/* Logs messages sent/received by the client */
void log_me(char* msg, int sent, int read_size) {
  if (log_file_fd != -1) {
    int fd = log_file_fd;
    if (fd == -1) {
      fprintf(stderr, "Unable to open or create log file\n\r");
      error_exit();
    }
        
    if (sent) {
      write_to(fd, "SENT ", 5); 
    } else {
      write_to(fd, "RECEIVED ", 9);
    }
    char int_string_buff[10];
    bzero(int_string_buff, 10);
    sprintf(int_string_buff, "%d", read_size);
    write_to(fd, int_string_buff, strlen(int_string_buff));
    write_to(fd, " bytes: ", 7);
    write_to(fd, msg, read_size);
    write_to(fd, "\n", 1);
  }
}

int main (int argc, char *argv[]) {
  char *encrypt_file = (char*)malloc(256);
  bzero(encrypt_file, 256);
  struct termios _termios;
  /* store old values of tcflags */
  tcgetattr(STDIN_FILENO, &_old_termios);

  _termios.c_iflag = ISTRIP;	                  /* only lowercase 7 bits */
  _termios.c_oflag = 0;		                  /* no processing		 */
  _termios.c_lflag = 0;		                  /* no processing 		 */
  tcsetattr(STDIN_FILENO, TCSANOW, &_termios);  /* set new termios falgs */

  /* set long option arguments to command line */
  static struct option long_options[] = {
    {"port", required_argument, 0, 'p'},
    {"log", required_argument, 0, 'l'},        
    {"encrypt", required_argument, 0, 'e'},
    {0, 0, 0, 0}
  };
    
  int port = -1;
  char *host = "localhost";
  char *log_file = NULL;
  char opt;
  /* process command line arguments */
  while ((opt =
	  getopt_long(argc, argv, "p:l:e:", long_options, NULL)) != -1) {
    switch (opt) {
    case 'e':
      if (optarg) {
        get_key(optarg);
	mcrypt_setup();
      } else {
	fprintf(stderr, "./lab1b-client: `--encrypt' option passed \
                                 with no argument\n\r");
	printUsage();
	error_exit();		
      }
      break;
    case 'p':
      if (optarg) {
	port = atoi(optarg);
      } else {
	fprintf(stderr, "./lab1b-client: `--port' option passed \
                                 with no argument\n\r");
	printUsage();
	error_exit();
      }
      break;
    case 'l':
      if (optarg) {
	log_file = optarg;
	if ((log_file_fd =
	     open(log_file, O_RDWR | O_TRUNC  | O_CREAT, 0666)) == -1) {
	  fprintf(stderr, "./lab1b-client: Log file must be both readable and writable\n\r");
	  error_exit();
	}
      } else {
	fprintf(stderr, "./lab1b-client: `--log' option passed \
                                     with no argument\n\r");
	printUsage();
	error_exit();
      }
      break;   
    default:
      printUsage();
      error_exit();
    }
  }

  if (port < 2000) {
    fprintf (stderr, "./lab1b-client: `--port=number' option must be \
                              specified with a value greater than 2000.\n\r");
    printUsage();
    error_exit();
  }

  /* Socket setup */
  int sockfd;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    fprintf(stderr, "ERROR opening socket\n\r");
    error_exit();
  }
  server = gethostbyname(host);
  if (server == NULL) {
    fprintf (stderr, "ERROR, no such host\n\r");
    error_exit();
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
	(char *)&serv_addr.sin_addr.s_addr,
	server->h_length);
  serv_addr.sin_port = htons(port);
  if (connect(sockfd,
	      (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    fprintf(stderr, "ERROR connecting\n\r");
    error_exit();
  }

  /* polling setup */
  struct pollfd poll_list[2];
  struct pollfd keyboard;
  keyboard.fd = STDIN_FILENO;
  keyboard.events = POLLIN | POLLHUP | POLLERR;
  struct pollfd socket_output;
  socket_output.fd = sockfd;
  socket_output.events = POLLIN | POLLHUP | POLLERR;
  poll_list[0] = keyboard;
  poll_list[1] = socket_output;

  while (1) {
    poll (poll_list, 2, 0);

    /* read from keyboard, write to stdout and socket */
    if (poll_list[0].revents & POLLIN) {
      char* result = (char*)malloc(256);
      int read_size = read_from(STDIN_FILENO, result, 256);

      char* echo_result = (char*)malloc(256);
      for (int i = 0; i < 256; i++) echo_result[i] = result[i];
      translate_me_echo(echo_result, read_size);
      write_to(STDOUT_FILENO, echo_result, strlen(echo_result));
      translate_me(result, read_size);
      read_size = strlen(result);
      if (key_len != 0) {
	for (int i = 0; i < read_size; i++) {
	  encrypt(result + i, 1);
	}
      }
      log_me(result, 1, read_size);
      write_to(sockfd, result, read_size);
    }
    /* read from socket, write to stdout */
    if (poll_list[1].revents & POLLIN) {
      char* result = (char*)malloc(256);
      int read_size = read_from(poll_list[1].fd, result, 256);
      log_me(result, 0, read_size);
      if (key_len != 0){
	for(int i = 0; i < read_size; i++)
	  decrypt(result + i, 1);
      }
      if (result[0] == EOF) {
	tcsetattr(STDIN_FILENO, TCSANOW, &_old_termios);  /* restores termios */
	exit(0);
      }
      write_to(STDOUT_FILENO, result, read_size);
    }
  }
  return 0;
}
