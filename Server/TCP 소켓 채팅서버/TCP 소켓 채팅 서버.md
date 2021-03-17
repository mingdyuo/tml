### 고려해야 할 점

- 클라이언트 선택 : Win API, console, or else...

  콘솔로 할 경우 입력하는 도중 출력이 안된다고 한다? → 가능한지 체크 필요함

  `std::cin`, `std::cout`, `fprintf()`는 멀티 스레드 safe 하지 않으므로 뮤텍스 처리 필요함.





### 03월 17일 수요일

구현 내용

- IOCP 서버 win api 스레드로 변경 및 동작
- 2개의 스레드 사용하는 Client 제작
- 콘솔 상에서 채팅 화면 구현 (채팅 입력창과 대화 화면 분리)
- Client - Server, Client - Client (브로드 캐스트) 구현 완료
- 