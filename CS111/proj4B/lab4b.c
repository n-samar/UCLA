/********************************************************************
 Project 4B
 lab4b.c
------------
 NAME: Nikola Samardzic
 EMAIL: nikola.s@ucla.edu
 ID: 904799099
********************************************************************/

#include <signal.h>
#include <getopt.h>
#include <stdio.h>
#include <mraa.h>
#include <math.h>
#include <time.h>
#include <poll.h>
#include <string.h>

int log_fd = -1;
time_t curr_time_raw;
struct tm *curr_time;
static unsigned int period  = 1;
static char scale = 'F';
static char* logfile = NULL;
char* buffer;
char* unprocessed;

sig_atomic_t volatile run_flag = 1;
sig_atomic_t volatile stop = 0;

void printUsage() {
  fprintf (stderr,
	   "\rUsage: ./lab4b [ --period=number ] [ --scale=F|C ] [ --log=filename ]\n\r");
}

void do_when_interrupted () {
    run_flag = 0;
}

void exec_cmd(char *cmd) {
  if (log_fd != -1) {
    write(log_fd, cmd, strlen(cmd));
    char nl = '\n';
    write(log_fd, &nl, 1);
  }
  if (strcmp(cmd, "STOP") == 0) {
    stop = 1;
  } else if (strcmp(cmd, "START") == 0) {
    stop = 0;
  } else if (strcmp(cmd, "OFF") == 0) {
    time(&curr_time_raw);
    curr_time = localtime(&curr_time_raw);
    char output_str[100];
    sprintf(output_str, "%02d:%02d:%02d SHUTDOWN\n", curr_time->tm_hour,
	    curr_time->tm_min, curr_time->tm_sec);
    printf(output_str);
    if (log_fd != -1) write(log_fd, output_str, strlen(output_str));
    run_flag = 0;
    raise(SIGINT);
  } else {
    char eq = '=';
    char *lhs = strtok(cmd, &eq);
    if (lhs == NULL) return;
    char *rhs = strtok(NULL, &eq);
    if (rhs == NULL) return;
    if (strcmp(lhs, "SCALE") == 0) {
      if (*rhs == 'F' || *rhs == 'C')
	scale = *rhs;
    } else if (strcmp(lhs, "PERIOD") == 0) {
      sscanf(rhs, "%d", &period);      
    }
  }
}

void process() {
  size_t len_unpr = strlen(unprocessed);
  char *next_cmd;
  char *saveptr;
  char nl = '\n';
  if (len_unpr == 0) return;
  for (next_cmd = strtok_r(unprocessed, &nl, &saveptr);
       1;
       next_cmd = strtok_r(NULL, &nl, &saveptr)) {
    if (next_cmd == NULL) {
      bzero(unprocessed, 512);
      break;
    }
    if (unprocessed + len_unpr > next_cmd + strlen(next_cmd)) {
      exec_cmd(next_cmd);
    }else {
      char* temp = malloc(50*sizeof(char));
      strcpy(temp, next_cmd);
      bzero(unprocessed, 512);
      strcpy(unprocessed, temp);
      free(temp);
      break;
    }
  }
}

int main (int argc, char *argv[]) {  
  unprocessed = malloc(512*sizeof(char));
  bzero(unprocessed, 512);
  buffer = malloc(256*sizeof(char));
  bzero(buffer, 256);
  signal(SIGINT, &do_when_interrupted);
  /* Read command-line arguments */
  static struct option long_options[] = {
    {"period", required_argument, 0, 'p'},
    {"scale", required_argument, 0, 's'},
    {"log", required_argument, 0, 'l'},
    {0, 0, 0, 0}
  };

  int opt;
  while ((opt = getopt_long(argc, argv, "l:s:p:", long_options, NULL))) {
    if (opt == -1) break;
    switch (opt) {
    case 'l':
      if (optarg) {
	logfile = optarg;
	log_fd = open(logfile, O_WRONLY | O_CREAT, 0640);
      }
      else {
	fprintf(stderr, "ERROR: `--log' option must be followed by a specified filename.\r\n");
	printUsage();
	exit(1);
      }
      break;
    case 's':
      if (optarg && (*optarg == 'F' || *optarg == 'C')) {
	scale = *optarg;
      } else {
	fprintf(stderr, "ERROR: `--scale' option must be followed by an `F' (Farenheit) or `C' (Celsius).\r\n");
	printUsage();
	exit(1);
      }
      break;
    case 'p':
      if (optarg) {
	sscanf(optarg, "%d", &period);
      } else {
	fprintf(stderr, "ERROR: `--period' option must be followed by argument of type double.\r\n");
	printUsage();
	exit(1);
      }

      break;
    default:
      printUsage();
      exit(1);
    }
  }
  
  uint16_t value;
  mraa_aio_context temp_sensor;
  temp_sensor = mraa_aio_init(1);

  mraa_gpio_context button = mraa_gpio_init(60);
  mraa_gpio_dir(button, MRAA_GPIO_IN);

  mraa_gpio_isr(button, MRAA_GPIO_EDGE_RISING, &do_when_interrupted, NULL);

  struct pollfd keyboard;
  keyboard.fd = STDIN_FILENO;
  keyboard.events = POLLIN | POLLHUP | POLLERR;
  while (run_flag) {
    poll (&keyboard, 1, 0);
    if (keyboard.revents & POLLIN) {
      read(STDIN_FILENO, buffer, 256);
      strcat(unprocessed, buffer);
      process();
      bzero(buffer, 256);
    }
    if (!stop && run_flag) {
      time(&curr_time_raw);
      curr_time = localtime(&curr_time_raw);
      value = mraa_aio_read(temp_sensor);
      double R =  1023.0/value - 1.0;
      /* convert voltage to Celsius */
      double temperature = 1.0/(log(R)/4275 + 1/298.15) - 273.15;
      if (scale == 'F')
	temperature = temperature*9.0/5 + 32;
      char output_str[100];
      sprintf(output_str, "%02d:%02d:%02d %.1lf\n", curr_time->tm_hour,
	     curr_time->tm_min, curr_time->tm_sec, temperature);
      printf(output_str);
      write(log_fd, output_str, strlen(output_str));
      sleep(period);
    }
  }
  mraa_aio_close(temp_sensor);
  mraa_gpio_close(button);
  return 0;
}
