#include "arphdr.h"
#include <syslog.h>
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

    openlog("send-garp", LOG_CONS, LOG_USER);

    /* Create Raw socket */
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    while(1) {
        if (sock == -1) {
            syslog(LOG_ERR, "Error opening socket");
            sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
        }else {
            break;
        }
    }
    /* Get Network Informations */
    struct ifreq ifr;
    char interface_backup[IFNAMSIZ];
    uint8_t backup = 0;
    strncpy(ifr.ifr_name, interface, IFNAMSIZ); // Sets interface name
    while(1) {
        if (ioctl(sock, SIOCGIFHWADDR, &ifr) == -1) { //  Gets the MAC address and the index of the given interface using ioctl
            syslog(LOG_ERR, "Error getting MAC address");
            /* Implemented to enter an existing interface instead of an invalid interface */
            fgets(interface_backup, IFNAMSIZ, stdin);
            interface_backup[strlen(interface_backup) - 1] = '\0'; // Remove newline characters
            strncpy(ifr.ifr_name, interface_backup, IFNAMSIZ);
            backup = 1;
        }else {
            break;
        }
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
    /* https://wiki.wireshark.org/Gratuitous_ARP - Discussion */
    // memset(garp.tmac, 0xFF, 6); // In wireshark, Gratuitous ARP 로 패킷 캡처 된다.
    memset(garp.tmac, 0x00, 6); // In wireshark, ARP Anouncement 로 패킷 캡처 된다.
    memcpy(&garp.tip, &sourceIP, sizeof(uint32_t));

     /* Link layer address data */
    struct sockaddr_ll sa;
    memset(&sa, 0, sizeof(struct sockaddr_ll));
    sa.sll_family = AF_PACKET;
    sa.sll_protocol = htons(ETH_P_ARP);
    if (backup)
        sa.sll_ifindex = if_nametoindex(interface_backup);
    else
        sa.sll_ifindex = if_nametoindex(interface); // → netinet/in.h
    // print_raw_packet(&garp, sizeof(garp));
    /* Send ARP packet */
    int reval = sendto(sock, &garp, sizeof(garp), 0, (struct sockaddr*)&sa, sizeof(struct sockaddr_ll));
    while(1) {
        if (reval == -1) {
            syslog(LOG_ERR, "Error sending ARP packet");
            reval = sendto(sock, &garp, sizeof(garp), 0, (struct sockaddr*)&sa, sizeof(struct sockaddr_ll));
        }else {
            break;
        }
    }

    syslog(LOG_USER, "Gratuitous ARP sent successfully.");

    closelog();
    close(sock);
}