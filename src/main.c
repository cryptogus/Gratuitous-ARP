/* ARP header file */
#include "arphdr.h"

void uasge(void) {
    printf("Usage: sned_garp <interface> <ipAddress>\n");
    printf("Sample: sned_garp eth0 192.168.7.192\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        uasge();
        return 3;
    }
    sendGARP(argv[1], argv[2]);

    return 0;
}