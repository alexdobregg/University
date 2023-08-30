#include <queue.h>
#include "skel.h"

int main(int argc, char *argv[])
{	
	packet m;
	int rc;

	init(argc - 2, argv + 2);
	// Reading the route table entries from the given file and obtaining its length.
	struct route_table_entry **rtable;
	int rtableL = read_rtable(&rtable, argv[1]);

	// Sorting the route table ascending by prefixes.
	qsort(rtable, rtableL, sizeof(struct route_table_entry *), cmp_quick);

	// Creating a queue to store the packets.
	queue queue = queue_create();


	// Allocating the arp table.
	struct arp_table *arp_table = (struct arp_table *) malloc(sizeof(struct arp_table));
	// Becase we don't know how many entries we will have, we allocate only 1 for the vector of entries.
	arp_table->arp_entries = (struct arp_table_entry **) malloc(sizeof(struct arp_table_entry *));
	// Size is set to 0, capacity to 1.
	arp_table->size = 0;
	arp_table->capacity = 1;


	while (1) {
		rc = get_packet(&m);
		DIE(rc < 0, "get_message");
		// Getting the ip_hdr and the eth_hdr from the payload.
		struct iphdr *ip_hdr = (struct iphdr *) (m.payload + sizeof(struct ether_header));
		struct ether_header *eth_hdr = (struct ether_header*)m.payload;
		
		// Trying to get the icmp_hdr from the payload.
		struct icmphdr *icmp_hdr = parse_icmp(m.payload);

		// If it is not null, it means we have an icmp. We check to see if it is for the router
		// and if it is an echo request, so we can send an icmp. Then we continue.
		if (icmp_hdr != NULL) {
			if (inet_addr(get_interface_ip(m.interface)) == ip_hdr->daddr) {
				if (icmp_hdr->type == ICMP_ECHO) {
					send_icmp(ip_hdr->saddr, ip_hdr->daddr, eth_hdr->ether_dhost, eth_hdr->ether_shost,
					 		  ICMP_ECHOREPLY, 0, m.interface, icmp_hdr->un.echo.id, icmp_hdr->un.echo.sequence);
					continue;
				}
			}
		} 

		// Trying to get the arp_hdr from the payload.
		struct arp_header *arp_hdr = parse_arp(m.payload);

		// If it is not null, it means we have an arp.
		if (arp_hdr != NULL) {
			// We check to see if the target is the router and the type of arp is request.
			// If it is, then we send an arp reply with the right mac address and continue.
			if ((inet_addr(get_interface_ip(m.interface)) == arp_hdr->tpa) &&
									 (ntohs(arp_hdr->op) == ARPOP_REQUEST)) {
				memcpy(eth_hdr->ether_dhost, eth_hdr->ether_shost, ETH_ALEN);
				get_interface_mac(m.interface, eth_hdr->ether_shost);
				send_arp(arp_hdr->spa, arp_hdr->tpa, eth_hdr, m.interface, htons(ARPOP_REPLY));
				continue;
			} else if (ntohs(arp_hdr->op) == ARPOP_REPLY) {
				// If the type of arp is a reply one, we check to see if we have a package in
				// the queue or not. If the queue is empty, we continue.
				if (queue_empty(queue)) {
					continue;
				}
				// We check to see if we need to reallocate the memory of the vector.
				if(arp_table->size == arp_table->capacity) {
					arp_table->arp_entries = (struct arp_table_entry **) realloc(arp_table->arp_entries,
					 					2 * arp_table->capacity * sizeof(struct arp_table_entry *)); 
				}

				// Completing the arp_table with the source address for the ip and the source
				// hardware address for the mac address, then incrementing the length.
				struct arp_table_entry *arp_entry = (struct arp_table_entry *) malloc(sizeof(struct arp_table_entry));
				arp_entry->ip = arp_hdr->spa;
				memcpy(arp_entry->macAddress, arp_hdr->sha, ETH_ALEN);
				arp_table->arp_entries[arp_table->size] = arp_entry;
				arp_table->size++;


				// We need to send the packet from the queue.
				packet *pack = (packet *) queue_deq(queue);


				// Obtaining the new ip_hdr and eth_hdr, related to the new payload.
				ip_hdr = (struct iphdr *) (pack->payload + sizeof(struct ether_header));

				eth_hdr = (struct ether_header*)pack->payload;

				// We copy the source hardware address in the destination host address and
				// the target hardware address in the source host address.
				memcpy(eth_hdr->ether_dhost, arp_hdr->sha, ETH_ALEN);	
				memcpy(eth_hdr->ether_shost, arp_hdr->tha, ETH_ALEN);
				eth_hdr->ether_type = htons(ETHERTYPE_IP);

				// We send the packet, then we continue.
				send_packet(pack->interface, pack);
				continue;

			}
		}

		// If the ttl drops below 1, then we have to send an icmp error with the type
		// ICMP_TIME_EXCEEDED and continue.
		if (ip_hdr->ttl <= 1) {
			send_icmp_error(ip_hdr->saddr, ip_hdr->daddr, eth_hdr->ether_dhost, eth_hdr->ether_shost,
					 		  ICMP_TIME_EXCEEDED, 0, m.interface);
			continue;
		}

		// If the checksum is wrong, then we continue.
		if(ip_checksum(ip_hdr, sizeof(struct iphdr)) != 0) {
			continue;
		}

		// Decrementing the ttl and recalculating the checksum.
		ip_hdr->ttl--;
		ip_hdr->check = 0;
		ip_hdr->check = ip_checksum(ip_hdr, sizeof(struct iphdr));

		// Trying to get the best route for our package based on the destination address.
		struct route_table_entry *best_route = (struct route_table_entry *) malloc(sizeof(struct route_table_entry));
		best_route = get_best_route(rtable, rtableL, ip_hdr->daddr);

		// If the route is null, then we need to send an icmp error with the type
		// ICMP_DEST_UNREACH.
		if (best_route == NULL) {
			send_icmp_error(ip_hdr->saddr, ip_hdr->daddr, eth_hdr->ether_dhost, eth_hdr->ether_shost,
					 		  ICMP_DEST_UNREACH, 0, m.interface);
			continue;
		}

		// Trying to get the arp entry from the arp table based on our next hop.
		// struct arp_table_entry *arp_entry = get_arp_entry(arp_table, arpL, best_route->next_hop);
		struct arp_table_entry *arp_entry = get_arp_entry(arp_table->arp_entries, arp_table->size, best_route->next_hop);

		if (arp_entry != NULL) {
			// If we have an entry, then we need to send the packet with the best route's interface.
			get_interface_mac(best_route->interface, eth_hdr->ether_shost);
			memcpy(eth_hdr->ether_dhost, arp_entry->macAddress, ETH_ALEN);
			send_packet(best_route->interface, &m);

		} else {
			// If we don't have an entry, we need to create one.
			get_interface_mac(best_route->interface, eth_hdr->ether_shost);
			for (int i = 0; i < 6; i++) {
				eth_hdr->ether_dhost[i] = 0xFF;
			}
			// We use an aux packet to put in the queue. This is the one that we
			// will send when the arp entry is created.
			packet aux_m;
			aux_m.interface = best_route->interface;
			aux_m.len = m.len;
			memcpy(aux_m.payload, m.payload, sizeof(m.payload));
			// We do not set the interface of the aux packet to the one of m, so
			// when we dequeue our packet, we will have the best route's interface.
			queue_enq(queue, &aux_m);
			eth_hdr->ether_type = htons(ETHERTYPE_ARP);
			// Sending the arp with a request.
			send_arp(best_route->next_hop, inet_addr(get_interface_ip(best_route->interface)), eth_hdr, best_route->interface, htons(ARPOP_REQUEST));
		}
	}
	free_rtable(rtable);
	free_atable(arp_table);
	free(queue);
}
