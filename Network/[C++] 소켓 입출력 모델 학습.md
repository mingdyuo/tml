### 공부 내용 출처

[IT CookBook, TCP/IP 윈도우 소켓 프로그래밍](https://www.hanbit.co.kr/store/books/look.php?p_code=B1196865370)

<br>

### 소켓 입출력 모델

- 다수의 소켓 관리, 소켓에 대한 입출력 처리하는 일관된 방식
- 프로그래밍 복잡도는 높아지지만, 시스템 자원을 적게 사용 가능함
- 다수의 클라이언트를 효율적으로 처리하는 서버를 만들 수 있음

<br>

### 소켓 모드의 종류

- 소켓 함수 호출 시 동작하는 방식에 따라 분류

1. 블로킹 소켓

   - 기본적으로 블로킹 소켓 생성

   - 호출 시 조건 만족 X → 함수 리턴 X, 스레드 정지

   - 조건 만족 O → 함수 리턴 O, 정지된 스레드 깨어나서 실행

   - 소켓 함수와 리턴 조건

     | 소켓 함수              | 리턴 조건                                                    |
     | ---------------------- | ------------------------------------------------------------ |
     | `accept()`             | 접속한 클라이언트가 있을 때                                  |
     | `connect()`            | 서버에 접속을 성공했을 때                                    |
     | `send()`, `sendto()`   | 응용 프로그램이 전송을 요청한 데이터를 소켓 송신 버퍼에 모두 복사했을 때 |
     | `recv()`, `recvfrom()` | 소켓 수신 버퍼에 도착한 데이터가 1바이트 이상 있고, 이를 응용 프로그램이 제공한 버퍼에 복사했을 때 |

   - `SO_SNDTIMEO`, `SO_RCVTIMEO` 소켓 옵션 사용 시 조건 만족 X라도 일정 시간 후 리턴 가능

2. 넌블로킹 소켓

   - 호출시 조건 만족 X 라도 함수가 리턴, 스레드 중단 X

   - 소켓 생성 후 전환해야 함

     ```c++
     SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
     if(sock == INVALID_SOCKET) err_quit("socket()");
     // 전환
     u_long on = 1;
     retval = ioctlsocket(sock, FIONBIO, &on);
     if(retval == SOCKET_ERROR) err_quit("ioctlsocket()");
     ```

   - 장단점 (특징)

     | 장점                                                         | 단점                                                         |
     | ------------------------------------------------------------ | ------------------------------------------------------------ |
     | - 소켓 함수 호출 시 항상 리턴하므로 교착 상태 발생 X<br />- 멀티 스레드 사용하지 않고 여러 소켓에 대해 돌아가며 입출력 처리 가능. 필요시 중간에 소켓과 관계 없는 다른 작업 가능 | - 소켓 함수를 호출할 때마다 `WSAEWOULDBLOCK`과 같은 오류 코드 확인하고 처리해야함. 프로그램 구조 복잡해짐<br />- 블로킹 소켓 사용하는 것보다 CPU 사용률 높음 (계속 확인하면서 돌기 때문) |

<br>

### 서버 작성 모델의 종류

1. 반복 서버

   - 한번에 클라이언트 하나 처리

     (스레드 한개로 구현)

   - Convoy 효과 발생 가능

   - 논리적 연결 개념을 사용하지 않는 UDP 서버에 적합. 다수의 클라이언트 동시에 서비스 가능(?)

2. 병행 서버

   - 여러 클라이언트를 동시에 처리

     (멀티 스레드로 구현)

   - 스레드 생성으로 인한 시스템 자원 소모 많음

<br>

### 이상적인 소켓 입출력 모델의 특징

- 가능한 많은 클라이언트를 수용
- 클라이언트의 서비스 요청에 빠르게 반응, 고속으로 데이터 전송
- 시스템 자원 사용량 최소화

이런 서버를 구현하기 위해서

- 소켓 함수 호출 시 블로킹을 최소화하자

  넌블로킹 소켓을 사용한 소켓 함수 호출이 항상 성공하도록 (`WSAEWOULDBLOCK`같은 오류 코드 발생 X)

- 스레드 개수를 일정 수준으로 유지하자

  스레드 늘어날수록 스택 증가하므로 메모리 사용량 증가, CPU 코어에 비례해서 생성하는 것 권장

- CPU 명령 수행과 입출력 작업을 병행하자

- 유저 모드와 커널 모드 전환 횟수를 최소화하자

  모드 전환은 CPU 사이클을 많이 소모한다.

<br>

### 소켓 입출력 모델의 종류

1. `Select`
2. `WSAAsyncSelect`
3. `WSAEventSelect`
4. `Overlapped`
5. `Completion Port`

<br>

## Select 모델

- `select()`함수가 핵심적인 역할을 한다. 
- 소켓 모드와 관계 없이 여러 소켓을 한 스레드로 처리할 수 있다.
- 소켓 모드에 따라서 `select` 모델을 사용 했을 때 뭐가 좋죠?
  - 블로킹: 소켓 함수 호출 시 조건 만족 X로 블로킹되는 상황 막음
  - 넌블로킹: 소켓 함수 호출 시 모전 만족 X로 나중에 다시 호출하는 상황 막음
- 멀티 스레드 사용하지 않고 CPU 사용률 낮출 수 있음

### 동작 원리

- 3개의 소켓 set가 필요하다. (소켓 셋 = 소켓 디스크립터를 모아놓은 것)

  → 읽기 셋, 쓰기 셋, 예외 셋

- 호출할 함수의 종류(할일)에 따라 소켓을 적당한 세트에 넣어 놓는다.

- `select()`함수를 호출하면 소켓 셋에 포함된 소켓이 입출력을 위한 준비가 될 때까지 대기. 적어도 한개 이상의 소켓이 준비되면 리턴. 소켓 셋에는 입출력 가능한 소켓만 남아 소켓 함수를 호출하고, 나머지는 제거된다.

### Select 함수

```c++
int select(
	int nfds, // 유닉스 호환성을 위한 변수, 윈도우에서는 무시
    fd_set *readfds, // 읽기, 쓰기, 예외의 소켓 셋을 나타냄. 안쓰면 NULL 넣으셈
    fd_set *writefds,
    fd_set *exceptfds,
    const struct timeval *timeout // 초나 마이크로초 단위로 지정. 
    // NULL인 경우 조건 만족 때까지 무한히 대기
    // 0 인 경우 모든 소켓 검사 후 바로 리턴
);
// 리턴값
// 성공시 조건을 만족하는 소켓 개수 or 0 (타임아웃), 실패시 SOCKET_ERROR

typedef struct timeval{
    long tv_sec; // 초
    long tv_usec; // 마이크로초
} TIMEVAL;
```

- 소켓 입출력 절차

  1. 초기화 - 소켓 셋을 비움

  2. 소켓 셋에 소켓을 넣음

     넣을 수 있는 최대 개수 정의됨 (FD_SETSIZE(64))

  3. `select()` 호출

<br>

## Completion port 모델

- 입출력 완료 포트(IOCP, I/O completion port)라는 윈도우가 제공하는 구조를 잘 이해하고 활용해야 함

- IOCP는 비동기 입출력 결과와, 결과를 처리할 스레드 정보를 담고 있는 구조이다.

  Overlapped 모델 (2)의 APC 큐와 비슷한 개념이다.

- APC 큐와의 차이점은 다음과 같다.

  1. 생성과 파괴

     APC 큐는 각 스레드마다 자동으로 생성 및 파괴가 이루어짐.

     IOCP에서는 `CreateIoCompletionPort(0)` 함수를 호출하여 생성, `CloseHandle(0)` 함수를 호출해서 파괴함

  2. 접근 제약

     APC 큐에 저장된 결과는 APC큐를 소유한 스레드만 확인 가능

     IOCP에서는 제약 없음. 보통 IOCP에 접근하는 스레드를 별도로 두고 `worker thread`라고 부른다.

     이상적인 `worker thread`의 수는 CPU 코어 수의 n배이다.

  3. 비동기 입출력 처리 방법

     APC 큐에 저장된 결과를 처리하려면 해당 스레드는 `alertable wait` 상태에 진입해야 함

     IOCP에 저장된 결과를 처리하려면 `worker thread`는 `GetQueuedCompletionStatus()` 함수를 호출해야 한다.

### 동작 과정

1. 응용 프로그램 내의 어떤 스레드가 비동기 입출력 함수를 호출, OS에 IO 작업 요청
2. 모든 `worker thread`는 `GetQueuedCompletionStatus()`함수를 호출, IOCP를 뒤져본다. 완료된 비동기 IO 작업이 없으면 모든 `worker thread`는 대기함. 대기하는 애들은 IOCP 내부에 저장됨.
3. 비동기 IO 작업이 완료되면 OS는 IOCP에 결과를 저장함. 이 저장되는 정보를 IOCpacket이라고 부름
4. OS는 IOCP에 저장된 대기중인 `worker thread`중 하나를 선택해서 깨워서 비동기 IO 결과를 처리하도록 함. 이 스레드는 필요하다면 다른 비동기 IO 함수를 호출할 수도 있다. 

### 소켓 입출력 절차

1. `createIoCompletionPort()` 함수를 호출해서 IOCP를 생성함
2. CPU 개수에 비례해서 `working thread`를 생성. 생성된 스레드는 모두 `GetQueuedCompletionStatus()`를 호출해서 대기상태가 된다.
3. 비동기 IO를 지원하는 소켓을 생성한다. 얘의 IO 결과가 IOCP에 저장되려면 `CreateIoCompletionPort()`를 호출해서 소켓이랑 IOCP를 연결해야 함
4. 비동기 IO 함수를 호출한다. 작업이 바로 완료되지 않는 경우 소켓 함수는 `SOCKET_ERROR`를 리턴함. 오류 코드는 `WSA_IO_PENDING`으로 설정됨
5. 비동기 IO 작업이 완료되면, OS는 IOCP에 결과를 저장하고 대기중인 스레드 중 하나를 깨운다. 깨어난 `working thread`는 비동기 IO 결과를 처리한다.

→ 새로운 소켓을 생성하는 경우 3~5 단계를 아닌 경우 4~5 단계를 반복함.

### IOCP 생성하기

`CreateIoCompletionPort()`의 역할은 두 가지가 있다! *

1. IOCP 오브젝트를 새로 생성하기

2. 소켓과 IOCP를 연결하기 (특정 핸들을 적용하기)

   연결하면 소켓에 대한 입출력 결과가 IOCP에 저장됨

```c++
HANDLE CreateIoCOmpletionPort(
    HANDLE FileHandle,
    HANDLE ExistingCompletionPort,
    ULONG CompletionKey,
    DWORD NumberOfConcurrentThreads
);
```

- `FileHandle`

  입출력 완료 포트와 연결할 파일 핸들. 소켓 디스크립터 넣어주면됨. 새로운 IOCP 생성시에는 `INVALID_HANDLE_VALUE` 값을 사용해도 된다.

- `ExistingCompletionPort`

  파일 또는 소켓과 연결할 IOCP 핸들. 이 값이 `NULL`이면 새로운 IOCP를 생성

- `CompletionKey`

  IOCPacket에 들어갈 부가 정도. 32비트를 넣음. 비동기 IO 완료될 때마다 생성됨. IOCP에 저장되는 정보임

- `NumberOfConcurrentThreads`

  동시에 실행할 수 있는 `worker thread`의 개수. 0을 넣으면 자동으로 CPU 개수와 같은 수로 설정됨. OS는 여기에 넣은 수 이하로 작업자 스레드의 개수를 조정함

### 비동기 입출력 결과 확인하기

- `GetQueuedCompletionStatus()`를 호출해서 IOCPacket이 들어올때까지 대기한다. 들어온 후에는 OS가 실행중인 `worker thread`의 개수를 체크하고 처음에 IOCP에 설정한 `worker thread`의 개수보다 작으면 대기중인 스레드를 깨우고 일을 시킨다.

  ```C++
  BOOL GetQueuedCompletionStatus(
  	HANDLE CompletionPort,
      LPDWORD lpNumberOfBytes,
      LPDWORD lpCompletionKey,
      LPOVERLAPPED *lpOverlapped,
      DWORD dwMilliseconds
  );
  ```

- `CompletionPort` 

  IOCP 핸들

- `lpNumberOfBytes`

  비동기 IO 작업으로 전송된 바이트 수가 저장되는 곳

- `lpCompletionKey`

  `CreateIoCompletionPort()`함 호출 시 전달한 세 번째 인자(32비트)가 저장되는 곳

- `lpOverlapped`

  비동기 IO 함수 호출 시 전달한 `OVERLAPPED` 구조체의 주소 값이 저장되는 곳

- `dwMilliseconds`

  작업자 스레드가 대기할 시간을 ms단위로 지정. 일반적으로 `INFINITE`를 넣고, IOCPacket이 생성되고 OS가 깨울 때까지 무한히 대기하도록 한다.

- `PostQueuedCompltionStatus()`를 사용하여 응용 프로그램이 `worker thread`특별한 사실을 알리기 위해 직접 IOCPacket을 생성할 수도 있다.

### 토막 지식

- `WSASend()`, `WSARecv()`가 논 블로킹 모드로 작동함
- IOCP는 MS가 특허낸 기술임 리눅스에서 못쓴당 윈도우만 쓸수있음 ㄷㄷ
- 
