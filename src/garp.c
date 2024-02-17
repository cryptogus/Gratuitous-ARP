#include "arphdr.h"
/**
 * Function used for printing the raw hexdecimal packet data
 */
void
print_raw_packet (ArpHdr_t *packet, unsigned int packet_len)
{
    char buffer[packet_len];
    memcpy(buffer, packet, packet_len);

    for(int i = 0; i < packet_len; i++) {

        /* Breaks line for each 32 bits size */
        if(!(i % 4)) {
            fprintf(stdout, "\n");
        }

        fprintf(stdout, "%.2X ", buffer[i] & 0xff);
    }
    fprintf(stdout, "\n");
}

/**
 * Create GARP packet
*/
void sendGARP(const char *interface, const char *ipAddress) {
    ArpHdr_t garp;
    
    /* Create Raw socket */
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (sock == -1) {
        perror("Error opening socket");
        exit(EXIT_FAILURE);
    }
    /* Get Network Informations */
    struct ifreq ifr;
    strncpy(ifr.ifr_name, interface, IFNAMSIZ); // Sets interface name
    if (ioctl(sock, SIOCGIFHWADDR, &ifr) == -1) { //  Gets the MAC address and the index of the given interface using ioctl
        perror("Error getting MAC address");
        close(sock);
        exit(EXIT_FAILURE);
    }
    
    /* Destination MAC address (broadcast) */
    memset(garp.frame.h_dest, 0xFF, 6);
    /* Source MAC address */
    memcpy(garp.frame.h_source, ifr.ifr_hwaddr.sa_data, 6);
    /* EtherType is ARP */
    garp.frame.h_proto = htons(ETH_P_ARP);

    /* Hardware type */
    garp.htype = htons(ETHER);
    /* Protocol type */
    garp.ptype = htons(0x0800);
    /* Hardware length */
    garp.hlen = 0x06;
    /* Protocol length */
    garp.plen = 0x04;
    /* Operation: ARP Request */
    garp.oper = htons(0x0001);
    /* Get IPv4 address for */
    struct in_addr sourceIP;
    sourceIP.s_addr = inet_addr(ipAddress);
    /* Sender MAC and IP address */
    memcpy(garp.smac, ifr.ifr_hwaddr.sa_data, 6);
    memcpy(&garp.sip, &sourceIP, sizeof(uint32_t));
    /* Target MAC and IP address */
    memset(garp.tmac, 0xFF, 6);
    memcpy(&garp.tip, &sourceIP, sizeof(uint32_t));

     /* Link layer address data */
    struct sockaddr_ll sa;
    memset(&sa, 0, sizeof(struct sockaddr_ll));
    sa.sll_family = AF_PACKET;
    sa.sll_protocol = htons(ETH_P_ARP);
    sa.sll_ifindex = if_nametoindex(interface); // → netinet/in.h
    // print_raw_packet(&garp, sizeof(garp));
    /* Send ARP packet */
    if (sendto(sock, &garp, sizeof(garp), 0, (struct sockaddr*)&sa, sizeof(struct sockaddr_ll)) == -1) {
        perror("Error sending ARP packet");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Gratuitous ARP sent successfully.\n");

    close(sock);
}