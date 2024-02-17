/* ARP header file */
#include "arphdr.h"

int main(int argc, char *argv[]) {
    sendGARP("eth0", "172.20.255.2");
}