## Nagle 알고리즘

### 이게 뭐다냐 ?

- 보낼 수 있는 데이터를 바로 패킷으로 만들지 않고, **가능한 데이터를 모아 더 큰 패킷으로 만들어**서 TCP/IP 네트워크의 효율성을 키우는 방법이다.

- *Small-packet Problem*

  프로그램이 1바이트짜리 작은 데이터를 지속적으로 내보내는 경우 TCP/IP 패킷은 40여 바이트의 헤더가 붙게 된다.(TCP 20byte, IP 20byte) 여기서 의미 있는 데이터는 1바이트이므로 오버헤드가 아주 크다. 

  요런 문제는 번호 하나 하나 누를 때마다 바로 데이터가 전송되는 `Telnet`세션에서 많이 일어나는데, 최악의 경우에는 패킷이 동시에 전송되어 혼재될 수도 있다.

  이런 문제를 해결하려는 것이 네이글 알고리즘

- 원래는 `ACK`이 오는 것과 상관 없이 데이터를 바로 바로 전송한다.

  네이글 알고리즘은 전송할 데이터를 버퍼에 가지고 있다가 상대방으로부터 `ACK`이 오면 그 때 내보낸다. `ACK`이 오지 않은 시간 동안 전송할 데이터를 버퍼에 차곡차곡 쌓아놓는다.

- 똑같은 데이터를 보내더라도 생산하는 패킷이 적으므로 네트워크 효율성이 높아진다.

  그러나 송신할 호스트가 `ACK`을 받을 때까지 기다려야 하므로 전송 속도가 느려진다.

- 만약 네트워크의 전송량이나 효율성, 혹은 부하 보다는 빠른 응답 속도가 더 중요한 경우라면 Nagle 알고리즘을 적용하지 않는다.

- 네이글 알고리즘을 적용시키면 안 좋은 경우

  - real-time 시스템에서의 제어
  - interactive한 키 입력을 하는 어플리케이션



### 동작 원리

- 수도코드

  ```c
  #define MSS "maximum segment size"
  
  if there is new data to send
    if the window size >= MSS and available data is >= MSS
      send complete MSS segment now
    else
      if there is unconfirmed data still in the pipe
        enqueue data in the buffer until an acknowledge is received
      else
        send data immediately
      end if
    end if
  end if
  ```

- 상대방 윈도우 사이즈랑 내가 보낼 데이터가 MSS보다 크면 MSS만큼 전송
- 그게 아닌데, 아직 ack이 안온 데이터가 버퍼에 남아 있는 경우 버퍼에 쌓는다.
- 그게 아닌 경우 걍 보낸다.

<br>

### `C++`에서 Nagle 옵션 설정하기

```c++
int option = TRUE;
setsockopt(socket, IPROTO_TCP, TCP_NODELAY, (const char*)&option, sizeof(option));
```

- 기본적인 소켓은 네이글 알고리즘을 사용하도록 세팅되어 있다.

- 소켓 옵션을 `TCP_NODELAY`로 설정하면 네이글  알고리즘을 사용하지 않도록 off 할 수 있다.

- 네트워크 부하가 증가하고, 서버의 전체적인 성능을 감소시키므로 꼭 필요한 경우에만 주의해서 써야 한다.

- 전송이 작은 단위로 이루어지지만, 즉각적인 응답은 필요하지 않는 어플리케이션에서만 사용되어야 한다.

  (ex. 마우스 움직임)

  

