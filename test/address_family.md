네트워크 패밀리(AF_INET, AF_INET6 등)는 소켓 프로그래밍에서 주로 사용되는 주소 체계를 지정하는 상수입니다. 여러 가지 네트워크 패밀리가 있으며, 각각 다른 목적에 사용됩니다.

1. **AF_INET (IPv4):**
   - 주로 사용되는 인터넷 프로토콜 버전 4 주소 체계입니다.
   - IPv4 주소를 나타내는 데 사용됩니다.
   - 예를 들어, `struct sockaddr_in` 구조체를 사용하여 IPv4 주소와 포트를 나타낼 수 있습니다.

2. **AF_INET6 (IPv6):**
   - 인터넷 프로토콜 버전 6 주소 체계로, 더 큰 주소 공간을 제공합니다.
   - IPv6 주소를 나타내는 데 사용됩니다.
   - `struct sockaddr_in6` 구조체를 사용하여 IPv6 주소와 포트를 나타낼 수 있습니다.

3. **AF_UNIX (Unix Domain):**
   - 로컬 시스템 내의 프로세스 간 통신을 위한 주소 체계입니다.
   - 파일 시스템의 파일 경로를 사용하여 소켓을 식별합니다.
   - `struct sockaddr_un` 구조체를 사용하여 주소를 나타냅니다.

4. **AF_PACKET (Packet Interface):**
   - 데이터 링크 레이어에서의 패킷 송수신을 위한 주소 체계입니다.
   - 네트워크 인터페이스와의 저수준 통신에 사용됩니다.
   - 주로 패킷 스니핑 등의 목적으로 사용됩니다.

각각의 주소 체계는 해당 프로토콜과 관련이 있으며, 소켓 프로그래밍에서 어떤 종류의 통신을 할 것인지에 따라 적절한 패밀리를 선택해야 합니다. 주로 TCP/IP 기반의 네트워크에서는 IPv4나 IPv6를 사용하며, 로컬 통신에서는 Unix Domain이나 기타 주소 체계가 사용됩니다.