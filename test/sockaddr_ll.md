refer: https://man7.org/linux/man-pages/man7/packet.7.html, https://stackoverflow.com/questions/62866943/how-does-the-af-packet-socket-work-in-linux

man packet을 통해 더 많은 정보를 알 수 있다.
```bash
$ man packet
```
`struct sockaddr_ll`은 Linux에서 패킷 송수신을 위해 사용되는 소켓 주소 구조체입니다. 이는 데이터 링크 레이어 (Layer 2) 정보를 다루는 데에 사용되며, 주로 프로토콜 스택을 우회하고 네트워크 인터페이스와 직접 통신할 때 유용합니다.

`struct sockaddr_ll`의 정의는 다음과 같습니다:

```c
struct sockaddr_ll {
    unsigned short sll_family;   /* Always AF_PACKET */
    unsigned short sll_protocol; /* Physical-layer protocol */
    int            sll_ifindex;  /* Interface index */
    unsigned short sll_hatype;   /* ARP hardware type */
    unsigned char  sll_pkttype;  /* Packet type */
    unsigned char  sll_halen;    /* Length of address */
    unsigned char  sll_addr[8];  /* Physical-layer address */
};
```

- `sll_family`: 항상 `AF_PACKET`으로 설정됩니다.
- `sll_protocol`: 패킷의 물리적 레이어 프로토콜을 지정합니다.
- `sll_ifindex`: 네트워크 인터페이스의 인덱스를 나타냅니다.
- `sll_hatype`: ARP (Address Resolution Protocol)의 하드웨어 타입을 지정합니다.
- `sll_pkttype`: 패킷의 타입을 지정합니다.
- `sll_halen`: 물리적 주소의 길이를 나타냅니다.
- `sll_addr`: 물리적 주소를 저장합니다.

`struct sockaddr_ll`을 사용하면 네트워크 인터페이스와 직접 통신할 때 특정 네트워크 인터페이스에 패킷을 송수신할 수 있습니다. 이를 통해 데이터 링크 레이어에서의 저수준 패킷 조작이 가능해집니다.