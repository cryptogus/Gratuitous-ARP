#ifndef __ETHHDR_H
#define __ETHHDR_H

#include <stdint.h>
typedef struct ethhdr_t{
    uint8_t  h_dest[6];  // Destination MAC address
    uint8_t  h_source[6];// Source MAC address
    uint16_t h_proto;    // Protocol type (ARP)
} EthHdr_t;


#endif