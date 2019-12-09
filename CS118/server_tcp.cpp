#include <fcntl.h>
#include <math.h>
#include <sys/stat.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <pthread.h>
#include <chrono>
#include "rdt.h"

const int ACKED = 1;
uint64_t fr_trigger_ack;
const int NOT_ACKED = 0;
enum status_code {SLOW_START,
		  CONGESTION_AVOIDANCE,
		  FAST_RETRANSMIT,
		  FAST_RECOVERY};

status_code status = SLOW_START;
int portno, sockfd;
socklen_t clilen;
struct sockaddr_in serv_addr, cli_addr;
unsigned min_index = 0;
unsigned max_index = 1;   /* window size is in range [min, max) */
uint64_t send_base;
uint64_t original_seq_num;
size_t filesize;
uint16_t rwnd;
uint16_t ssthresh = 15360;
uint16_t cwnd = 1024;
int num_packets;
int *ack_count;
int *timer_active;
struct packet_hdr *headers;
std::chrono::time_point<std::chrono::steady_clock> *timers;
int file_fd;
int handshake_done = 0;
int finack_received = 0;
char *filename;

unsigned int CA_counter = 0;

pthread_mutex_t master_lock = PTHREAD_MUTEX_INITIALIZER;


int send_data(int index) { //send corresponding header + payload
    char holder[2 * MAX_PACKET_SIZE];
    bzero(holder, 2 * MAX_PACKET_SIZE);
    memcpy(holder, &(headers[index].seq_n), 8);
    memcpy(holder+8, &(headers[index].ack_n), 8);
    memcpy(holder+16, &(headers[index].rcv_window), 2);
    memcpy(holder+18, &(headers[index].ack_flag), 1);
    memcpy(holder+19, &(headers[index].syn_flag), 1);
    memcpy(holder+20, &(headers[index].fin_flag), 1);
  
    size_t rdsize;
    off_t offset;
    if (index == num_packets - 1) {
	rdsize = filesize - (num_packets - 1) * (MAX_PACKET_SIZE - sizeof(struct packet_hdr));
	offset = index * (MAX_PACKET_SIZE - sizeof(struct packet_hdr));
    }
    else {
	rdsize = MAX_PACKET_SIZE - sizeof(struct packet_hdr);
	offset = index * rdsize;
    }

    pread(file_fd, holder+21, rdsize, offset);
    size_t length = rdsize + 21;
    char *ptr = holder;
  
    int i = sendto(sockfd, ptr, length, 0, (struct sockaddr *) &cli_addr, clilen);
    if (i < 0) {
	fprintf(stderr, "ERROR in send_data\n");
	return -1;
    }

    return 0;
}

void print_send(unsigned seq_num, int retrans, int syn, int fin) {
    printf("Sending packet ");
    printf("%u %u", seq_num, (max_index - min_index)*1024);
    if (retrans == 1)
	printf(" Retransmission");
    if (syn == 1)
	printf(" SYN");
    if (fin == 1)
	printf(" FIN");
  
    printf("\n");
    fflush(stdout);
}


/* Called when the ACK is received, but before max_index 
   or min_index are changed */
void ACK_occured(int j) {
    if (j+1 == min_index && ack_count[j] >= 3 && status != FAST_RECOVERY) {
	status = FAST_RECOVERY;
	send_data(j+1);
	print_send(headers[j+1].seq_n, 1, 0, 0);
	ssthresh = (max_index - min_index) / 2;
	max_index = min_index + ssthresh + 3;
    } else if (status == SLOW_START && ack_count[j] == NOT_ACKED) {
	max_index++;
	if ((max_index - min_index) * 1024 > ssthresh) {
	    status = CONGESTION_AVOIDANCE;
	}
    } else if (status == CONGESTION_AVOIDANCE && ack_count[j] == NOT_ACKED) {
	CA_counter++;
	if (CA_counter >= max_index - min_index) {
	    CA_counter = 0;
	    max_index++;
	}
    } else if (status == FAST_RECOVERY) {
	if (j > fr_trigger_ack) {
	    status = CONGESTION_AVOIDANCE;
	    max_index = min_index + ssthresh + 1;
	} else {
	    max_index++;
	}
    }
}


void timeout_occured() {
    ssthresh = (max_index - min_index) * 1024/2;
    max_index = min_index + 1;    
    status = SLOW_START;
}

uint64_t generate_seq() {
    srand(time(NULL));
    return rand() % SEQ_GEN_MAX;
}

