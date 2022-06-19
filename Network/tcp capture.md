

## WHAT ARE THESE???

> **tcp dump, stderr**, bond0, eth0, eth1, lbvip & lo, nic, software nic, private nic, bpf (0~4?), --input-raw-bpf-filter, [goreplay의 동작방식](https://github.com/buger/goreplay/wiki/Capturing-traffic-outside-of-the-application-machine), sasl-protocol, libpcap, CIDR 포맷?, 



### TCP dump

- 네트워크 인터페이스 거치는 패킷의 헤더 출력
- 주어긴 조건식 만족시키는 애들 가져옴
- 윈도우에서는 와이어 샤크 사용

```bash
tcpdump -i eth0
```

- 이거 쳤는데 이런 디바이스 없다고 뜬다 

  - > tcpdump: eth0: No such device exists
    > (BIOCSETIF failed: Device not configured)

  - 맥에서는 네트워크 디바이스 이름이 다른것인가

  - ifconfig 치면 nic 인터페이스 이름이 나오는것 같은데 여기 없는듯..?

- 조건 걸어서 패킷 확인도 가능함

  - 포트 조건, source ip, dest ip 조건 걸어서 그것만 캡쳐 가능
    - 단순 호스트만 지정하면 해당 ip로 들어오거나 나가는 양방향 패킷 보여준다. 
  - and 옵션도 넣을 수 있음 ||, && , ! 옵션 지정가능, 그루핑도 가능
  - tcp, udp만 따로 지정 가능. ICMP나 비 TCP 트래픽도 캡쳐 가능
  - 파일로도 저장 가능함 (-w 옵션)
    - txt(표준 출력)가 아니고 bin 형식으로 저장됨
  - dump 파일 읽을 때 -r 옵션 주면 된다.

```bash
tcpdump -w tcpdump.log -i etho0 dst {{아이피}} and udp and port 5151
```

- vi 로 열면 깨지고, `tcpdump -r tcpdump.log`로 봐야 하는듯 
- snaplen
  - 패킷 덤프할 때 길이
  - 와이어샤크는 기본적으로 덤프 길이가 65535로 되어 있음 (len 길이 최대치)
  - 유닉스 환경의 tcpdump는 기본 덤프 길이가 작은듯? 데이터 그램의 처음 68 바이트..? 페이로드가 완벽히 기록되지 않을 수 있다. -s로 높여줘야함
  - tcpdump 최신 버전(4.1.1 이상?)에서는 기본 사이즈가 65535 바이트로 지정되어 있다고 한다~

```bash
[root@centos1 ~]# tcpdump -r /tmp/tcpdump.pkt | tail -5 
reading from file /tmp/tcpdump.pkt, link-type EN10MB (Ethernet) 
// 데이터 전송
20:10:12.538184 IP 192.168.100.1.telnet > 192.168.100.100.59708: P 1075:1082(7) ack 106 win 91 
20:10:12.543157 IP 192.168.100.100.59708 > 192.168.100.1.telnet: . ack 1082 win 159 
// 여기부터 접속 종료 과정
20:10:12.674086 IP 192.168.100.1.telnet > 192.168.100.100.59708: F 1082:1082(0) ack 106 win 91 
20:10:12.686700 IP 192.168.100.100.59708 > 192.168.100.1.telnet: F 106:106(0) ack 1083 win 159 
20:10:12.688650 IP 192.168.100.1.telnet > 192.168.100.100.59708: . ack 107 win 91
```

- 패킷 flag 
  - S : SYN - 연결요청 
  - F : FIN - 연결종료요청 
  - R : RST - 즉시연결종료 
  - P : PSH - 프로세스로 데이터전송 
  - U : URG - 긴급한 데이터에 데이터전송 우선순위를 할당. 
  - . : flag 가 설정되지 않았음.
- 비활성화된 서비스로 접속한 경우
  - 서버에서 seq num 응답 x
  - r flag 세팅하고, 클라에서 r 받으면 즉시 연결 종료함
  - connection refused 뜸
- 방화벽에 막힌 경우
  - Drop으로 되어 있는 경우 
    - 같은 패킷 보내도 응답 X
    - connection timeout 뜸
  - Reject로 되어 있는 경우 
    - connection refused 뜸
    - 패킷 하나만 보내고 끝남
- udp 기반 서비스
  - 3way 악수 없슴
  - 방화벽에 막힌 경우
    - drop으로 막힌 경우
    - time out나고 응답 없음
- https://logwatch.tistory.com/entry/tcpdump-packet-capture-%EB%B6%84%EC%84%9D



### `stderr`

- 스트림 여러 종류 존재
  - stdin, stdout, stderr, stdprn, stdaux
- printf 로 stdout에 쓰는 경우 버퍼링을 하는데, 버퍼 문제로 제대로 출력이 안될 수 있음
  - stdout은 개행문자(`\n`)가 한 줄의 끝으로 인식되어서 버퍼링을 끝내야 함.
  - stderr의 경우 버퍼 없이 바로 출력한다
  - 따라서 어떤 상황이 와도 가장 빠르게 에러 메시지 출력 가능



### NIC, bond0, lo

- NIC

  - 네트워크에 접속할 수 있게 하기 위해 컴퓨터 내에 설치되는 확장 카드
  - 랜카드와 같은 것인듯? NIC 마다 고유 식별 주소 갖는데 그것이 맥주소 인듯?
  - vmware 네트워크 카드는 포트가 1개 있는 single nic로 인식한다.
    - 실제 서버에 사용하는 네트워크 카드는 포트가 보통 4개 있는 quard nic를 사용한다 ? 

- 본딩이란

  - 여러 개의 NIC를 논리적으로 묶어서, 한 개의 NIC의 개수만큼 대역폭 확장하는 기술 

    - 네트워크 카드를 두개 이상 사용해야 가능함

  - active/standby로 구성 가능

    - fault tolerance 가능

  - ifconfig 치면 나오는 인터페이스들을 다 묶어서 한개의 nic 처럼 사용하면 대역폭이 확장되는것인가? 🤔

  - 여러 네트워크를 본딩한 후 최종 interface가 bond0 인듯 ? 🤔

    - 이런 식으로 정보 저장해서 매핑하는 듯

    - ```
      DEVICE=bond0
      BOOTPROTO=none
      USERCTL=no
      NM_CONTROLLED=No
      ONBOOT=yes
      IPADDR=192.168.6.150
      NETMASK=255.255.255.0
      GATEWAY=192.168.6.2
      DNS1=168.126.63.1
      DNS2=8.8.8.8
      BONDING_OPTS="mode=1 miimon=100"
      ```

  - 본딩에 여러 종류가 있음 0~6 등. 근데 보통 1번 모드 (active/stanby)가 대부분이라고 하는 듯

    - mode 0 : round-robin
    - mode 1 : active/standby
    - mode 2 : balance-xor

- lo 

  - 루프백 인터페이스

- nm

  - network manager

- 주소

  - 맥주소 : LAN 내에서 통신하기 위한 주소
  - IP주소 : 서로 다른 LAN 간의 통신을 위한 주소
  - DHCP (Dynamic Host Configuration protocol)
    - 네트워크 내부 (같은 LAN인듯?) 컴퓨터에 자동으로 네임 서버 주소, IP 주소, 게이트웨이 주소를 할당
