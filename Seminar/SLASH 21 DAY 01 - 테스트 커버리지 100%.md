## 테스트 커버리지 100%

### 배경

- 서버 개발을 하면서 테스트 커버리지가 어느 정도 되어야 적당한지 고민했음

- 70%정도만 넘으면 괜찮겠지 하고 넘어갔었음

  그러나 클린 코더라는 책에서 테스트 커버리지 100%를 강력히 요구한다는 문장을 읽음

  100%를 유지하려면 테스트 코드의 양이 커지고, 테스트가 어려운 코드도 있을 것이라고 생각했음

- 하지만 실제로 해보기로 했다. 

  만약 현실적으로 불가능하면 왜 그런지 입증해 보려고 했다. 

### 시작

- 당시 토스 홈 리뉴얼 프로젝트에 참여하여 서버를 만들고 있었음
  - 유저의 금융 정보를 수집해서 보여주는 역할
  - spring webflux + kotlin 사용
- 결론 : 의외로 해보니 가능함 (띠용)

![image](https://user-images.githubusercontent.com/41130448/135708758-fce3930c-1c45-49f7-ac52-92da3cac407a.png)

- 커버리지 50%에서 100%까지 2개월 소요

![image](https://user-images.githubusercontent.com/41130448/135708777-adfef278-29cc-4f65-8209-e481e7cd7bb5.png)

- 초반에 프로덕션 코드 4천라인 / 테스트 코드는 2천 라인

- 시간이 날 때마다 테스트 작성 

  23%의 커밋이 테스트 작성이었다. 

- 커버리지가 84%에 도달했을 때부터는 커버리지가 낮아지면 배포가 되지 않도록 설정

### 계속 유지할 수 있을까?

- 애써서 올려놨는데 원상복구 되면 의미가 없음
- 1년 6개월간 유지함
- 떨어지면 빌드가 안되게 설정해놨기 때문에 당연한 걸수도
- 이점
  - 모든 코드가 테스트 되었다는 사실
  - 자신있게 배포 가능
  - 과감한 리팩토링 가능
    - 과거에는 영향도 예측하기 어렵다는 이유로 자제
    - 모든 코드 테스트 가능하므로 현재는 리팩토링을 주저하지 않음
  - 실행되지 않는 불필요한 프로덕션 코드가 사라진다.
  - 프로덕션 코드에 대한 이해도 상승
    - 이해가 충분하지 못하면 테스트를 작성할 수 없기 때문
    - 기존 프로덕션 코드에 모든 케이스에 대한 테스트가 존재한다.  이를 참고해서 새로운 테스트를 작성할 수 있다. 

### 테스트 커버리지를 높이기 위해 필요한 것들

1. 믿음 - "테스트가 필요하다"

   - 테스트 코드 작성은 기능 구현보다 어렵고 시간과 노력이 많이 들어가기 때문.

     고도의 정신적 노동이다. 따라서 자의적으로 할 수 있도록 믿음이 필요함.

   - 테스트 필요성에 대한 사고 흐름

     테스트가 없으면 리팩토링 할 수 없고, 리팩토링 하지 않은 코드는 이해할 수 없게 된다. 이해할 수 없는 코드는 수정할 수 없다.

2. 믿음 - "어떤 코드든 테스트할 수 있다."

   - 조금만 테스트 하기 어려운 코드를 만나도 불가능하다고 생각해서 포기할 것이다. 결과적으로 커버리지를 높일 수 없다.
   - 저도 이 믿음은 충분하지 못했지만 커버리지 100%를 달성하면서 얻게 되었음

3. 시간

   - 이미 커버리지가 높다면 추가적인 시간을 들일 일이 없지만, 낮은 상태에서 끌어올린다면 상당한 시간이 소요된다. 
   - 프로젝트 초기부터 커버리지가 높은 상태를 유지하는 것이 가장 좋다.  

### (의외로) 필요하지 않은 것

- 의지
  - 도구로 대신할 수 있다.
  - 커버리지가 지정 수치 미만이면 빌드 배포가 안되게 하면 무조건 높은 커버리지를 유지할 수 있다.

### 어려운 점

1. 테스트가 점점 느려진다.

   - 테스트의 개수가 시간이 지나면서 점점 늘어난다. -> 생산성의 저하

     전체 테스트 400개, 실행시간 1분 초과

     - 작업하면서 확인할 때마다 1분씩 기다려야 한다.
     - 배포할 때마다 1분의 시간이 추가된다. 

   - 테스트가 느려지는 원인 (1)

     - 스프링 애플리케이션 컨텍스트 로딩이 압도적인 부분을 차지.

     - 컨텍스트 로딩을 찾아내서 모두 제거했다.

     - 보통 서버의 http api를 테스트할 때 컨텍스트 로딩을 하는 경우가 많다.

       WebTestClient는 컨텍스트 로딩 없이 테스트 가능하다.

       stand alone 기능 -> 컨텍스트 없이 테스트 가능

     - 다만 main 함수가 실행될 때 스프링 애플리케이션이 기동되는지 테스트 할 때에는, 애플리케이션 컨텍스트 로딩만 제거하는 것은 상당히 어려웠다. 

       모킹 라이브러리의 스태틱 모킹 기능을 이용해서 스프링을 모킹함. 실제로 스프링 애플리케이션을 기동하지 않고 `SpringApplication.run` 이 호출되는지만 확인하여 해결함.

   - 위의 방법으로 해결하여 당분간은 괜찮았다. 그러나 테스트 케이스가 계속 증가하면서 다시 느려지기 시작함. 

     테스트 케이스의 개수는 1,600개를 넘어갔다. 전체 테스트 실행하는 데 1분이 넘게 걸리기 시작함. 

   - 테스트가 느려지는 원인 (2)

     - 원인을 찾기 위해서 IntelliJ에 내장된 프로파일러 async-profiler를 이용해서 테스트 코드를 성능 프로파일링 해보았다. 

     - 얻은 느려지는 원인들은 다음과 같다.

       Byte Buddy 초기화, SLF4J 초기화, Handlebars 컴파일, Jackson의 `ObjectMapper()` 생성, 코틀린 리플렉션 초기화, MockK

     - 여기서도 코틀린을 위한 모킹 프레임워크인 MockK가 가장 성능에 미치는 영향이 컸다. 

       - 초기화가 굉장히 느리다. 
         - 바이트 코드 조작을 위해서 Byte Buddy를 이용한다. 이 Byte Buddy의 초기화가 느리다. 
         - 동시에 코틀린 리플렉션을 이용하는데, 이 초기화도 굉장히 느렸다. 
         - 둘을 합치면 1초가 넘는 시간
       - 매번 모킹을 할 때 목 서브 클래스를 만든다. 이 때에도 수십 ms의 시간 소요.

     - 테스트가 순차적으로 실행되기 때문에, CPU를 충분히 활용하지 못했다. 

       - 테스트가 적을 때에는 큰 문제가 아님. 
       - 1,600개나 되니 하나씩 실행하는 것은 시간이 많이 걸림. 

   - 원인 해결 방법들

     | 원인                        | 해결                                                         |
     | --------------------------- | ------------------------------------------------------------ |
     | SLF4J 초기화                | 설정 파일 파싱 때문에 느린 것이다.<br />테스트에서 불필요한 로깅 설정 제거 |
     | Jackson ObjectMapper() 생성 | Jackson을 Gson으로 교체                                      |
     | Handlebars 컴파일           | handlebars 캐시 적용으로 간단히 해결                         |
     | Byte Buddy 초기화           | 기존에 더미 테스트 생성을 위해서 사용하고 있었지만<br />너무 느리므로 테스트에서 사용 중단 |
     | 코틀린 리플렉션 모듈 초기화 | 모듈 초기화를 유발하는 isSubclassOf 함수 호출 제거           |
     | MockK                       | 필수적이지 않으면 모두 제거<br />스태틱 모킹 한번 제외하고는 모두 제거<br />모킹이 필요한 경우에는 직접 목 오브젝트를 구현 |
     | 테스트의 순차적 실행        | JUnit 테스트 설정을 고쳤다.<br />테스트를 클래스 단위로 병렬 실행<br />함수 단위 병렬 실행도 가능하지만 성능상 이득이 크지 않음. <br />또한 동시성 문제로 테스트가 실패하는 경우가 잦아짐. |

     개선 후의 테스트 실행 시간은 40초 미만이 되었다. 

     이것보다 더 빠르게 만들어 보기로 함.

   - 쓰던 노트북보다 맥북이 더 빠르게 돌아가는 것을 보고 회사 돈으로 샀다.

     테스트 2.5배나 빨라졌다. 이것이 돈의 힘인가

     인텔리제이에서 6초대에 테스트 완료했다. 

2. 테스트 하기 정말 어려운 케이스들이 있다.

   - 기존에는 db 테스트, 네트워크 테스트, 프레임워크 테스트, 랜덤 테스트, 시간에 의존적인 테스트가 어려운 테스트일 것이라고 생각했다.

     하지만 이런것들은 모킹을 이용하면 그럭저럭 테스트가 가능했다. 

   - 진짜 어려운 테스트는 코틀린이 생성해낸 바이트 코드를 테스트 하는 것.

     커버리지 기준이 라인 커버리지가 아닌 인스트럭션 커버리지이기 떄문에 모든 바이트 코드 인스트럭션이 커버되어야 했다. 

   - nullable인 객체를 받아서 nonnull 객체를 반환하는 함수에서 사용된 연산자인 elvis operator가 있다. 

     - 이것을 테스트로 어떻게 커버할 수 있을까?

       객체가 null인 경우와 그렇지 않은 경우를 보면 될 것 같았지만, 그렇지 않다. 

       테스트 커버리지 레포트를 보면 커버되지 않은 브랜치가 존재한다. 

     - 코틀린 코드를 코드를 바이트 코드로 컴파일한 뒤 자바로 역컴파일 하면 이유를 알 수 있다. 

       객체 내 변수가 null인지 검사하는 분기가 있었다. 

       코틀린 코드에서는 이 변수가 nonnull임이 보장되지만 jvm 레벨에서는 보장하지 않기 때문에 컴파일 후의 바이트 코드에서는 null이 맞는지 검사하는 코드가 필요하게 된다. 하지만 코틀린에서는 nonnull 객체의 null 여부를 검사할 수 없다. 

       쉬운 해결책은 elvis operator를 if문으로 대체하는 것이다. 처음에는 영 불만스러웠지만 굳이 if문을 기피할 필요도 없다고 생각했다. 

   - 이 외에도 테스트가 어려운 케이스들이 있지만 대부분 우회책이 존재한다. 

     다만 어떻게 해도 테스트가 불가능한 케이스도 나올 수 있다. 커버리지 검사 도구가 특정 파일을 제외시키는 기능을 제공하기 때문에 이 경우에는 너무 시간을 쓰지 말고 제외시켜 버리자. 

### 그래서 유지한 100%의 가치는?

- 100%의 규칙은 단순하다.
  - 새로 추가된 코드가 조금이라도 커버되지 않으면 언제나 실패한다.
  - 실패한다면 커버리지 리포트를 보고 빠뜨린 곳을 알 수 있다. 
- 99%의 규칙은 다소 복잡하다.
  - 운 좋게 넘어갈 수도 있고, 억울하게 실패할 수도 있다. 

### 100%를 넘어서 해결해야 할 문제 

- 그래도 버그는 존재한다

1. 테스트를 잘못 작성하는 경우

   - 대표적으로는 테스트 케이스를 빠뜨리는 경우이다. 

   - 대부분 커버리지 도구가 잡아주지만 그렇지 못하는 경우도 있다. 

     테스트 커버리지가 100%임에도 불구하고 버그가 만들어질 수 있다. 연산자를 바꿔도 같은 값이 나오는 경우를 들 수 있다. 

   - 해결방법
     - 뮤테이션 테스팅 기법을 사용 : 프로덕션 코드를 무작위로 조작한 다음에 그럼에도 테스트가 통과한다면 테스트 케이스가 충분하지 않다고 판단하는 기법. 
     - pitest.org : jvm 기반에서 동작하는 애플리케이션에 대해 뮤테이션 테스팅을 실행해준다. 
     - 주의점
       - 굉장히 느리므로, 중요한 로직에만 부분적으로 적용하는 것이 좋다. 
       - 상당히 많은 연산을 하는 비싼 작업이며, 때때로 무한 루프를 유발할 수도 있다. 따라서 도입하지는 않았음. 

2. 요구사항에 오해가 생긴 경우

   - 해결책

     - 요구사항을 작성한 사람이 직접 인수 테스트를 작성하는 것이 가장 확실한 해결책이 될 것이다. 하지만 보통 요구사항 작성자는 개발자가 아니기 때문에 프로그래밍 언어로 테스트 케이스를 작성하는 것은 쉽지 않다. 

     - 그래서 반대로 개발자가 테스트를 작성하면 그 테스트가 스펙 문서로 만들어지게 하는 시도를 해보았다. 이를 요구한 사람이 읽으면 의도대로 구현된 것이 맞는지 리뷰 받을 수 있을 것이다. 

     - BDD 도구인 cucumber를 시도해보았다.

       자연어로 스펙 문서를 작성하고 스펙 문서에 대한 테스트를 작성하면 되는 방식

       별로 만족스럽지는 않았음. 가족성이 떨어지고 커스터마이징이 잘 안됨.

       무엇보다 모든 테스트를 Junit5 하나로 작성하고 싶었다. 

     - 스펙 문서로 작성해주는 도구를 만들었음.

       JUnit5의 TestExecutionListener를 이용하면 테스트가 실행될 때마다 테스트의 이름을 수집할 수 있다. 이를 이용하여 마크 다운 형식으로 된 스펙 문서를 생성하게 함. 

     - 실제로 적용해보고 요구 사항 작성자가 리뷰하여 오류를 발견하는 등의 성과가 있었다. 

       보기 좋은 문서를 만들기 위해서 테스트를 작성할 때 노력이 많이 들어간다. 항상 노력하기는 어려우므로 문서의 품질이 일정하지 못했다. 따라서 지속하지는 못함. 

3. 컴포넌트간 협업이 실패한 경우 

   - 협업하는 다른 서버 컴포넌트에 버그가 있는 경우가 있다. 

   - Consumer Driven Contracts라고 불리는 기법을 사용하여 해결 가능

     - API를 사용하는 쪽인 컨슈머가 요구사항을 machine-readable하게 기술하면 API를 제공하는 쪽인 프로바이더가 그 요구사항을 만족하는지 테스트 하는 것.

     - Spring Cloud Contract 

       계약을 관리해주는 도구가 없어서 선택 안함

     - Pact

       컴포넌트 간의 계약을 관리해주는 "Pactflow"라는 도구가 존재하여 선택 

### 결론

- 테스트 커버리지는 얼마든지 높일 수 있다.
- 테스트 커버리지가 낮으면 빌드 실패하게 하자.
- 테스트는 빨라야 한다.
- 커버리지가 100%라도 버그는 있다. 
