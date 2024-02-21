# Gratuitous-ARP (GARP)
## TO do
- 에러시 프로그램 종료가 아닌 에러 로그 남기고 해당 부분 다시 시작할 수 있도록 해보기

https://datatracker.ietf.org/doc/html/rfc2002#section-4.6

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
[Recommand GARP version 2](ver2/README.md)
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
$ cd test/ && make
$ sudo ./garp-test eth0 172.20.255.2
```
or
```bash
$ cmake -B build -S . && cd build && make
$ sudo ./send-garp eth0 172.20.255.2
```
I used wireshark.
![image](https://github.com/cryptogus/Gratuitous-ARP/assets/60291830/403279ec-16b2-4859-9256-45596d157203)
## GARP(ARP announcements) version 2
```bash
$ cd ver2
```
---
**IPv6 does not use the ARP (Address Resolution Protocol) as IPv4 does.** Instead, it uses the Neighbor Discovery Protocol ([NDP](https://en.wikipedia.org/wiki/Neighbor_Discovery_Protocol)) to perform functions similar to ARP in IPv4. NDP is part of the ICMPv6 (Internet Control Message Protocol version 6) suite.

Here are the key differences between ARP in IPv4 and NDP in IPv6:

1. **Address Resolution:**
   - **IPv4 (ARP):** ARP is used to map an IPv4 address to a corresponding MAC (Media Access Control) address.
   - **IPv6 (NDP):** NDP performs both address resolution and neighbor discovery. It can resolve the link-layer address (like MAC address) associated with an IPv6 address and discover neighbors on the link.

2. **Protocol Type:**
   - **IPv4 (ARP):** ARP operates at the Link Layer and is part of the TCP/IP protocol suite.
   - **IPv6 (NDP):** NDP is part of the ICMPv6 protocol suite, and it operates at the Network Layer.

3. **Multicast Addresses:**
   - **IPv4 (ARP):** ARP requests are broadcasted to all devices on the local network.
   - **IPv6 (NDP):** NDP uses multicast addresses, such as the Solicited-Node multicast address, for more efficient communication.

4. **Router Discovery:**
   - **IPv4 (ARP):** ARP does not include a mechanism for router discovery.
   - **IPv6 (NDP):** NDP includes Router Advertisement and Router Solicitation messages for router discovery in IPv6 networks.

5. **Duplicate Address Detection (DAD):**
   - **IPv4 (ARP):** ARP does not have a standardized mechanism for Duplicate Address Detection.
   - **IPv6 (NDP):** NDP includes Duplicate Address Detection to avoid conflicts during address assignment.

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
https://github.com/pantuza/gratuitous-arp

### arp-spoofing
https://gitlab.com/gilgil/sns/-/wikis/about-arp/report-send-arp
https://gitlab.com/gilgil/send-arp-test