uint64_t expected_ack(int ind) {  //return the expected ack for given packet
    if (ind < num_packets - 1)
	return 1 + original_seq_num + (ind+1) * MAX_PACKET_SIZE;
    else
	return 1 + original_seq_num + filesize + num_packets * sizeof(struct packet_hdr);
}


void print_rcv(unsigned ack) {
    printf("Receiving packet %u\n", ack);
    fflush(stdout);
}


int send_hdr(struct packet_hdr *pkt) {
    char holder[32];
    bzero(holder, 32);
    memcpy(holder, &(pkt->seq_n), 8);
    memcpy(holder+8, &(pkt->ack_n), 8);
    memcpy(holder+16, &(pkt->rcv_window), 2);
    memcpy(holder+18, &(pkt->ack_flag), 1);
    memcpy(holder+19, &(pkt->syn_flag), 1);
    memcpy(holder+20, &(pkt->fin_flag), 1);
    int length = 21;
    char *ptr = holder;

    int i = sendto(sockfd, ptr, length, 0, (struct sockaddr *) &cli_addr, clilen);
    if (i < 0) {
	fprintf(stderr, "ERROR in send_hdr\n");
	return -1;
    }

    return 0;
}

void rcv_hdr(struct packet_hdr *pkt) {
    char holder[32];
    bzero(holder, 32);
    int n = recvfrom(sockfd, holder, 32, 0, (struct sockaddr *) &cli_addr, &clilen);
    if (n < 0) {
	fprintf(stderr, "Error receiving data\n");
	exit(1);
    }
    memcpy(&(pkt->seq_n), holder, 8);
    memcpy(&(pkt->ack_n), holder+8, 8);
    memcpy(&(pkt->rcv_window), holder+16, 2);
    memcpy(&(pkt->ack_flag), holder+18, 1);
    memcpy(&(pkt->syn_flag), holder+19, 1);
    memcpy(&(pkt->fin_flag), holder+20, 1);
}

/* this thread handles reception of ACK's */
void *ACKreader(void *arg) {  
    while (1) {
	struct packet_hdr pkt;
	memset(&pkt, 0, sizeof(struct packet_hdr));
    
	rcv_hdr(&pkt);
	if (pkt.ack_flag != 1)   /* ACK flag not set */
	    continue;
	int j;
	for (j = 0; j < num_packets; j++) {
	    if (expected_ack(j) == pkt.ack_n)
		break;
	}

        /* No corresponding packet */
	if (j >= num_packets) {
	    continue;
	}
	
	pthread_mutex_lock(&master_lock);
	if (j >= min_index) {
	    max_index+= (j+1 - min_index);
	    min_index = j+1;		
	}
	
	print_rcv(pkt.ack_n);	
	ACK_occured(j);
	ack_count[j] += ACKED;   /* count ACKs */
	

	if (min_index == num_packets) {
	    pthread_mutex_unlock(&master_lock);
	    return NULL;
	}
	pthread_mutex_unlock(&master_lock);
    }
}

void *handshake_ACK(void *arg) { //this thread reads client ACK during handshake
    while(1) {
	struct packet_hdr pkt;
	memset(&pkt, 0, sizeof(struct packet_hdr));
	char holder[MAX_PACKET_SIZE];
	bzero(holder, MAX_PACKET_SIZE);

	int n = recvfrom(sockfd, holder, MAX_PACKET_SIZE, 0, (struct sockaddr *) &cli_addr, &clilen);
	if (n < 0)
	    return NULL;
	memcpy(&(pkt.seq_n), holder, 8);
	memcpy(&(pkt.ack_n), holder+8, 8);
	memcpy(&(pkt.rcv_window), holder+16, 2);
	memcpy(&(pkt.ack_flag), holder+18, 1);
	memcpy(&(pkt.syn_flag), holder+19, 1);
	memcpy(&(pkt.fin_flag), holder+20, 1);

	if (pkt.ack_flag == 0 || pkt.ack_n != send_base)
	    continue;

	pthread_mutex_lock(&master_lock);
	print_rcv(pkt.ack_n);
	filename = (char *) malloc(sizeof(char) * n);
	bzero(filename, n);
	memcpy(filename, holder+21, n-21);
	handshake_done = 1;
	pthread_mutex_unlock(&master_lock);
	return NULL;
    }
}

