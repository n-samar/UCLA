#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <string.h>

void handler(int signum) {
  int err_num = signum;
  if (err_num == SIGSEGV) {
    fprintf(stderr, "%s\n", strerror(err_num));
    exit(4);
  }
}

int main(int argc, char *argv[]) {
  int opt;
  int opt_catch = 0, opt_segfault = 0, opt_input = 0, opt_output = 0;
  char *input = NULL;
  char *output = NULL;
  while (1) {
    static struct option long_options[] = {
      {"input", 1, 0, 'i'},
      {"output", 1, 0, 'o'},
      {"segfault", 0, 0, 's'},
      {"catch", 0, 0, 'c'},
      {0, 0, 0, 0}
    };

    opt = getopt_long(argc, argv, "i:o:sc",
                      long_options, NULL);
    if (opt == -1)
      break;

    switch(opt) {
      case 's':
        opt_segfault = 1;
        break;
      case 'c':
	    opt_catch = 1;
	    break;
      case 'i':
	 opt_input = 1;
        if (optarg) {
          input = optarg;
        } else{
          fprintf(stderr, "./lab0: No input file specified!\n");
          exit(2);
        }
        break;
      case 'o':
        opt_output = 1;
        if (optarg) {
	   output = optarg;
        } else {
          fprintf(stderr, "./lab0: No output file specified!\n");
          exit(3);
        }
        break;
      default:
        fprintf(stderr, "./lab0: Unrecognized argument!\n");
	fprintf(stderr, "Usage: ./lab0 --input=filename --output=filename --catch --segfault\n");
        exit(1);
    }
  }

  if (opt_input == 1) {
    int ifd = open(input, O_RDONLY);

    if (ifd >= 0) {
      close(0);
      dup(ifd);
      close(ifd);
    } else {
      if (errno == EACCES) {
      fprintf(stderr, "Error: Do not have permission to read File specified for input!\n");
      exit(2);      	
      } else {
	fprintf(stderr, "Error: File specified for input does not exist!\n");
	exit(2);
      }
    }
  }
  if (opt_output == 1) {
    int ofd = creat(output, 0644);

    if (ofd >= 0) {
      close(1);
      dup(ofd);
      close(ofd);
    } else {
      if (errno == EACCES) {
	fprintf(stderr, "./lab0: Could not open output file!\n");
	exit(3);
      }else {
	fprintf(stderr, "./lab0: Could not create output file!\n");
        exit(3);
      }
      
    }
  }

  if (opt_catch == 1) {
    signal(11, *handler);
  }
  if (opt_segfault == 1) {
    char* p = NULL;
    *p = 'e';
  }

  char c;
  while (read(0, &c, 1) != 0) {
    write(1, &c, 1);
  }
  exit(0);
}
