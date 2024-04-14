# ARP announcements vs Gratuitous ARP
https://wiki.wireshark.org/Gratuitous_ARP - Discussion
https://en.wikipedia.org/wiki/Address_Resolution_Protocol#ARP_announcements - ARP announcements also called a gratuitous ARP (GARP)

The wiki says they are the same, and there seems to be some debate in wireshark as to whether to put 00:00:00:00:00:00:00 or FF:FF:FF:FF:FF:FF in the target mac address.
**Note that the image below captures a packet with the opcode set to 1.**
### ARP announcements in wireshark
![image](https://github.com/cryptogus/Gratuitous-ARP/assets/60291830/dbdd53b0-2c0e-4355-afed-4daef3af638b)
### Gratuitous ARP in wireshark
![image](https://github.com/cryptogus/Gratuitous-ARP/assets/60291830/b85e66ba-1467-4aa4-b16f-d12d7504e7f7)

Notice that the two wireshark packet captures above differ only in the target mac address. **If anyone knows why the two are split like this, please let me know.**

## ARP announcements and Gratuitous ARP are different.
In Gratuitous ARP Packet Structure, the Opcode is set to 2, indicating a response. Refer: https://www.practicalnetworking.net/series/arp/gratuitous-arp/

![image](https://github.com/cryptogus/Gratuitous-ARP/assets/60291830/6f0333b7-12f1-4ecc-9ae8-b9fad76479f9)

The Opcode in an ARP Announcement is set to 1, indicating a request. Typical Gratuitous ARP will have an Opcode set to 2.
--중략--
Like the Gratuitous ARP, the Target MAC address is ignored, in this example it is set to 0000.0000.0000, some implementations of the ARP Announcement use ffff.ffff.ffff instead.
Refer: https://www.practicalnetworking.net/series/arp/arp-probe-arp-announcement/

![image](https://github.com/cryptogus/Gratuitous-ARP/assets/60291830/cfb12980-ab01-4af6-8107-1cf1d636165e)

I set opcode to 1 for all source code outside of the ver2 directory. When I read many articles about GARP, they all set the opcode(In the source code, the oper variable of the struct arphdr_t) to 1, and I didn't understand this, so I assigned it to 1. I'm not going to change this, just reflect it in the ver2/ directory. Of course, I was able to tamper with the arp table even when opcode was 1.

ver2 디렉터리 외의 모든 소스코드에는 opcode를 1로 설정해두었다. GARP 관련 많은 글들을 참고했을 때 모두 opcode(소스코드에서는 구조체의 oper 변수)를 1로 설정해두었고 나 또한 이런 점을 잘 몰랐기 때문에 1로 할당했었다. 이 점은 굳이 수정하지 않고 ver2/ 디렉터리에서만 반영하겠다. 물론 opcode가 1었을 때도 arp table을 변조할 수 있었다.
## ARP Packet bytes in wireshark
Sometimes wireshark captures 60-byte ARP packets with 18 bytes of zero padding, and other times it captures 42-byte ARP packets.

![스크린샷 2024-02-20 092641](https://github.com/cryptogus/Gratuitous-ARP/assets/60291830/390828ed-4dff-4f3a-8d04-41a7e2cab91b)

어떤 때는 ARP 패킷이 (request, reply 상관없이) 18바이트씩 차이가 난다. 18바이트의 0 패딩이 생기는 경우가 있는데 이 이유에 대해서 알아보자. 사실 ARP 패킷에 들어갈 정보는 42바이트면 충분하다. 굳이 아무 의미없는 0이라는 값을 18바이트나 넣어줄 필요가 단순하게 생각했을 때 성능적인 측면에서 전혀 의미가 없다.


![image](https://github.com/cryptogus/Gratuitous-ARP/assets/60291830/630e7f58-90e1-4ec5-a214-feed0c9f805f)

위 이미지에서 보이는 물리계층 헤더는 wireshark에서 찍히지 않는다. MAC 프레임 부터 패킷이 캡처 된다. DA는 Destination MAC이다. 위 패킷 캡처 이미지들을 보면  DA : Destination MAC Address, SA : Source MAC Address라고 이해하면 된다. 실제 위의 ARP 패킷을 Ethernet이라 되어있는 부분을 보면 6바이트씩 잡혀있는 것을 볼 수있다. Len/Type : 길이 또는 타입이고 2바이트로 나타낸다. 이제 LLC 프레임(데이터 + 패딩) 이라 되어있는 부분을 보면 최소 46바이트라는 정보가 있다.

결론 부터 말하자면 **최소 프레임 길이 : 64 바이트 (MAC 헤더 14 + 데이터 46 + FCS 4)** 이라는 점이다.

FCS 4바이트를 제외하면 60바이트의 ARP 패킷이 왜 잡힌 이유가 이제야 설명이 된다. (6 + 6 + 2 + 46 하면 60이다.)
쨋든 저 최소 프레임 길이(64바이트)가 아닌 패킷이 들어오면 이상한 패킷인 줄 알고 드랍해버린다고 한다.(그런 글을 본 것 같은데 아마 맞을 것 같다.)

최소 프레임 길이가 있는 이유는 바로 과거 Layer 1계층의 표준 때문이였는데, 과거 통신을 하기 위해 만들었던 최초의 케이블의 Type은 10BASE5 (대역폭 10Mbps & 거리 500M)를 이용해서 통신을 하였습니다. 현재는 Half-Duplex가 없지만 그 당시만 하더라도 Half-Duplex를 사용하고 있었기 때문에 패킷 전송 시 정상적인 패킷 전달을 위한 방법이 있었는데 그것이 CSMA/CD 방식입니다. 그런데 이 CSMA/CD방식을 이용하여 전달을 할 때에도 여전히 문제는 존재 했었습니다.  
출처: https://blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=roser111&logNo=221141395803

하지만 아직 해결해야할 부분이 있다. 바로 42바이트 패킷이 캡처된다는 점이다. **MAC 헤더 14 + 데이터 24 바이트**라는 건데 이는 아래 유튜브 링크에서도 설명해준다. 일단 42 바이트 패킷이 잡힐때는 ARP 패킷이 해당 기기에서 나갈때이다. 들어오는 패킷은 60바이트이다.(이는 패킷을 outbound하기 전에 전체 64바이트 최소 프레임 길이를 맞춰주기 위해 padding이 18 바이트가 붙기 때문이다. 그런데 왜 패킷이 나갈때가 아닌 들어올 때 이 패딩값이 찍히는 걸까?? )  
이 이유는 바로 wireshark가 캡처하는 시점이 2계층과 3계층 사이라고 한다. 즉, ARP 패킷을 보낼 때는 패딩이 붙기 전에 wireshark에서 패킷을 캡처한 것이고 받은 ARP 패킷에는 패딩이 붙어서 60바이트로 찍혀버린 것이다.

arp관련 패킷을 `packet/` 디렉터리에 3개의 pcap 파일로 올려두었다. wireshark를 이용해 보면서 공부해보면 좋을 것 같다.

refer: http://www.ktword.co.kr/test/view/view.php?m_temp1=2965 , https://youtu.be/4xSexyzxUJo?si=7rbJAFE5QNZ05s2U , https://www.youtube.com/watch?si=EoK5HHKHuV8WnOIN&v=-M_S50Ga384&feature=youtu.be

## Build & syslog
이전에 `printf` or `perror` 등으로 오류 및 성공 메시지를 출력하였다. 여기서는 로그를 남기도록 바꾸었다. 오류시 프로그램을 종료하는 기능도 없앴다.

왜냐하면 일반적인 네트워크 daemon의 특성상 백그라운드로 실행되기에 출력 기능이 있는게 말이 안되며 프로그램이 종료되는 것 또한 있을 수 없는 일이다.

```bash
$ cd ver2/
$ cmake -B build -S . && cd build && make
$ sudo ./send-garp <Your network interface> <Your IP address corresponding to the network interface>
$ tail -n 1 /var/log/syslog
Feb 19 21:28:53 LAPTOP-1Q8UP14B send-garp: Gratuitous ARP sent successfully.
# Error log example
Feb 19 21:31:26 LAPTOP-1Q8UP14B send-garp: Error opening socket
```
In WSL2 environments, for example, logs may not be recorded. (In some cases, `/etc/syslog.conf` does not exist.)
This is one of the solutions.
```bash
$ sudo apt install syslog-ng
# You can add or modify settings for how to log by opening "/etc/syslog-ng/syslog-ng.conf"
$ sudo service syslog-ng restart
# or sudo systemctl restart syslog-ng
```