void handshake(int fd, struct sockaddr_in *addr, socklen_t *addr_len) {
    struct packet_hdr pkt;
    memset(&pkt, 0, sizeof(struct packet_hdr));
  
    /* Read SYN */
    rcv_hdr(&pkt);
    if (pkt.syn_flag == 0)
	exit(0);
    int ret_ack = pkt.seq_n + 1;
    rwnd = pkt.rcv_window;
  
    /* Send SYNACK amd read ACK */
    send_base++;
    pthread_t t;
    pthread_create(&t, NULL, handshake_ACK, NULL);
    pthread_detach(t);

    std::chrono::time_point<std::chrono::steady_clock> timer;
    timer = std::chrono::steady_clock::now();
    int retrans = 0;
  
    while(1) {
    
	pthread_mutex_lock(&master_lock);
	if (handshake_done == 1) {
	    pthread_mutex_unlock(&master_lock);
	    break;
	}

	std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
	double d = std::chrono::duration_cast<std::chrono::milliseconds>(end - timer).count();
	if (retrans && d < TIMEOUT) {
	    pthread_mutex_unlock(&master_lock);
	    continue;
	}

	timer = std::chrono::steady_clock::now(); //timer reset
	memset(&pkt, 0, sizeof(struct packet_hdr));
	pkt.syn_flag = 1;
	pkt.seq_n = original_seq_num;
	pkt.ack_flag = 1;
	pkt.ack_n = ret_ack;

	if(send_hdr(&pkt) == -1)
	    exit(1);

	print_send(pkt.seq_n, retrans, 1, 0);
	pthread_mutex_unlock(&master_lock);

	retrans = 1;
    }
}

void create_headers() {
    num_packets = ceil(filesize / (double) (MAX_PACKET_SIZE - sizeof(struct packet_hdr)));
  
    ack_count = (int *) malloc(sizeof(int) * num_packets);
    timer_active = (int *) malloc(sizeof(int) * num_packets);
    headers = (struct packet_hdr *) malloc(sizeof(struct packet_hdr) * num_packets);
    timers = new std::chrono::time_point<std::chrono::steady_clock>[num_packets];
    for (int i = 0; i < num_packets; i++) {
	timer_active[i] = 0;
	ack_count[i] = NOT_ACKED;
    }
  
    for (int i = 0; i < num_packets; i++) { //Fill in each header
	headers[i].seq_n = send_base + i * MAX_PACKET_SIZE;
	headers[i].ack_flag = 0;
	headers[i].syn_flag = 0;
	headers[i].fin_flag = 0;
    }
}

void send_file() {
    file_fd = open(filename, O_RDONLY);
    struct packet_hdr pkt;
    memset(&pkt, 0, sizeof(struct packet_hdr));
  
    if (file_fd == -1) { //404 Not Found error TODO: change this to new version
	char msg[30] = "HTTP/1.1 404 Not Found \r\n\r\n";
	pkt.seq_n = send_base;
	if(send_hdr(&pkt) == -1)
	    exit(1);
	print_send(pkt.seq_n, 0, 0, 0);
	sendto(sockfd, msg, 26, 0, (struct sockaddr *) &cli_addr, clilen);
	return;
    }
  
    struct stat sb;
    stat(filename, &sb);
    filesize = sb.st_size;
    create_headers();
  
    pthread_t ack_thread;
    pthread_create(&ack_thread, NULL, ACKreader, NULL); //start ACK reader thread
    pthread_detach(ack_thread);
  
    int index = 0;
    while (1) {
	pthread_mutex_lock(&master_lock);
	int min = min_index;
	int max = max_index;

	if (min == num_packets) {
	    pthread_mutex_unlock(&master_lock);
	    break;
	}
    
	for (int i = min; i < max && i < num_packets; i++) {
	    if (ack_count[i] > 0)
		continue;

	    if (timer_active[i] == 0) {     //new transmission
		timers[i] = std::chrono::steady_clock::now();
		if (send_data(i) == -1)
		    exit(1);
		print_send(headers[i].seq_n, 0, 0, 0);
		timer_active[i] = 1;
	    } else {
		std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
		double t = std::chrono::duration_cast<std::chrono::milliseconds>(end - timers[i]).count();
		if (t < TIMEOUT) 
		    continue;
		timers[i] = std::chrono::steady_clock::now(); //reset clock
		if (send_data(i) == -1) {
		    exit(1);
		}
		
		timeout_occured();
		print_send(headers[i].seq_n, 1, 0, 0);
	    }
	}
    
	pthread_mutex_unlock(&master_lock);
	//optional usleep
    }

    delete [] timers;
    free(timer_active);
    free(headers);
    free(ack_count);
}

