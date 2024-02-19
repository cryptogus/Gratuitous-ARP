#ifndef __ARPHDR_H
#define __ARPHDR_H
/* C standard library */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* System libraries */
#include <sys/socket.h>
#include <sys/ioctl.h>

/* Networking header files */
#include <netdb.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/if_ether.h> // For ETH_P_ARP
#include <netpacket/packet.h> // For struct sockaddr_ll

#include "ethhdr.h"
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

typedef struct arphdr_t {
    EthHdr_t frame;    // Ethernet header
    uint16_t htype;    // Hardware type (Ethernet)
    uint16_t ptype;    // Protocol type (IPv4)
    uint8_t  hlen;     // Hardware address length
    uint8_t  plen;     // Protocol address length
    uint16_t oper;     // Operation (ARP Request is 1, Reply is 2)
    uint8_t  smac[6];  // Sender hardware address (MAC address)
    uint32_t sip;      // Sender protocol address (IP address)
    uint8_t  tmac[6];  // Target hardware address (MAC address)
    uint32_t tip;      // Target protocol address (IP address)
} __attribute__((packed)) ArpHdr_t;

void sendGARP(const char *interface, const char *ipAddress);

#endif