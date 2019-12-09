/********************************************************************
 Project 2A
 lab2_add.c
------------
 NAME: Nikola Samardzic
 EMAIL: nikola.s@ucla.edu
 ID: 904799099
********************************************************************/
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>

static int my_lock = 0;
int opt_yield = 0;
pthread_mutex_t mutex;
char sync_char = '-';

struct thread_info {
    long long *counter_ptr;
    size_t iterator;
    pthread_t thread_id;
    size_t thread_num;
};

void add_spin_lock (long long *pointer, long long value) {
    while (__sync_lock_test_and_set(&my_lock, 1) == 1) ;
    long long sum = *pointer + value;
    if (opt_yield) sched_yield();
    *pointer = sum;
    __sync_lock_release (&my_lock);
}

void add_c (long long *pointer, long long value) {
    long long curr = *pointer;
    while ((curr
	    != __sync_val_compare_and_swap(pointer, curr, curr+value))) {
	curr = *pointer;
	if (opt_yield) sched_yield();
    }
}

void add_mutex (long long *pointer, long long value) {
    pthread_mutex_lock(&mutex);
    long long sum = *pointer + value;
    if (opt_yield) sched_yield();
    *pointer = sum;
    pthread_mutex_unlock(&mutex);
}

void add(long long *pointer, long long value) {    
    long long sum = *pointer + value;
    if (opt_yield) sched_yield();
    *pointer = sum;
}

void printUsage() {
    fprintf (stderr, "\rUsage: ./lab2a_add [ --threads=number ] [ --iterations=number ]\n\r");
}

void *thread_start (void *void_ptr) {
    struct thread_info ptr = *((struct thread_info*)void_ptr);
    for (size_t i = 0; i < ptr.iterator; i++) {
	switch (sync_char) {
	case '-':
	    add(ptr.counter_ptr, 1);
	    add(ptr.counter_ptr, -1);
	    break;
	case 'm':
	    add_mutex(ptr.counter_ptr, 1);
	    add_mutex(ptr.counter_ptr, -1);
	    break;
	case 's':
	    add_spin_lock(ptr.counter_ptr, 1);
	    add_spin_lock(ptr.counter_ptr, -1);
	    break;
	case 'c':
	    add_c(ptr.counter_ptr, 1);
	    add_c(ptr.counter_ptr, -1);
	    break;
	}
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    static struct option long_options[] = {
	{"threads", required_argument, 0, 't'},
	{"iterations", required_argument, 0, 'i'},
	{"yield", no_argument, 0, 'y'},
	{"sync", required_argument, 0, 's'},
        {0, 0, 0, 0}
    };

    /* Read command-line arguments */
    char opt;
    unsigned int num_threads = 1;
    unsigned int num_iterations = 1;
    while ((opt = getopt_long(argc, argv, "t:i:", long_options, NULL))) {
        if (opt == -1) break;
        switch (opt) {
        case 't':
            if (optarg) num_threads = strtol(optarg, NULL, 10);
            break;
        case 'i':
            if (optarg) num_iterations = strtol(optarg, NULL, 10);
            break;
	case 'y':
	    opt_yield = 1;
	    break;
	case 's':
	    if (optarg) {
		switch(*optarg) {
		case 'm':
		    pthread_mutex_init(&mutex, NULL);
		    sync_char = 'm';
		    break;
		case 's':
		    sync_char = 's';
		    break;
		case 'c':
		    sync_char = 'c';
		    break;
		default:
		    fprintf(stderr, "./lab2_add: The `--sync' option takes on only arguments `c', `s', or `m'\n\r");
		    printUsage();
		    exit(1);
		}
	    }
	    else {
		fprintf(stderr,
	       "./lab2_add: The `--sync' option requires an argument\n\r");
	        printUsage();
		exit(1);
	    }
	    break;
        default:
            printUsage();
	    exit(1);
        }
    }

    struct timespec start_timespec;
    clock_gettime(CLOCK_REALTIME, &start_timespec);
    long long counter = 0;
    
    struct thread_info *tinfo = calloc(num_threads,
				       sizeof(struct thread_info));
    if (tinfo == NULL) {
        fprintf(stderr, "./lab2_add: Call to malloc failed\r\n");
        exit(2);
    }
    
    for (size_t tnum = 0; tnum < num_threads; tnum++) {
	tinfo[tnum].thread_num = tnum + 1;
	tinfo[tnum].counter_ptr = &counter;
	tinfo[tnum].iterator = num_iterations;

	/* The pthread_create() call stores the thread ID into
           corresponding element of tinfo[] */

	int create_result = pthread_create(&(tinfo[tnum].thread_id),
					   NULL,
					   &thread_start,
					   (void*)(&tinfo[tnum]));
        if (create_result != 0)
            fprintf(stderr,
                    "./lab2_add: ERROR while creating new thread\n\r");
    }

    for (size_t tnum = 0; tnum < num_threads; tnum++) {
	int rc = pthread_join(tinfo[tnum].thread_id, NULL);
	if (rc) {
	    fprintf (stderr, "./lab2_add: ERROR while joining thread\n\r");
	    exit(2);
	}
    }

    struct timespec end_timespec;
    clock_gettime(CLOCK_REALTIME, &end_timespec);
    
    long total_time = 1000000000L*(end_timespec.tv_sec - start_timespec.tv_sec) + (end_timespec.tv_nsec - start_timespec.tv_nsec);
    char* first_string = (char*)malloc(100);
    switch (sync_char) {
    case '-':
	if (opt_yield) first_string = "add-yield-none";
	else first_string = "add-none";
	break;
    case 's':
	if (opt_yield) first_string = "add-yield-s";
	else first_string = "add-s";	
	break;
    case 'm':
	if (opt_yield) first_string = "add-yield-m";
	else first_string = "add-m";	
	break;
    case 'c':
	if (opt_yield) first_string = "add-yield-c";
	else first_string = "add-c";
	break;
    }
    fprintf(stdout, "%s,%d,%d,%d,%ld,%d,%lld\n", 
	    first_string, num_threads, num_iterations, 
	    2*num_threads*num_iterations, total_time,
	    (int)(total_time/(2.0*num_threads*num_iterations)), counter);
    free(tinfo);
    exit(0);
}