void *finack_reader(void *arg) { // this thread reads the finack
    while(1) {
	struct packet_hdr pkt;
	memset(&pkt, 0, sizeof(struct packet_hdr));
	char holder[MAX_PACKET_SIZE];
	bzero(holder, MAX_PACKET_SIZE);
    
	int n = recvfrom(sockfd, holder, MAX_PACKET_SIZE, 0, (struct sockaddr *) &cli_addr, &clilen);
	if (n < 0) 
	    return NULL;
    
	memcpy(&(pkt.seq_n), holder, 8);
	memcpy(&(pkt.ack_n), holder+8, 8);
	memcpy(&(pkt.rcv_window), holder+16, 2);
	memcpy(&(pkt.ack_flag), holder+18, 1);
	memcpy(&(pkt.syn_flag), holder+19, 1);
	memcpy(&(pkt.fin_flag), holder+20, 1);

	if (pkt.fin_flag != 1 || pkt.ack_flag != 1)
	    continue;

	pthread_mutex_lock(&master_lock);
	print_rcv(pkt.ack_n);
	finack_received = 1;
	pthread_mutex_unlock(&master_lock);
	return NULL;
    }
}

void fin() {
    struct packet_hdr pkt;
    pthread_t t;
    pthread_create(&t, NULL, finack_reader, NULL);
    pthread_detach(t);

    std::chrono::time_point<std::chrono::steady_clock> timer;
    timer = std::chrono::steady_clock::now();
    int retrans = 0;
    int count = 0;

    // send FIN and wait for FINACK, max 99 retransmissions
    while(1) {
	pthread_mutex_lock(&master_lock);
	if (finack_received == 1) {
	    pthread_mutex_unlock(&master_lock);
	    break;
	}

	if (count == 100)
	    exit(0);
    
	std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
	double d = std::chrono::duration_cast<std::chrono::milliseconds>(end - timer).count();
	if (retrans && d < TIMEOUT) {
	    pthread_mutex_unlock(&master_lock);
	    continue;
	}

	timer = std::chrono::steady_clock::now(); //timer reset
	memset(&pkt, 0, sizeof(struct packet_hdr));
	pkt.fin_flag = 1;
	pkt.seq_n = expected_ack(num_packets - 1);

	if(send_hdr(&pkt) == -1)
	    exit(1);
	count++;
	print_send(pkt.seq_n, retrans, 0, 1);
	pthread_mutex_unlock(&master_lock);

	retrans = 1;
    }
  
    // wait 2*TIMEOUT ms to ensure ACK reaches client
    char holder[32];
    bzero(holder, 32);
  
    timer = std::chrono::steady_clock::now();
    memset(&pkt, 0, sizeof(struct packet_hdr));
    pkt.ack_flag = 1;
    if(send_hdr(&pkt) == -1)
	exit(1);

    while(1) {
	std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
	double d = std::chrono::duration_cast<std::chrono::milliseconds>(end - timer).count();
	if (d >= 2 * TIMEOUT) {
	    printf("SERVER DONE\n");
	    exit(0);
	}
    
	int n = recvfrom(sockfd, holder, 32, MSG_DONTWAIT, (struct sockaddr *) &cli_addr, &clilen);
	if (n < 0)
	    continue;

	// Got something from client
	memcpy(&(pkt.seq_n), holder, 8);
	memcpy(&(pkt.ack_n), holder+8, 8);
	memcpy(&(pkt.rcv_window), holder+16, 2);
	memcpy(&(pkt.ack_flag), holder+18, 1);
	memcpy(&(pkt.syn_flag), holder+19, 1);
	memcpy(&(pkt.fin_flag), holder+20, 1);

	if (pkt.syn_flag != 1 || pkt.ack_flag != 1) //not a SYNACK
	    continue;

	memset(&pkt, 0, sizeof(struct packet_hdr));
	pkt.ack_flag = 1;
	pkt.seq_n = expected_ack(num_packets - 1);
	if(send_hdr(&pkt) == -1)
	    exit(1);
    }
}

int main(int argc, char **argv) {
    clilen = sizeof(cli_addr);

    if (argc != 2) {
	fprintf(stderr,"Please provide port number\n");
	exit(1);
    }
    else
	portno = atoi(argv[1]);

    if (portno == 0) {
	fprintf(stderr, "Please enter valid port number\n");
	exit(1);
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0); //UDP connection
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;          //Code of address family
    serv_addr.sin_addr.s_addr = INADDR_ANY;  //IP address of this machine

    serv_addr.sin_port = htons(portno);      //Port number in network byte order

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
	perror("Error on binding: ");
	exit(1);
    }

    send_base = generate_seq();
    original_seq_num = send_base;
  
    handshake(sockfd, &cli_addr, &clilen);
    send_file();
    free(filename);
    fin();
}
