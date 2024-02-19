# ARP announcements vs Gratuitous ARP
https://wiki.wireshark.org/Gratuitous_ARP - Discussion
https://en.wikipedia.org/wiki/Address_Resolution_Protocol#ARP_announcements - ARP announcements also called a gratuitous ARP (GARP)

The wiki says they are the same, and there seems to be some debate in wireshark as to whether to put 00:00:00:00:00:00:00 or FF:FF:FF:FF:FF:FF in the target mac address.
### ARP announcements in wireshark
![image](https://github.com/cryptogus/Gratuitous-ARP/assets/60291830/dbdd53b0-2c0e-4355-afed-4daef3af638b)
### Gratuitous ARP in wireshark
![image](https://github.com/cryptogus/Gratuitous-ARP/assets/60291830/b85e66ba-1467-4aa4-b16f-d12d7504e7f7)

Notice that the two wireshark packet captures above differ only in the target mac address. **If anyone knows why the two are split like this, please let me know.**

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