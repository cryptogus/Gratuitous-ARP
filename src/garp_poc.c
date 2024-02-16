#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <sys/socket.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>  // For ETH_P_ARP
#include <netpacket/packet.h>  // For struct sockaddr_ll

#include <stdint.h>

// HardwareType (HTYPE)
#define	ETHER 1    // Ethernet 10Mbps
#define	EETHER 2   // Experimental Ethernet 3Mbps
#define	AX25 3     // Amateur Radio AX.25
#define	PRONET 4   // PROnet token ring
#define	CHAOS 5    // Chaosnet
#define IEEE802 6  // IEEE 802.2 Ethernet/TR/TB
#define ARCNET 7   // ARCnet
#define	HC 8       // Hyperchannel
#define	LANSTAR 9  // Lanstar

void sendGratuitousARP(const char *interface, const char *ipAddress) {
    /* Raw socket */
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (sock == -1) {
        perror("Error opening socket");
        exit(EXIT_FAILURE);
    }

    struct ifreq ifr;
    strncpy(ifr.ifr_name, interface, IFNAMSIZ);
    if (ioctl(sock, SIOCGIFHWADDR, &ifr) == -1) {
        perror("Error getting MAC address");
        close(sock);
        exit(EXIT_FAILURE);
    }

/* Create ARP packet */
    char arpPacket[42];
/* Ethernet header */
    memset(arpPacket, 0xFF, 6);                        // Destination MAC address (broadcast)
    memcpy(arpPacket + 6, ifr.ifr_hwaddr.sa_data, 6);  // Source MAC address
    // EtherType is ARP
    arpPacket[12] = 0x08;
    arpPacket[13] = 0x06;
/* ARP header */
    // Hardware type is Ethernet 10Mbps
    arpPacket[14] = 0x00; 
    arpPacket[15] = ETHER;
    // Protocol type is IPv4
    arpPacket[16] = 0x08;
    arpPacket[17] = 0x00;
    // Hardware length (Ethernet address length is 6)
    arpPacket[18] = 0x06;
    // Protocol length (IPv4 address length is 4)
    arpPacket[19] = 0x04;
    // Operation (Specifies the operation that the sender is performing: 1 for request, 2 for reply)
    arpPacket[20] = 0x00;
    arpPacket[21] = 0x01;

    // Sender MAC and IP address
    memcpy(arpPacket + 22, ifr.ifr_hwaddr.sa_data, 6);  // Sender MAC address
    struct in_addr sourceIP;
    if (inet_pton(AF_INET, ipAddress, &sourceIP) != 1) {
        perror("Invalid IP address");
        close(sock);
        exit(EXIT_FAILURE);
    }
    memcpy(arpPacket + 28, &sourceIP, 4);                       // Sender IP address

    // Target MAC and IP address
    memset(arpPacket + 32, 0xFF, 6);  // Target MAC address
    memcpy(arpPacket + 38, &sourceIP, 4);  // Target IP address

    struct sockaddr_ll sa;
    memset(&sa, 0, sizeof(struct sockaddr_ll));
    sa.sll_family = AF_PACKET;
    sa.sll_protocol = htons(ETH_P_ARP);
    sa.sll_ifindex = if_nametoindex("eth0");  // Replace with your network interface
    // Send ARP packet
    if (sendto(sock, arpPacket, sizeof(arpPacket), 0, (struct sockaddr*)&sa, sizeof(struct sockaddr_ll)) == -1) {
        perror("Error sending ARP packet");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Gratuitous ARP sent successfully.\n");

    close(sock);
}

int main() {
    const char *interface = "eth0";          // My network interface
    const char *ipAddress = "172.29.98.119";    // My IP address
    //const char *targetMacAddress = "FF:FF:FF:FF:FF:FF"; 

    sendGratuitousARP(interface, ipAddress);

    return 0;
}