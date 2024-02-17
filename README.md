# Gratuitous-ARP (GARP)
Gratuitous ARP (Address Resolution Protocol) is an ARP packet that is sent by a device in a network to announce or update its own IP-to-MAC address mapping to other devices in the same network. Unlike a regular ARP request or response, gratuitous ARP packets are not prompted by an ARP request for the sender's IP address. Instead, they are voluntarily broadcasted to the entire network.

The primary purposes of Gratuitous ARP include:

1. **IP Address Conflict Detection:** A device may send a gratuitous ARP to check if another device is already using its IP address. If it receives a response, it indicates an IP address conflict.

2. **MAC Address Update:** When a device's MAC address changes (e.g., due to hardware replacement or configuration change), it can use gratuitous ARP to inform other devices of the new MAC address associated with its IP address.

3. **Network Initialization:** Gratuitous ARP can be used during network initialization to announce the presence of a device and update ARP caches in other devices.

A Gratuitous ARP packet has the same format as a regular ARP packet but may have some fields, such as the source and destination MAC addresses, set to specific values or left blank. The sender's IP and MAC addresses are typically set, and the target IP and MAC addresses may be left blank or set to broadcast values.
## Implementation
There are various ways to implement GARP, such as using rawsocket or using the pcap library.  
I used rawsocket for my implementation.

## Gratuitous ARP TEST
My Network information in WSL2
```bash
$ ifconfig
eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 172.20.255.2  netmask 255.255.240.0  broadcast 172.20.255.255
        inet6 fe80::215:5dff:fe5a:e6b3  prefixlen 64  scopeid 0x20<link>
        ether 00:15:5d:5a:e6:b3  txqueuelen 1000  (Ethernet)
        RX packets 1024  bytes 802933 (802.9 KB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 1148  bytes 442262 (442.2 KB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
```
```bash
$ cd src/ && make
$ sudo ./garp-test eth0 172.20.255.2
```
I used wireshark.  
![image](https://github.com/cryptogus/Gratuitous-ARP/assets/60291830/403279ec-16b2-4859-9256-45596d157203)
### Reference  
https://wiki.wireshark.org/Gratuitous_ARP  
https://www.practicalnetworking.net/series/arp/gratuitous-arp/  
https://en.wikipedia.org/wiki/Address_Resolution_Protocol#ARP_announcements  
https://velog.io/@yellowsky24/데비안에서-GARP로  
https://en.wikipedia.org/wiki/ARP_spoofing

### Reference in implementation
https://en.wikipedia.org/wiki/Ethernet_frame  
https://en.wikipedia.org/wiki/EtherType  
https://www.iana.org/assignments/arp-parameters/arp-parameters.xhtml  
https://api.riot-os.org/structsockaddr__ll.html  
https://stackoverflow.com/questions/4139405/how-can-i-get-to-know-the-ip-address-for-interfaces-in-c

### arp-spoofing
https://gitlab.com/gilgil/sns/-/wikis/about-arp/report-send-arp  
https://gitlab.com/gilgil/send-arp-test  