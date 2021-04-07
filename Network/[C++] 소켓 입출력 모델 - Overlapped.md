### 공부 내용 출처

[IT CookBook, TCP/IP 윈도우 소켓 프로그래밍](https://www.hanbit.co.kr/store/books/look.php?p_code=B1196865370)

<br>

### Overlapped 모델

- 고성능의 소켓 입출력을 제공하는 모델임. 크게 두 종류의 구현 방법을 제공함.

- 비동기 입출력(중첩 입출력) 이라고 부름 + 비동기 통지도 사용

- 입출력 함수 호출하고, 그게 완료됬던 상관 없이 다른 작업을 진행할 수 있음. 

  입출력이 완료되면 커널이 응용 프로그램에게 알려줌

  그러면 하던일 멈추고 입출력 결과를 처리함

- 비동기 입출력 지원하는 소켓 함수들 

  `AcceptEx()`, `ConnectEx()`, `DisConnectEx()`, `TransmitFile()`, `TransmitPackets()`, `WSAIoctl()`, `WSANSPIoctl()`, `WSAProviderConfigChange()`, `WSARecvMsg()` **`WSARecv()`, `WSARecvFrom()`, `WSASend()`, `WSASendTo()` **

### Overlapped 모델의 종류

1. 모델 (1)

   소켓 입출력 작업 완료 시 → 커널은 응용 프로그램이 등록해둔 이벤트 객체를 신호 상태로 바꾼다.

   응용 프로그램은 그 이벤트 객체를 관찰함으로써 입출력 작업 완료를 감지

2. 모델 (2)

   소켓 입출력 작업 완료 시 → 커널은 응용 프로그램이 등록해둔 함수를 자동으로 호출한다.

   (커널이 호출하는 응용 프로그램 함수는 보통 콜백 함수라고 함. 근데 Overlapped 2에서는 완료 루틴이라고 부름)

### `WSASend()`, `WSARecv()`의 특징

1. Scatter/Gather 입출력을 지원

   송신 측, 수신 측에서 `WSABUF` 구조체 배열을 사용하면 여러 버퍼에 저장된 데이터를 모아서 송신 / 흩뜨려 수신 가능

2. 마지막 두 인자에 모두 `NULL` 값을 사용하면 `send()`, `recv()` 함수처럼 동기로 작동함

   그래서 비동기로 Overlapped 모델 쓰려면 구조체 넣어줘야 함

3. Overlapped (1)에서는 `hEvent` 변수를 사용하고 Overlapped (2)에서는 `lpCompletionRoutine` 인자를 사용함. 둘 중에서는 `lpCompletionRoutine`의 우선순위가 더 높음

### Overlapped (1)

1. 비동기 입출력을 지원하는 소켓 생성. 대응하는 이벤트 객체도 생성.

2. 비동기 입출력을 지원하는 소켓 함수 호출

   `hEvent` 변수에 이벤트 객체 핸들 값 넣어서 전달함

   입출력 작업이 곧바로 완료되지 않으면 소켓 함수는 오류를 리턴함. 오류 코드는 `WSA_IO_PENDING`으로 설정되므로, 소켓에 오류 값이 들어 있더라도 오류 코드를 항상 확인해야 함

3. `WSAWaitForMultipleEvents()` 호출하여 이벤트 객체가 신호 보내는 것을 기다림

4. 비동기 입출력 작업이 완료되면 `WSAWaitForMultipleEvents()`이 리턴됨

   그러면 `WSAGetOverlappedResult()` 함수를 호출해서 입출력 결과 확인하고 데이터 처리한다.

5. 새로운 소켓 생성하면 1~4, 아니면 2~4를 반복

### Overlapped (2)

1. 스레드가 비동기 입출력 함수를 호출하면, 커널에 입출력 작업이 요청된다.

2. 해당 스레드는 비동기 입출력을 위한 특별한 대기 상태인 alertable wait 상태에 진입한다. 스레드가 이 상태에 있어야 완료 루틴이 호출될 수 있음. 

   스레드를 alertable wait 상태로 만드는 함수는 다양하게 존재한다.

3. 비동기 입출력 작업이 완료되면, 커널은 스레드의 APC 큐에 결과를 저장한다. 

   APC 큐란: asynchronous procedure call queue, 비동기 입출력 결과를 저장하기 위해 운영체제가 스레드에 할당하는 메모리 영역임

4. 스레드가 alertable wait 상태에 있으면, 커널이 APC 큐에 저장된 정보를 참조해서 완료 루틴을 호출한다. 완료 루틴 내부에서는 데이터를 처리한 후에 다시 비동기 입출력 함수를 호출할 수 있다.

5. APC 큐에 저장된 정보를 토대로 모든 완료 루틴 호출이 끝나면, 스레드는 alertable wait 상태에서 빠져나온다. 

   빠져나온 스레드가 비동기 입출력 결과를 계속 처리하려면 alertable wait 상태에 다시 진입해야 한다.