#include "server_helper.h"

void usage(char *file)
{
	fprintf(stderr, "Usage: %s ID server_address server_port\n", file);
	exit(0);
}

void sending_subMessage(char *buffer, int sockfd) {
	// Creating a sub_pack to store the command, topic and sf.
	struct sub_pack *sub_pack = (struct sub_pack *) calloc(1, sizeof(struct sub_pack));

	strcpy(sub_pack->type, "subscribe");

	char *token = strtok(buffer, " ");
	token = strtok(NULL, " ");
	strcpy(sub_pack->topic, token);

	token = strtok(NULL, " ");
	sub_pack->sf = atoi(token);

	// Sending the packet to the server.
	DIE(send(sockfd, (char *) sub_pack, sizeof(struct sub_pack), 0) < 0,
		"Sending subscribe packet failed.\n");

	free(sub_pack);
}

void sending_unsubMessage(char *buffer, int sockfd) {
	// Creating a sub_pack to store the command and topic.
	struct sub_pack *sub_pack = (struct sub_pack *) calloc(1, sizeof(struct sub_pack));

	strcpy(sub_pack->type, "unsubscribe");

	char *token = strtok(buffer, " ");
	token = strtok(NULL, "\n");
	strcpy(sub_pack->topic, token);
	
	// Sending the packet to the server.
	DIE(send(sockfd, (char *) sub_pack, sizeof(struct sub_pack), 0) < 0,
		"Sending unsubscribe packet failed.\n");

	free(sub_pack);
}

void compute_and_print(struct tcp_pack *tcp_pack) {
	char payload[1501], data_type[12];
	long long number_int;
	double number_real;
	// Computing the payload and the data type from the packet.
	if (tcp_pack->data_type == 0) {
		sprintf(data_type, "INT");
		number_int = ntohl(*(uint32_t*)(tcp_pack->payload + 1));
		if (tcp_pack->payload[0]) {
			number_int *= -1;
		}
		sprintf(payload, "%lld", number_int);
	} else if (tcp_pack->data_type == 1) {
		sprintf(data_type, "SHORT_REAL");
		number_real = ntohs(*(uint16_t*)(tcp_pack->payload));
		number_real /= 100;
		sprintf(payload, "%.2f", number_real);
	} else if (tcp_pack->data_type == 2) {
		sprintf(data_type, "FLOAT");
		number_real = ntohl(*(uint32_t*)(tcp_pack->payload + 1));
		number_real /= pow(10, tcp_pack->payload[5]);
		if (tcp_pack->payload[0]) {
			number_real *= -1;
		}
		sprintf(payload, "%lf", number_real);
	} else {
		sprintf(data_type, "STRING");
		strcpy(payload, tcp_pack->payload);
	}

	printf("%s:%hu - %s - %s - %s\n", inet_ntoa(tcp_pack->udp.sin_addr), ntohs(tcp_pack->udp.sin_port),
					tcp_pack->topic, data_type, payload);

}

int main(int argc, char *argv[]) {
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);
	// The way of starting a subscriber is not used properly.
	if (argc < 4) {
		usage(argv[0]);
	}
	char buffer[BUFFLEN];
	int sockfd, fdmax, portNumber, flag = 1;
	struct sockaddr_in server_address;
	fd_set read_fds, tmp_fds;
	// Creating the socket.
	DIE((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0,
		"Creating socket failed.\n");

	// Verifying that the number of the given port is greater than 1024 because
	// 0-1023 are for the system.
    DIE((portNumber = atoi(argv[3])) < 1024,
		"Change the port number with one above 1024. It needs to be the same as the port of server.\n");

	// Computing the necessary information about the server address.
	memset((char *) &server_address, 0, sizeof(sockaddr_in));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(portNumber);
	DIE(inet_aton(argv[2], &server_address.sin_addr) == 0,
		"Converting the Internet host address failed.\n");

	// Connecting the socket to the server address.
	DIE(connect(sockfd, (struct sockaddr*) &server_address, sizeof(server_address)) < 0,
		"Connecting to the server address failed.\n");

	// Setting the file descriptors.
	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);
	FD_SET(sockfd, &read_fds);
	FD_SET(0, &read_fds);

	fdmax = sockfd;

	// Sending the ID of the client to the server.
    DIE(send(sockfd, argv[1], strlen(argv[1])+ 1, 0) < 0,
		"Sending client ID failed.\n");

	// Deactivating Nagle algorithm.
    DIE(setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int)) < 0,
		"Nagle deactivation failed.\n");

	while (1) {
		tmp_fds = read_fds;

		DIE(select(fdmax + 1, &tmp_fds, NULL, NULL, NULL) < 0,
			"Selecting one of the fds failed.\n");

		if (FD_ISSET(0, &tmp_fds)) {
			// The server receives a stdin command, which can be one from the
			// commands: exit, subscribe topic_name sf, unsubscribe topic_name.
			memset(buffer, 0, BUFFLEN);
			fgets(buffer, BUFFLEN - 1, stdin);

			if (strcmp(buffer, "exit\n") == 0) {
				// Received an exit command.
				break;
			} else if (strncmp(buffer, "subscribe", 9) == 0) {
				// Received a subscribe command.
				sending_subMessage(buffer, sockfd);
				printf("Subscribed to topic.\n");
			} else if (strncmp(buffer, "unsubscribe", 11) == 0) {
				// Received an unsubscribe command.
				sending_unsubMessage(buffer, sockfd);
				printf("Unsubscribed from topic.\n");
			}
		} else{
			memset(buffer, 0, BUFFLEN);
			int ret, bytes_to_recv, bytes_received = 0;
			DIE((ret = recv(sockfd, &bytes_to_recv, sizeof(int), 0)) < 0,
				"Receiving packet from server failed.");
			if (ret == 0) {
                // The server closed the connection to the client.
                break;
            }
			// Knowing the number of bytes, information is received until all of the bytes are received.
			char pack_recv[bytes_to_recv];
			memset(pack_recv, 0, bytes_to_recv);
			while (1) {
				if (bytes_received >= bytes_to_recv) {
					break;
				}
				DIE((ret = recv(sockfd, &pack_recv + bytes_received, bytes_to_recv, 0)) < 0, "Receiving all the bytes failed.\n");
				bytes_received += ret;
			}
			// The message is computed and printed.
			struct tcp_pack *tcp_pack = (struct tcp_pack *)pack_recv;
			compute_and_print(tcp_pack);
		}
	}
	close(sockfd);
	return 0;
}