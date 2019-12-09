/********************************************************************
 Project 2B
 lab2_list.c
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
#include "SortedList.h"

int my_lock = 0;
int opt_yield = 0;
pthread_mutex_t mutex;
char sync_char = '-';

struct thread_info {
    SortedListElement_t *sle_arr;
    SortedList_t *head;
    size_t iterator;
    pthread_t thread_id;
    size_t thread_num;
};

void *thread_start (void *void_ptr) {
    struct thread_info ptr = *((struct thread_info*)void_ptr);
    for (size_t i = (ptr.thread_num)*(ptr.iterator); i < (ptr.thread_num + 1)*(ptr.iterator); i++) {
	switch (sync_char) {
	case '-':
	    SortedList_insert(ptr.head, &(ptr.sle_arr[i]));
	    break;
	case 'm':
	    pthread_mutex_lock(&mutex);
	    SortedList_insert(ptr.head, &(ptr.sle_arr[i]));
	    pthread_mutex_unlock(&mutex);
	    break;
	case 's':
	    while (__sync_lock_test_and_set(&my_lock, 1) == 1);
	    SortedList_insert(ptr.head, &(ptr.sle_arr[i]));
	    __sync_lock_release (&my_lock);	    
	    break;
	}
    }

    switch (sync_char) {
    case '-':
	if (SortedList_length(ptr.head) == -1) exit(1);
	break;
    case 'm':
	pthread_mutex_lock(&mutex);
	if (SortedList_length(ptr.head) == -1) exit(1);
	pthread_mutex_unlock(&mutex);
	break;
    case 's':
	while (__sync_lock_test_and_set(&my_lock, 1) == 1);
	if (SortedList_length(ptr.head) == -1) exit(1);
	__sync_lock_release (&my_lock);	    
	break;
    }
	
    
    for (size_t i = (ptr.thread_num)*(ptr.iterator); i < (ptr.thread_num + 1)*(ptr.iterator); i++) {
	switch (sync_char) {
	case '-':
	    if (SortedList_lookup(ptr.head, ptr.sle_arr[i].key) == 0) 
		exit(2);
	    if(SortedList_delete(&(ptr.sle_arr[i])) == 1) 
		exit(2);
	    break;
	case 'm':
	    pthread_mutex_lock(&mutex);
	    if (SortedList_lookup(ptr.head, ptr.sle_arr[i].key) == 0) 
		exit(2);
	    if(SortedList_delete(&(ptr.sle_arr[i])) == 1) 
		exit(2);
	    pthread_mutex_unlock(&mutex);
	    break;
	case 's':
	    while (__sync_lock_test_and_set(&my_lock, 1) == 1);
	    if (SortedList_lookup(ptr.head, ptr.sle_arr[i].key) == 0) 
		exit(2);
	    if(SortedList_delete(&(ptr.sle_arr[i])) == 1) 
		exit(2);
	    __sync_lock_release (&my_lock);	    
	    break;
	}
    }
    return NULL;
}

void printUsage() {
    fprintf (stderr, "\rUsage: ./lab2_list [ --threads=number ] [ --iterations=number ] [ --yield=[ild] ]\n\r");
}


char* get_sync() {
    char* first_string = (char*)malloc(100);
    bzero(first_string, 100);
    switch (sync_char) {
    case '-':
	first_string[0] = 'n';
	first_string[1] = 'o';
	first_string[2] = 'n';
	first_string[3] = 'e';
	break;
    case 's':
	first_string[0] = 's';	
	break;
    case 'm':
	first_string[0] = 'm';	
	break;
    }
    return first_string;
}

char* get_yield_opts() {
    char* result = (char*)malloc(5*sizeof(char));
    if (result == NULL) {
	fprintf(stderr, "ERROR while trying to allocate memory\n\r");
	exit(1);
    }
    bzero(result, 5);
    int index = 0;
    if (opt_yield & INSERT_YIELD) {
	result[index] = 'i';
	index++;
    }
    if (opt_yield & DELETE_YIELD) {
	result[index] = 'd';
	index++;
    }
    if (opt_yield & LOOKUP_YIELD) {
	result[index] = 'l';
	index++;
    }
    if (index == 0) {
	result[0] = 'n';
	result[1] = 'o';
	result[2] = 'n';
	result[3] = 'e';
    }
    return result;
}

int main (int argc, char *argv[]) {
    static struct option long_options[] = {
	{"threads", required_argument, 0, 't'},
	{"iterations", required_argument, 0, 'i'},
	{"yield", required_argument, 0, 'y'},
	{"sync", required_argument, 0, 's'},
        {0, 0, 0, 0}
    };

    /* Read command-line arguments */
    char opt;
    unsigned int num_threads = 1;
    unsigned int num_iterations = 1;
    while ((opt = getopt_long(argc, argv, "y::s:t:i:", long_options, NULL))) {
        if (opt == -1) break;
        switch (opt) {
        case 't':
            if (optarg) num_threads = strtol(optarg, NULL, 10);
            break;
        case 'i':
            if (optarg) num_iterations = strtol(optarg, NULL, 10);
            break;
	case 'y':
	    opt_yield = 0x00;
	    for (size_t i = 0; i < strlen(optarg); i++) {		
		switch (optarg[i]) {
		case 'i':
		    opt_yield = 0x01 | opt_yield;
		    break;
		case 'd':		    
		    opt_yield = 0x02 | opt_yield;
		    break;
		case 'l':
		    opt_yield = 0x04 | opt_yield;
		    break;
		default:
		    fprintf(stderr, 
			    "./lab2_list: The `--yield' parameter accepts conditional arguments `i', `l', and `d'\n\r");
		    printUsage();
		    exit(1);
		}	    
	    }
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

    /* initialize empty list and populate */
    SortedList_t *head = (SortedList_t*)malloc(sizeof(SortedList_t));
    SortedList_t temp = {head, head, NULL};
    memcpy(head, &temp, sizeof(SortedList_t));
    
    SortedListElement_t *sle_arr = (SortedListElement_t*)malloc(sizeof(SortedListElement_t)*(num_threads*num_iterations));
    srand(time(NULL));
    for(long long i = 0; i < num_threads*num_iterations; i++) {
	char *temp_key = (char*)malloc(sizeof(char));
	*temp_key = (char)(rand() % 256);
	SortedListElement_t temp2 = {NULL, NULL, temp_key};
	memcpy(&sle_arr[i], &temp2, sizeof(SortedListElement_t));
    }

    /* create threads */
    struct thread_info *tinfo = calloc(num_threads,
				       sizeof(struct thread_info));
    if (tinfo == NULL) {
        fprintf(stderr, "./lab2_add: Call to malloc failed\r\n");
        exit(2);
    }
    char *yieldopts;
    yieldopts = get_yield_opts();
    struct timespec start_timespec;
    clock_gettime(CLOCK_REALTIME, &start_timespec);
    for (size_t tnum = 0; tnum < num_threads; tnum++) {
	tinfo[tnum].thread_num = tnum;
	tinfo[tnum].head = head;
	tinfo[tnum].sle_arr = sle_arr;
	tinfo[tnum].iterator = num_iterations;

	/* The pthread_create() call stores the thread ID into
           corresponding element of tinfo[] */

	int create_result = pthread_create(&(tinfo[tnum].thread_id),
					   NULL,
					   &thread_start,
					   (void*)(&tinfo[tnum]));
        if (create_result != 0)
            fprintf(stderr,
                    "./lab2_list: ERROR while creating new thread\n\r");
    }

    for (size_t tnum = 0; tnum < num_threads; tnum++) {
	int rc = pthread_join(tinfo[tnum].thread_id, NULL);
	if (rc) {
	    fprintf (stderr, "./lab2_list: ERROR while joining thread\n\r");
	    exit(2);
	}
    }

    struct timespec end_timespec;
    clock_gettime(CLOCK_REALTIME, &end_timespec);
    if (SortedList_length(head) != 0) {
	exit(2);
    }
    long total_time = 1000000000L*(end_timespec.tv_sec - start_timespec.tv_sec) + (end_timespec.tv_nsec - start_timespec.tv_nsec);
    char* first_string = get_sync();
    fprintf(stdout, "list-%s-%s,%d,%d,1,%d,%ld,%d\n", 
	    yieldopts, first_string, num_threads, num_iterations, 
	    3*num_threads*num_iterations, total_time,
	    (int)(total_time/(3.0*num_threads*num_iterations)));
    free(tinfo);
    exit(0);
}
