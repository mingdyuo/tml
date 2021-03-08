## 3월 8일 월요일



[소켓 프로그래밍 함수](https://velog.io/@underlier12/C-15-%EC%86%8C%EC%BC%93-%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%98%EB%B0%8D-%ED%95%A8%EC%88%98)랑 [TCP 에코 통신](https://velog.io/@underlier12/C-16-TCP-%EC%97%90%EC%BD%94-%ED%86%B5%EC%8B%A0)을 봤음

[여기](https://blog.naver.com/stop2y/221105953920)는 조금 봤는데 내일 이어보겠음



### 서버

1. `socket()` 서버의 소켓 생성

   프로토콜 선정(TCP, UDP)

   ```c++
   int server_socket = socket(PF_INET, SOCK_STREAM, 0);
   ```

2. `bind()` 

   IP와 포트 등록 / 소켓 주소와 포트 할당하기 위해 구조체 `sockaddr_in` 사용

   ```c++
   struct sockaddr_in server_addr;
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET; // ipv4
   server_addr.sin_port = htons(4000); // 포트 번호 4000
   server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 32bit ipv4
   
   if(bind(server_socker, (struct sockaddr*)&server_addr, sizeof(server_addr))==-1){
       printf("bind() 실행 에러");
       exit(1);
   }
   ```

3. `listen()`

   클라이언트가 보내는 `connect()` 요청을 확인하도록 요청함

   확인되면 `accept()`로 넘김

... 시간이 없어서 정리 안함



### 걍 모르는거 정리

1. `MAKEWORD(2, 2)`

   윈속 초기화할때 쓰는건데, 2개의 인수를 word로 pack하는 매크로임

2. `inet_addr("127.0.0.1")`

   문자열 IP를 네트워크 바이트 형식으로 바꿔줌