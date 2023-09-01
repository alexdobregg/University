#pragma once
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>
#include <unordered_map>
#include <limits.h>
using namespace std;

#define BUFFLEN 1560
#define BUFFY sizeof(struct tcp_pack)

#define DIE(assertion, call_description)	\
	do {									\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",	\
					__FILE__, __LINE__);	\
			perror(call_description);		\
			exit(EXIT_FAILURE);				\
		}									\
	} while(0)

struct subscribers {
	struct subscriber **subscribers;
	int size;
	int capacity;
} __attribute__((packed));

struct subscriber {
	char ID[10];
	struct topic **topics;
	int top_size;
	int top_capacity;
	int socket;
	bool connected;
} __attribute__((packed));

struct topic {
	char topic[51];
	int sf;
	bool subscribed;
	struct tcp_pack **tcp_packets;
	int pack_size;
	int pack_capacity;
} __attribute__((packed));

struct tcp_pack {
	struct sockaddr_in udp;
	int bytes_to_send;
	char topic[51];
	uint8_t data_type;
	char payload[1501];
} __attribute__((packed));

struct udp_pack {
	char topic[50];
	uint8_t data_type;
	char payload[1500];
} __attribute__((packed));

struct sub_pack {
	char type[12];
	char topic[51];
	int sf;
} __attribute__((packed));