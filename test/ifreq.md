`ifreq`는 네트워크 인터페이스의 구성 정보를 얻거나 설정하기 위한 용도로 사용되는 구조체입니다. 주로 소켓 프로그래밍에서 네트워크 인터페이스와 관련된 작업에서 활용됩니다. `ifreq` 구조체는 다양한 정보를 저장할 수 있는 유연한 형태를 갖고 있습니다.

```c
#include <sys/ioctl.h>
#include <net/if.h>

struct ifreq {
    char ifr_name[IFNAMSIZ]; /* Interface name */
    union {
        struct sockaddr ifr_addr;
        struct sockaddr ifr_dstaddr;
        struct sockaddr ifr_broadaddr;
        struct sockaddr ifr_netmask;
        struct sockaddr ifr_hwaddr;
        short           ifr_flags;
        int             ifr_ifindex;
        int             ifr_metric;
        int             ifr_mtu;
        struct ifmap    ifr_map;
        char            ifr_slave[IFNAMSIZ];
        char            ifr_newname[IFNAMSIZ];
        char           *ifr_data;
    };
};
```

`struct ifreq`의 멤버 변수들에 대한 설명은 다음과 같습니다:

1. `ifr_name`: 네트워크 인터페이스의 이름을 저장하는 문자열 배열입니다. `IFNAMSIZ`는 인터페이스 이름의 최대 길이를 나타냅니다.

2. `union`: 다양한 정보를 저장하기 위한 `union`입니다. 멤버로는 다양한 종류의 정보를 저장하는 멤버들이 있습니다.

   - `ifr_addr`: 네트워크 인터페이스의 주소 정보를 저장하는 `sockaddr` 구조체입니다. (ifr_addr 구조체의 sa_data 멤버는 주로 MAC 주소를 담고 있습니다. IPv4 를 가지고 있다는데 잘 모르겠다.)
   - `ifr_dstaddr`: 목적지 주소 정보를 저장하는 `sockaddr` 구조체입니다.
   - `ifr_broadaddr`: 브로드캐스트 주소 정보를 저장하는 `sockaddr` 구조체입니다.
   - `ifr_netmask`: 네트워크 마스크 정보를 저장하는 `sockaddr` 구조체입니다.
   - `ifr_hwaddr`: 네트워크 인터페이스의 하드웨어(MAC) 주소를 저장하는 `sockaddr` 구조체입니다.
   - `ifr_flags`: 인터페이스의 플래그를 저장하는 변수로, 이진 비트 플래그들을 포함합니다.
   - `ifr_ifindex`: 인터페이스의 인덱스를 저장하는 변수입니다.
   - `ifr_metric`: 라우팅에서 사용되는 메트릭을 저장하는 변수입니다.
   - `ifr_mtu`: 네트워크 인터페이스의 MTU(Maximum Transmission Unit)를 저장하는 변수입니다.
   - `ifr_map`: 인터페이스의 메모리 맵을 저장하는 구조체입니다.
   - `ifr_slave`: 마스터 슬레이브 관계에 있는 인터페이스의 이름을 저장하는 문자열 배열입니다.
   - `ifr_newname`: 인터페이스의 새로운 이름을 저장하는 문자열 배열입니다.
   - `ifr_data`: 임의의 데이터를 저장하는 포인터입니다.

이러한 멤버들은 `union`으로 구성되어 있어 하나의 `ifreq` 객체에서 필요한 정보에 따라 적절한 멤버를 사용할 수 있습니다.

예를 들어, 소켓을 생성하고 해당 소켓에 대한 네트워크 인터페이스 정보를 얻기 위해 `SIOCGIFHWADDR` 명령을 사용할 때에는 다음과 같이 `ifreq` 구조체를 활용할 수 있습니다:

```c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq ifr;

    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    // 네트워크 인터페이스의 이름 설정
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);

    // SIOCGIFHWADDR 명령을 사용하여 네트워크 인터페이스의 MAC 주소 획득
    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == -1) {
        perror("ioctl");
        close(sockfd);
        return -1;
    }

    // 획득한 MAC 주소 출력
    printf("MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
           (unsigned char)ifr.ifr_hwaddr.sa_data[0],
           (unsigned char)ifr.ifr_hwaddr.sa_data[1],
           (unsigned char)ifr.ifr_hwaddr.sa_data[2],
           (unsigned char)ifr.ifr_hwaddr.sa_data[3],
           (unsigned char)ifr.ifr_hwaddr.sa_data[4],
           (unsigned char)ifr.ifr_hwaddr.sa_data[5]);

    close(sockfd);
    return 0;
}
```

이 코드에서는 `SIOCGIFHWADDR` 명령을 사용하여 네트워크 인터페이스의 MAC 주소를 획득하고 출력하고 있습니다.