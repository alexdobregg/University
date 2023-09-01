#include "server_helper.h"


void usage(char *file)
{
	fprintf(stderr, "Usage: %s server_port\n", file);
	exit(0);
}


int main(int argc, char** argv) {
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);
    // The way of starting a server is not used properly.
    if (argc < 2) {
		usage(argv[0]);
	}

    char buffer[BUFFLEN];
    int udp_socket, tcp_socket, new_socket, portNumber, fdmax, flag = 1;
    struct sockaddr_in udp_addr, tcp_addr, sub_addr;
    fd_set connected_fds, temp_fds;
    socklen_t sublen, udplen;

    // This way all the subscribers are stored, even if they are disconnected.
    struct subscribers *subscribers = (struct subscribers *) calloc(1, sizeof(struct subscribers));
    subscribers->size = 0;
    subscribers->capacity = 1;
    subscribers->subscribers = (struct subscriber **) calloc(subscribers->capacity, sizeof(struct subscriber *));

    // Creating the udp and tcp socket.
    DIE((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0,
        "Creating UDP socket failed.\n");
    DIE((tcp_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0,
        "Creating TCP socket failed.\n");

    // Verifying that the number of the given port is greater than 1024 because
    // 0-1023 are for the system.
    DIE((portNumber = atoi(argv[1])) < 1024,
        "Change the port number with one above 1024.\n");

    // Computing the necessary information about the udp and tcp addresses.
    memset((char *) &udp_addr, 0, sizeof(udp_addr));
    memset((char *) &tcp_addr, 0, sizeof(tcp_addr));
    udp_addr.sin_family = AF_INET;
    tcp_addr.sin_family = AF_INET;

    udp_addr.sin_port = htons(portNumber);
    tcp_addr.sin_port = htons(portNumber);

    udp_addr.sin_addr.s_addr = INADDR_ANY;
    tcp_addr.sin_addr.s_addr = INADDR_ANY;

    // Binding the sockets to the addresses.
    DIE(bind(udp_socket, (struct sockaddr *) &udp_addr, sizeof(sockaddr)) < 0,
        "Binding UDP socket failed.\n");
    DIE(bind(tcp_socket, (struct sockaddr *) &tcp_addr, sizeof(sockaddr)) < 0,
        "Binding TCP socket failed.\n");

    // Listening on the tcp socket.
    DIE(listen(tcp_socket, INT_MAX) < 0,
        "Listening on the TCP socket failed.\n");

    // Setting the file descriptors.
    FD_ZERO(&connected_fds);
    FD_ZERO(&temp_fds);
    FD_SET(udp_socket, &connected_fds);
    FD_SET(tcp_socket, &connected_fds);
    FD_SET(0, &connected_fds);

    fdmax = tcp_socket;

    while(1) {
        temp_fds = connected_fds;
        memset(buffer, 0, BUFFLEN);
        DIE(select(fdmax + 1, &temp_fds, NULL, NULL, NULL) < 0,
            "Selecting one of the fds failed.\n");

        if (FD_ISSET(0, &temp_fds)) {
            // The server receives a stdin command, that can only be exit.
			fgets(buffer, BUFFLEN - 1, stdin);
			if (strcmp(buffer, "exit\n") == 0) {
				break;
			} else {
                printf("Server only accepts the command `exit`.\n");
            }
        }

        // Going through the file descriptors to see from which socket a
        // message is received.
        for (int i = 1; i <= fdmax; i++) {
            if (FD_ISSET(i, &temp_fds)) {
                if (i == tcp_socket) {
                    // If the socket is a tcp socket, it means a new tcp
                    // client tries to connect to the server.
                    sublen = sizeof(sub_addr);
                    // Accepting the connection.
                    DIE((new_socket = accept(tcp_socket, (struct sockaddr *) &sub_addr, &sublen)) < 0,
                        "Accepting tcp client from tcp socket failed.\n");
                    // Deactivating Nagle.
                    DIE(setsockopt(new_socket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int)) < 0,
                        "Nagle deactivation failed.\n");
                    
                    // Adding the new_socket to the connected fds.
                    FD_SET(new_socket, &connected_fds);
                    // If it is greater than fdmax, fdmax will become the value of new_socket.
                    if (new_socket > fdmax) { 
				        fdmax = new_socket;
					}
                    // Receiving the ID of the client trying to connect to the server.
                    DIE(recv(new_socket, buffer, BUFFLEN, 0) < 0,
                        "Receiving ID failed.\n");

                    bool already_connected = false;
                    bool messages_sent = false;
                    // Going through the subscribers to see if ID is already connected or have previously connected to the server.
                    for (int j = 0; j < subscribers->size; j++) {
                        if (strcmp(subscribers->subscribers[j]->ID, buffer) == 0) {
                            // If the subscriber exists in the data base, it is checked to see if it is connected or not.
                            if (subscribers->subscribers[j]->connected) {
                                // If he's connected, the new_socket is removed from the connected fds and then closed.
                                already_connected = true;
                                printf("Client %s already connected.\n", buffer);
                                FD_CLR(new_socket, &connected_fds);
                                // If the fdmax was changed, it is changed back to the highest value that is set.
                                for (int j = fdmax; j > 2; --j) {
                                    if(FD_ISSET(j, &connected_fds)) {
                                        fdmax = j;
                                        break;
                                    }
                                }
                                close(new_socket);
                                break;
                            } else if (!subscribers->subscribers[j]->connected){
                                // If he's not connected, he is reconnected.
                                subscribers->subscribers[j]->connected = true;
                                printf("New client %s connected from %s:%d.\n",
                                        buffer, inet_ntoa(sub_addr.sin_addr), ntohs(sub_addr.sin_port));

                                // Changing the socket that he previously had to the new one.
                                subscribers->subscribers[j]->socket = new_socket;

                                // Going through his topics to see if there are any messages that need to be sent.
                                for (int k = 0; k < subscribers->subscribers[j]->top_size; k++) {
                                    // Checking to see if the topics sf is positive and if he's subscribed to the topic.
                                    if (subscribers->subscribers[j]->topics[k]->sf == 1 &&
                                                    subscribers->subscribers[j]->topics[k]->subscribed) {
                                        int size = subscribers->subscribers[j]->topics[k]->pack_size;
                                        // Going through the tcp packets stored while he was offline in order to send them to him.
                                        for (int l = 0; l < size; l++) {
                                            // // The packet is sent to him.
                                            int bytes = subscribers->subscribers[j]->topics[k]->tcp_packets[l]->bytes_to_send;
                                            DIE(send(subscribers->subscribers[j]->socket, &bytes, sizeof(bytes), 0) < 0,
                                                "Sending bytes failed");
                                            DIE(send(new_socket, (char *) subscribers->subscribers[j]->topics[k]->tcp_packets[l],bytes, 0) < 0,
                                                "Sending all the tcp messages stored failed.\n");

                                            // The memory is freed and the size is decreased.
                                            free(subscribers->subscribers[j]->topics[k]->tcp_packets[l]);
                                            subscribers->subscribers[j]->topics[k]->pack_size--;
                                        }
                                    }
                                }
                                messages_sent = true;
                                break;
                            }
                        }
                    }
                    if (messages_sent || already_connected) {
                        continue;
                    }
                    // If he was not disconnected nor previously connected, it means he is a new tcp client.
                    if (subscribers->capacity == subscribers->size) {
                        subscribers->capacity *= 2;
                        subscribers->subscribers = (struct subscriber **)
                                    realloc(subscribers->subscribers, subscribers->capacity * sizeof(struct subscriber *)); 
                    }
                    // Creating a new subscriber and adding the necessary information to his fields.
                    struct subscriber *subscriber = (struct subscriber *) calloc(1, sizeof(struct subscriber));

                    subscriber->top_size = 0;
                    subscriber->top_capacity = 1;
                    subscriber->topics = (struct topic **) calloc(subscriber->top_capacity, sizeof(struct topic *));

                    subscriber->connected = true;
                    strcpy(subscriber->ID, buffer);
                    subscriber->socket = new_socket;
                    subscribers->subscribers[subscribers->size++] = subscriber;

                    printf("New client %s connected from %s:%d.\n",
                                    buffer, inet_ntoa(sub_addr.sin_addr), ntohs(sub_addr.sin_port));

                } else if (i == udp_socket) {
                    memset(buffer, 0, BUFFLEN);
                    udplen = sizeof(udp_addr);
                    int bytes_received;
                    // Receiving the udp packet from the udp client.
                    DIE((bytes_received = recvfrom(udp_socket, buffer, BUFFLEN, 0, (sockaddr *) &udp_addr, &udplen)) < 0,
                        "Receiving from udp failed.\n");
                    struct udp_pack *udp_pack = (struct udp_pack *) buffer;
                    // Going through subsribers to see if any of them is
                    // subscribed to the topic of the message.
                    for (int j = 0; j < subscribers->size; j++) {
                        // Making a new tcp packet from the udp one in order
                        // to send it to the subscribers.
                        struct tcp_pack *tcp_pack = (struct tcp_pack *) calloc(1, sizeof(struct tcp_pack));

                        // Filling the fields of the tcp packet.
                        tcp_pack->udp = udp_addr;
                        strncpy(tcp_pack->topic, udp_pack->topic, 50);
                        tcp_pack->topic[50] = 0;

                        tcp_pack->data_type = udp_pack->data_type;
                        tcp_pack->bytes_to_send = 51 + 16 + 4 + 1;

                        memcpy(&tcp_pack->payload, &udp_pack->payload, 1500);

                        if (tcp_pack->data_type == 0) {
                            tcp_pack->bytes_to_send += 5;
                        } else if (tcp_pack->data_type == 1) {
                            tcp_pack->bytes_to_send += 2;
                        } else if (tcp_pack->data_type == 2) {
                            tcp_pack->bytes_to_send += 6;
                        } else {
                            tcp_pack->bytes_to_send += strlen(tcp_pack->payload) + 1;
                        }
                        
                        bool found_topic = false;
                        // Going through the topics of every subscriber in
                        // order to send them a message if they have a
                        // subscription to the topic related to the message.
                        for (int k = 0; k < subscribers->subscribers[j]->top_size; k++) {
                            if (strcmp(tcp_pack->topic, subscribers->subscribers[j]->topics[k]->topic) == 0) {
                                found_topic = true;
                                if (subscribers->subscribers[j]->connected && subscribers->subscribers[j]->topics[k]->subscribed) {
                                    // If he is both connected and subscribed to
                                    // the topic, then he will receive the message.
                                    int bytes = tcp_pack->bytes_to_send;
                                    DIE(send(subscribers->subscribers[j]->socket, &bytes, sizeof(bytes), 0) < 0,
                                        "Sending bytes failed");

                                    DIE(send(subscribers->subscribers[j]->socket, (char *) tcp_pack, bytes, 0) < 0,
                                        "Sending tcp message failed.\n");
                                    free(tcp_pack);
                                } else if (!subscribers->subscribers[j]->connected &&
                                                subscribers->subscribers[j]->topics[k]->subscribed) {
                                    // If he is not connected but he is subscribed
                                    // to the topic and has his store-and-forward on,
                                    // the message is stored.
                                    if (subscribers->subscribers[j]->topics[k]->sf == 1) {
                                        if (subscribers->subscribers[j]->topics[k]->pack_size ==
                                                        subscribers->subscribers[j]->topics[k]->pack_capacity) {
                                            subscribers->subscribers[j]->topics[k]->pack_capacity *= 2;
                                            subscribers->subscribers[j]->topics[k]->tcp_packets =
                                                    (struct tcp_pack **) realloc(subscribers->subscribers[j]->topics[k]->tcp_packets,
                                                    subscribers->subscribers[j]->topics[k]->pack_capacity * sizeof(struct tcp_pack *));
                                        }
                                        subscribers->subscribers[j]->topics[k]->
                                                    tcp_packets[subscribers->subscribers[j]->topics[k]->pack_size++] = tcp_pack;
                                    }
                                }
                                break;
                            }
                        }
                        if (!found_topic) {
                            free(tcp_pack);
                        }
                    }
                } else {
                    // Receiving a subscriber packet: exit, subscribe or unsubscribe.
                    memset(buffer, 0, sizeof(struct sub_pack));
                    int closed;
                    DIE((closed = recv(i, buffer, sizeof(struct sub_pack), 0)) < 0,
                        "Receiving packet from subscriber failed.\n");
                    if (closed == 0) {
                        for (int j = 0; j < subscribers->size; j++) {
                            if (subscribers->subscribers[j]->socket == i) {
                                // Disconnecting the subscriber.
                                printf("Client %s disconnected.\n", subscribers->subscribers[j]->ID);
                                subscribers->subscribers[j]->connected = false;
                                break;
                            }
                        }
                        FD_CLR(i, &connected_fds);
                        for (int j = fdmax; j > 2; --j) {
                            if(FD_ISSET(j, &connected_fds)) {
                                fdmax = j;
                                break;
                            }
                        }
                        close(i);
                    } else {
                        // This is a packet different than exit. It may be subscribe or unsubscribe.
                        struct sub_pack *sub_pack = (struct sub_pack *) buffer;
                        for (int j = 0; j < subscribers->size; j++) {
                            if (subscribers->subscribers[j]->socket == i) {
                                if (strcmp(sub_pack->type, "subscribe") == 0) {
                                    // If the packet is a subscribe one, then the
                                    // topic is created and added to the subscriber topics.
                                    if (subscribers->subscribers[j]->top_size == subscribers->subscribers[j]->top_capacity) {
                                        subscribers->subscribers[j]->top_capacity *= 2;
                                        subscribers->subscribers[j]->topics = (struct topic **) realloc(subscribers->subscribers[j]->topics,
                                                                    subscribers->subscribers[j]->top_capacity * sizeof(struct topic *));
                                    }
                                    struct topic *topic = (struct topic *) calloc(1, sizeof(struct topic));

                                    strcpy(topic->topic, sub_pack->topic);
                                    topic->sf = sub_pack->sf;
                                    topic->subscribed = true;
                                    topic->pack_size = 0;
                                    topic->pack_capacity = 1;
                                    topic->tcp_packets = (struct tcp_pack **) calloc(topic->pack_capacity, sizeof(struct tcp_pack *));
                                    
                                    subscribers->subscribers[j]->topics[subscribers->subscribers[j]->top_size++] = topic;
                                    break;
                                } else if (strcmp(sub_pack->type, "unsubscribe") == 0) {
                                    // If the packet is an unsubscribe one, then
                                    // the subscribed option is turned off for the
                                    // topic.
                                    for (int k = 0; k < subscribers->subscribers[j]->top_size; k++) {
                                        if (strcmp(sub_pack->topic, subscribers->subscribers[j]->topics[k]->topic) == 0) {
                                            subscribers->subscribers[j]->topics[k]->subscribed = false;
                                            break;
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // Freeing the used memory.
    for (int i = 0; i < subscribers->size; i++) {
        for (int j = 0; j < subscribers->subscribers[i]->top_size; j++) {
            for (int k = 0; k < subscribers->subscribers[i]->topics[j]->pack_size; k++) {
                free(subscribers->subscribers[i]->topics[j]->tcp_packets[k]);
            }
            free(subscribers->subscribers[i]->topics[j]->tcp_packets);
            free(subscribers->subscribers[i]->topics[j]);
        }
        free(subscribers->subscribers[i]->topics);
        free(subscribers->subscribers[i]);
    }
    free(subscribers->subscribers);
    free(subscribers);
    for (int i = 3; i <= fdmax; i++) {
        if (FD_ISSET(i, &connected_fds)) {
            close(i);
        }
    }
    return 0;
}