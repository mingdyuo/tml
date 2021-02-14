## 스프링 부트와 AWS로 혼자 구현하는 웹 서비스



1. **인텔리 제이로 스프링부트 시작하기**

   1.1 인텔리제이 소개

   1.2 인텔리제이 설치하기

   1.3 인텔리제이 커뮤니티에서 프로젝트 생성하기

   1.4 그레이들 프로젝트를 스프링부트 프로젝트로 변경하기

> 의존성 라이브러리 원격 저장소
>
> 1. mavenCenter()
>    - 기본적으로 많이 사용하는 것
>    - 본인이 만든 라이브러리 업로드 시 많은 설정과 과정이 필요
>
> 2. jcenter()
>    - 라이브러리 업로드를 간편화한 것
>    - jcenter에 업로드 시 mavenCenteral에도 업로드 되도록 자동화 가능

   1.5 인텔리에이에서 깃과 깃허브 사용하기



2. **스프링부트에서 테스트 코드를 작성하자**

   2.1 테스트 코드 소개

   > **TDD** 와 **단위 테스트 (unit test)**는 다르다
   >
   > 1. TDD
   >
   >    테스트가 주도하는 개발, 테스트 코드를 먼저 작성하며 시작함
   >
   >    | Red                                  | Green                                      | Refactor                                        |
   >    | ------------------------------------ | ------------------------------------------ | ----------------------------------------------- |
   >    | 항상 실패하는 테스트를 먼저 작성하고 | 테스트가 통과하는 프로덕션 코드를 작성하고 | 테스트가 통과하면 프로덕션 코드를 리팩토링한다. |
   >
   > 2. 단위 테스트 
   >
   >    TDD의 첫 번째 단계인 **기능 단위의 테스트 코드를 작성**하는 것 의미
   >
   >    테스트 코드를 꼭 먼저 작성하지도 X, 리팩토링 포함 X
   >
   > [TDD 실천법과 도구](https://repo.yona.io/doortts/blog/issue/1)

   - 테스트 코드를 작성하는 이유

     1. 개발 단계 초기에 문제 발견을 도와줌

     2. 개발자가 만든 기능을 안전하게 보호해줌

        기능 B를 추가했을 때 잘 되던 기능 A에 문제가 생길 경우 빠른 발견 가능

     3. 프린트 찍어서 눈으로 검증하는 것보다 낫다 자동검증이 됨

     4. 단위 테스트 자체가 문서 역할 가능

   - 테스트 코드 작성 프레임워크

     1. Java - JUnit
     2. DB - DBUnit
     3. C++ - CppUnit
     4. .net - NUnit

   

   2.2 Hello Controller 테스트 코드 작성하기

   > 스프링 부트에서는 **내장 WAS 사용을 권장**함
   >
   > - 언제 어디서나 같은 환경에서 스트링 부트를 배포할 수 있기 때문
   >
   > - 외장 WAS를 쓴다고 하면, 모든 서버는 WAS의 종류와 버전, 설정을 일치시켜야 함
   >
   >   새로운 서버 추가시 모든 서버가 같은 환경 구축해야 함 → 노답이다.
   >
   > `src/main/java/com/mingdyuo/book/springboot/web/HelloController`
   >
   > 1. `@RestController`
   >    - 컨트롤러를 JSON을 반환하는 컨트롤러로 만들어줌
   >    - 예전에는 `@ResponseBody`를 각 메소드마다 선언 → 이를 한번에 사용할 수 있도록 해줌
   > 2. `@GetMapping`
   >    - HTTP Method인 Get의 요청을 받을 수 있는 API 만들어줌
   >    - 예전에는 `@RequestMapping`(method = RequestMethod.GET)으로 사용됨
   >    - `/hello`로 요청이 오면 문자열 `"hello"`를 반환하는 기능 만들어준 것
   >
   > `src/test/java/com/mingdyuo/book/springboot/web/HelloControllerTest`
   >
   > 1. `@RunWith(SpringRunner.class)`
   >
   >    - 테스트 진행 시 JUnit에 내장된 실행자 외에 다른 실행자를 실행
   >
   >    - 여기서는 SpringRunner라는 스프링 실행자를 사용
   >
   >      즉, 스프링 부트 테스트와 JUnit 사이에서 연결자 역할
   >
   > 2. `@WebMvcTest`
   >
   >    - 여러 스프링 테스트 어노테이션 중 Web(Spring MVC)에 집중할 수 있는 어노테이션
   >    - 선언할 경우 `@Controller`, `@ControllerAdvice`등을 사용할 수 있음
   >    - 단 `@Service`, `@Component`, `@Repository` 등은 사용할 수 없음
   >    - 여기서는 컨트롤러만 사용하기 때문에 선언함
   >
   > 3. `@Autowired`
   >
   >    스프링이 관리하는 빈(Bean)을 주입받는다.
   >
   > 4. `private MockMvc mvc`
   >
   >    - 웹 API를 테스트할 때 사용
   >    - 스프링 MVC 테스트의 시작점
   >    - 이 클래스를 통해 HTTP GET, POST 등에 대한 API 테스트를 할 수 있음
   >
   > 5. `mvc.perform(get("/hello"))`
   >
   >    - MockMvc를 통해 `/hello` 주소로 HTTP GET 요청을 함
   >    - 체이닝(쩜)이 지원되므로 아래와 같이 여러 검증 기능을 이어서 선언 가능
   >
   > 6. `.andExpect(status().isOk())`
   >
   >    - `mvc.perform`의 결과를 검증함
   >
   >    - HTTP header의 status를 검증함
   >
   >      → 200(OK)인지 아닌지 검증
   >
   > 7. `.andExpect(content().string(hello))`
   >
   >    - `mvc.perform`의 결과를 검증
   >    - 응답 본문의 내용을 검증
   >    - Controller에서 "hello"를 리턴하기 때문에 이값이 맞는지 검증함

   * 절대 수동으로 검증 후 테스트 코드를 작성하지 않는다.

     **테스트 코드로 먼저 검증 후**, 정말 못 믿겠을 때 프로젝트를 실행해 확인한다.

   아씨 모니터 사고싶다...

   

   2.3 롬복 소개 및 설치하기

   2.4 Hello Controller 코드를 롬복으로 전환하기

   > `src/main/java/com/mingdyuo/book/springboot/web/dto/HelloResponseDto`
   >
   > 1. `@Getter`
   >
   >    선언된 모든 필드의 get 메소드를 생성해줌
   >
   > 2. `@RequiredArgsConstructor`
   >
   >    - 선언된 모든 final 필드가 포함된 생성자를 생성해 준다
   >    - final이 없는 필드는 포함 X

   > `src/test/java/com/mingdyuo/book/springboot/web/dto/HelloResponseDtoTest`
   >
   > 1. `assertThat`
   >
   >    - `assertj`라는 테스트 검증 라이브러리의 검증 메소드
   >
   >    - 검증하고 싶은 대상을 메소드 인자로 받음
   >
   >    - 메소드 체이닝이 지원
   >
   >      `isEqualTo`와 같이 메소드를 이어서 사용 가능
   >
   > 2. `isEqualTo`
   >
   >    - `assertj`의 동등 비교 메소드
   >    - `assertThat`에 있는 값과  `isEqualTo`의 값을 비교해서 같을 때만 성공

   - `Junit`과 비교했을 때 `assertj`의 장점

     1. `CodeMatchers`와 달리 추가적인 라이브러리가 필요하지 않음

        `Junit`의 `assertThat`을 쓰면 `is()`와 같은 `CoreMatchers` 라이브러리가 필요함

     2. 자동완성이 좀 더 확실하게 지원됨

     3. [참고 영상](https://youtu.be/zLx_fI24UXM)

   > 1. `param`
   >    - API 테스트 할 때 사용될 요청 파라미터를 설정
   >    - 단 값은 `String`만 허용
   >    - 따라서 숫자/날짜 등의 데이터도 등록시에는 문자열로 변경해야 ㅎ ㅏㅁ
   > 2. `jsonPath`
   >    - JSON 응답값을 필드별로 검증할 수 있는 메소드
   >    - `$`를 기준으로 필드명을 명시
   >    - 예를 들어 `name, amount`를 검증 시 `$.name, $.amount`로 검증

   

3. **스프링부트에서 JPA로 데이터베이스 다뤄보자**

   JPA는 자바 표준 ORM(Object Relational Mapping) 기술임. 관계형 데이터베이스를 사용하면서 객체지향 프로그래밍을 하기 위해 사용한다. 

   3.1 JPA 소개

   - 현대의 웹 애플리케이션에서 RDB는 매우 중요한 요소임. (Oracle MySQL, MSSQL를 통해 조작) 따라서 객체를 관계형 데이터베이스에서 관리하는 것이 아주 중요하다. 

   - RDB가 웹 서비스의 중심이 되면서 모든 코드가 SQL 중심이 되어가고 있음. 이는 RDB가 SQL만 인식할 수 있기 때문인데, 이 때문에 각 테이블마다 기본적인 CRUD(Create, Read, Update, Delete)를 매번 생성해야 함. 따라서 개발자가 아무리 클래스를 잘 설계해도 SQL을 통해서만 디비에 저장 조회가 가능함. SQL은 피할 수 없음. 엄청 많은 테이블에 대해서 단순 작업을 만복해야 함

   - 또한 패러다임 불일치 문제도 발생함. 

     > **관계형 데이터베이스** 어떻게 데이터를 저장할지에 초점
     >
     > **객체 지향 프로그래밍**  메시지를 기반으로 기능과 속성을 한 곳에서 관리
     >
     > → 시작점 자체가 다르다.

   - 상속, 1:N 등 다양한 객체 모델링을 DB로 구현할 수 없기 때문에 웹 애플리케이션을 개발할 때에는 점점 데이터 베이스 모델링에 집중하게 된다.

     **→ 이 중간에서 패러다임 일치를 시켜 주는 것이 JPA**

     - 개발자가 객체 지향 프로그래밍을 하면 JPA가 이를 RDB에 맞게 SQL을 대신 생성한다.
     - SQL에 종속적인 개발을 하지 않아도 됨

   - JPA는 인터페이스이기 때문에 구현체가 필요하지만, Spring에서 사용할 때에는 구현체를 추상화시킨 Spring Data JPA라는 모듈을 이용하여 다룬다.

     **JPA ← Hibernate ← Spring Data JPA**

   - Spring Data JPA의 장점

     1. 구현체 교체의 용이성

        hibernate과 같은 구현체를 다른 것으로 바꿀 때 편리

     2. 저장소 교체의 용이성

        RDB외에 다른 저장소로 교체할 때 편리

        - 트래픽이 많아지면 RDB로는 감당이 안될 수 있음
        - 이 때 MongoDB로 교체가 필요하다면, Spring Data JPA에서 Spring Data MongoDB로 **의존성만 교체하면 된다.**
        - 이는 Spring Data의 하위 프로젝트는 기본적인 CRUD의 인터페이스가 같기 때문임기본적인 기능은 변경할 것이 없음

   - JPA를 잘 쓰려면 객체 지향 프로그래밍과 RDB를 둘 다 이해해야 함. 러닝커브가 높다. 

     

   3.2 프로젝트에 Spring Data JPA 적용하기

   - `build.gradle`

     > 1. `spring-boot-starter-data-jpa`
     >    - 스프링 부트용 Spring Data JPA 추상화 라이브러리
     >    - 스프링 부트 버전에 맞춰서 자동으로 JPA 관련 라이브러리의 버전을 관리해준다. 
     > 2. `h2`
     >    - 인메모리 관계형 DB
     >    - 별도의 설치 없이 의존성 만으로 관리 가능
     >    - 메모리에서 실행되므로 애플리케이션 재시작마다 초기화됨. 테스트 용도로 많이 사용
     >    - 여기서는 JPA 테스트, 로컬 환경 구동에서 사용할 것임

   - `domain` 패키지 : 게시글, 댓글, 회원, 정산, 결제 등의 소프트웨어 요구사항 영역

     원래는 xml에 쿼리를 답고 클래스에는 쿼리에 결과만 담았지만, 이제는 다 도메인 클래스에서 해결한다.

     [참고 자료(DDD START! 도메인 주도 설계 구현과 핵심 개념 익히기)](http://www.yes24.com/Product/Goods/27750871)

   - 주요 어노테이션을 클래스에 가깝게 두는 게 좋다.

     > `@Entity`는 JPA의 어노테이션이고, `@Getter, @NoArgsConstructor`는 롬복의 어노테이션
     >
     > 롬복은 코드를 단순화 시켜주지만 필수 어노테이션은 아님. 따라서 주요 어노테이션인 `@Entity`를 코드에 가까이 두는 게 좋다. 이 경우 코틀린 등으로 언어를 전환시 롬복이 필요 없는 경우 쉽게 삭제 가능

   - Entity Class

     실제 DB table과 매칭될 클래스, Entity 클래스라고도 함

     * JPA 사용시 DB 데이터에 작업할 경우 실제 쿼리 날리지 않고

     * 이 Entity 클래스의 수정을 통해 작업 가능

   - `com.mingdyuo.book.springboot.domain.posts.Posts`

     > 1. `@Entity`
     >
     >    - 테이블과 링크될 클래스임을 나타냄
     >
     >    - 기본값으로 클래스의 카멜케이스 이름을 언더스코어 네이밍으로 테이블 매칭
     >
     >      SalesManager.java → sales_manager table
     >
     > 2. `@Id`
     >
     >    - 해당 테이블의 PK 필드를 나타냄
     >
     > 3. `@GeneratedValue`
     >
     >    - PK의 생성 규칙을 나타냄
     >    - 스프링부트 2.0에서는 `GeneratedType.IDENTITY` 옵션을 추가해야만 `auto_increment`가 된다.
     >    - [스프링부트 2.0과 1.5의 차이](https://jojoldu.tistory.com/295)
     >
     > 4. `@Column`
     >
     >    - 테이블의 칼럼을 나타냄
     >
     >    - 굳이 선언 안해도 해당 클래스의 필드는 모두 칼럼이 됨
     >
     >    - 기본값 외에 추가로 변경이 필요한 옵션이 있을 때 사용한다. 
     >
     >      > (예)
     >      >
     >      > 문자열의 경우 VARCHAR(255)가 기본값인데, 사이즈를 500으로 늘리고 싶거나 타입을 TEXT로 변경하고 싶은 경우 등'
     >
     > 5. `@NoArgsConstructor`
     >
     >    - 기본 생성자 자동 추가
     >    - `public Posts(){ }`와 같은 효과
     >
     > 6. `@Getter`
     >
     >    클래스 내 모든 필드의 Getter 메소드를 자동 생성
     >
     > 7. `@Builder`
     >
     >    - 해당 클래스의 빌더 패턴 클래스를 생성
     >    - 생성자 상단에 선언 시 생성자에 포함된 필드만 빌더에 포함

   - Entity의 PK는 Long 타입의 `Auto_increment`가 좋다. (MySQL으로 bigint 타입)

     주민등록번호와 같이 비즈니스 상 unique 키거나, 여러 키를 조합한 복합키로 PK를 잡을 시 난감한 상황이 발생할 수 있음

     1. FK를 맺을 때 다른 테이블에서 복합키 전부를 갖고 있거나, 중간 테이블을 하나 더 둬야 하는 상황이 발생할 수 있음
     2. 인덱스에 안좋은 영향
     3. 유니크한 조건 변경시 PK 전체를 수정해야함

     → 주민등록번호, 복합키 등은 유니크 키로 별로 추가하는 것이 좋다.

   - Entity 클래스에서는 절대 Setter 메소드를 만들지 않는다. 

     - 무작정 getter/setter 생성시 클래스의 인스턴스 값들이 언제 어디서 변해야 하는지 코드상으로 명확하게 구분하기 어려우므로 차후 기능 변경 시 복잡해짐
     - 해당 필드의 값 변경이 필요하다면, 그 목적과 의도를 명확히 나타낼 수 있는 메소드 추가해야함.

   - Setter가 없다면 어떻게 값을 채워서 DB에 삽입할까

     - 기본적 구조는 생성자를 통해 최종값을 채운 후 DB에 삽입하는 것

     - 값 변경이 필요하면 해당 이벤트에 맞는 public 메소드를 호출해서 변경

     - 이 프로젝트에서는 생성자 대신 `@Builder`를 통해 제공되는 빌더 클래스를 사용함

       빌더를 사용하는 경우 어느 필드에 어떤 값을 채워야 하는지 명확히 인지 가능.

   - JPA Repository

     - 인터페이스로 생성

   - ibatis나 MyBatis 등에서 Dao라고 불리는 DB Layer임.

     - `JpaRepository<Entity 클래스, PK타입>`상속 시 기본적인 CRUD 메소드가 자동으로 생성됨
     - `@Repository`를 추가할 필요 없으나, Entity 클래스와 기본 Entity Repository는 함께 위치해야함. 둘은 밀접한 관계이며 기본 Repository 없이 Entity 클래스는 역할을 할 수 없음.
     - 프로젝트 규모가 커져서 도메인별로 프로젝트를 분리해야 한다면, 도메인 패키지에서 함께 관리한다.

   3.3 Spring Data JPA 테스트 코드 작성하기

   - `com.mingdyuo.book.springboot.web.domain.posts.PostsRepositoryTest`

     > 1. `@After`
     >    - Junit에서 단위 테스트가 끝날 때마다 수행되는 메소드를 지정함
     >    - 보통은 배포 전 전체 테스트를 수행할 때 테스트 간에 데이터 침범을 막기 위해 사용함
     >    - 여러 테스트가 동시에 수행되면, 테스트용 DB인 H2에 데이터가 그대로 남아 있어 다음 테스트 실행 시 테스트가 실패할 수 있음.
     > 2. `postsRepository.save`
     >    - 테이블 posts에 `insert/update` 쿼리를 실행
     >    - id 값이 있다면 update가, 없다면 insert 쿼리가 실행됨
     > 3. `postsRepository.findAll`
     >    - 테이블 posts에 있는 모든 데이터를 조회

   3.4 등록/수정/조회 API 만들기

   - API 만들기 위해 필요한 클래스

     1. Request 데이터를 받을 Dto
     2. API 요청을 받을 Controller
     3. 트랜잭션, 도메인 기능 간의 순서를 보장하는 Service
        - 얘는 순서만 보장하는 역할을 함
        - 비즈니스 로직을 처리하는 것이 아님

   - Spring 웹 계층

     1. Web layer

        - 흔히 사용하는 컨트롤러(`@Controller`)와 JSP/Freemarker 등의 뷰 템플릿 영역
        - 이외에도 필터(`@Filter`), 인터셉터, 컨트롤러 어드바이스(`@ControllerAdvice`) 등 **외부 요청과 응답**에 대한 전반적인 영역을 이야기함.

     2. Service layer

        - `@Service`에 사용되는 서비스 영역
        - 일반적으로 Controller와 Dao의 중간 영역에서 사용
        - `@Transaction`이 사용되어야 하는 영역이기도 함.

     3. Repository layer

        - Database와 같이 데이터 저장소에 접근하는 영역
        - 기존에 개발했다면 Dao(Data Access Object) 영역으로 이해해도 됨

     4. Dtos

        - Dto는 계층 간에 데이터 교환을 위한 객체를 의미

          Dtos는 이들의 영역을 의미

        - 예를 들어, 뷰 템플릿 엔진에서 사용될 객체나 Repository Layer에서 결과로 넘겨준 객체 등이 이를 이야기함.

     5. Domain Model

        - 도메인이라 불리는 개발 대상을 모든 사람이 동일한 관점에서 이해할 수 있고, 공유할 수 있도록 단순화시킨 것

        - 예를 들어 택시 앱을 이야기할 때, 배치, 탑승, 요금 등이 모두 도메인이 될 수 있음

        - `@Entity`가 사용된 영역 역시 도메인 모델로 이해할 수 있다.

        - 무조건 데이터 베이스의 테이블과 관계가 있어야 하는 것은 아님. VO처럼 값 객체들도 이 영역에 해당하기 때문

       > | Web Layer        | DTOs         |
       > | ---------------- | ------------ |
       > | Service Layer    |              |
       > | Repository Layer | Domain Model |

   - 비즈니스 처리를 담당해야 하는 곳은 Domain이다.

   - 기존에 서비스로 처리하던 방식을 트랜잭션 스크립트라고 한다.

   - 스프링에서 Bean을 주입받는 방식

     1. `@Autowired`

        권장하지 않음

     2. setter

     3. 생성자

        가장 권장하는 방식. `@Autowired`와 동일한 효과임

        `@RequiredArgsConstructor`에서 생성자 만들어줌

   - Entity 클래스는 절대 Request/Response로 사용하면 안된다.

     - 데이터 베이스와 맞닿은 핵심 클래스임

     - Entity를 기준으로 테이블이 생성되고 스키마가 변경됨

     - Entity 클래스와 비슷한 내용이지만 request/response용으로 Dto를 따로 만드는 이유임

       → View layer와 DB layer의 철저한 역할 분리를 권장함

     - 실제 controller에서 결괏값으로 여러 테이블을 조인해서 줘야 할 경우가 많으므로 Entity 클래스만으로는 표현하기 어려울 수 있음 

   - `@WebMvcTest`의 경우 JPA 기능이 작동하지 않으며, Controller와 ControllerAdvice 등 외부 연동과 관련된 부분만 활성화된다. JPA 기능까지 한번에 테스트 할 때에는 `@SpringBootTest`와 같이 TestRestTemplate를 사용함

   - posts update에는 쿼리를 날리는 부분이 없다. 이는 JPA의 영속성 컨텍스트 덕분

   - 영속성 컨텍스트란

     - 엔티티를 영구하게 저장한느 환경, 논리적 개념

     - JPA의 핵심 내용은 엔티티가 영속성 컨텍스트에 포함되어 있냐 아니냐로 갈린다.

     - JPA의 엔티티 매니저가 활성화된 상태로 트랜잭션 안에서 DB 데이터를 가져오면, 이 데이터는 영속성 컨텍스트가 유지된 상태임

     - 이 상태에서 해당 데이터의 값을 변경하면, 트랜잭션이 끝나는 시점에 해당 테이블에 변경분을 반영한다. 즉, Entity 객체의 값만 변경하면 별도로 Update 쿼리를 날릴 필요가 없다. 

       → [더티 체킹](https://jojoldu.tistory.com/415)

       

   3.5 JPA Auditing으로 생성시간/수정시간 자동화하기

   - JPA Auditing 사용 이유

     entity에서는 보통 데이터 생성, 수정 시간을 포함하는데, 이 때문에 매번 DB에 삽입 및 갱신 전에 날짜 데이터를 등록하거나 수정하는 코드가 여기저기 쓰이게 된다. 이런 단순 반복을 지양하기 위해 사용

   - LocalData 사용

     날짜 타입 사용. Java8의 `LocalData`, `LocatDateTime`을 쓴다. 

   - `BaseTimeEntity.java`

     1. `@MappedSuperclass`

        JPA Entity 클래스들이 BaseTimeEntity를 상속할 경우 필드들(createdDate, modifiedDate)도 컬럼으로 인식하도록 함

     2. `@EntityListeners(AuditingEntityListener.class)`

        BaseTimeEntity 클래스에 Auditing 기능을 포함시킨다.

     3. `@CreatedDate`

        Entity가 생성되어 저장될 때 시간이 자동 저장됨

     4. `@LastModifiedDate`

        조회한 Entity의 값을 변경할 때 시간이 자동 저장됨

   - 미리 만든 BaseTimeEntity를 상속받으면 추가될 엔티티의 등록/수정일 고민을 할 필요가 없어진다.

   - [자바 ORM 표준 JPA 프로그래밍 - 김영한](http://www.kyobobook.co.kr/product/detailViewKor.laf?mallGb=KOR&ejkGb=KOR&barcode=9788960777330)



4. **머스테치로 화면 구성하기**

   4.1 서버 템플릿 엔진과 머스테치 소개

   - 템플릿 엔진이란

     - 지정된 템플릿 양식과 데이터가 합쳐져 HTML 문서를 출력하는 소프트웨어

     1. 서버 템플릿 엔진 : JSP, Freemarker 등
        - 서버 템플릿 엔진은 서버에서 구동됨
        - 화면 생성을 할 때, 서버에서 Java 코드로 문자열을 만든 뒤 이 문자열을 HTML로 변환하여 브라우저로 전달함
     2. 클라이언트 템플릿 엔진 : 리액트, 뷰 등 
        - 자바 스크립트는 브라우저 위에서 작동됨, 즉 코드의 실행 장소는 서버가 아닌 브라우저, 따라서 서버의 제어를 받을 수 없음
        - 이런 SPA(Single Page Application)는 브라우저에서 화면을 생성함. 
        - 서버에서는 Json, Xml 형식의 데이터만 전달하고 클라이언트에서 조립함
        - 최근데는 리액트나 뷰에서 서버사이드 렌더링을 지원하기도 하는데, 아무튼 그렇다.
        - 스프링 부트를 사용하면서 JS를 서버사이드에서 렌더링하도록 구현하는 것은 많은 수고가 필요함. 

   - [머스테치](http://mustache.github.io)란

     - 수많은 언어를 지원하는 가장 심플한 템플릿 엔진

     - 현존 대부분 언어를 지원 

       → 자바에서 사용 시 서버 템플릿 엔진, JS 사용시 클라이언트 템플릿 엔진으로 사용 가능

     - 장점은 문법이 심플하고, 로직 코드를 사용할 수 없으므로 view 역할과 서버 역할이 명확히 분리되며, *.js와 *.java 둘 다 있으므로 문법 하나로 클라/서버 템플릿 모두 사용 가능하다는 것임.

     - 그 외에 JSP, Velocity, Freemarker, Thymeleaf 등이 있음

       

   4.2 기본 페이지 만들기

   - 머스테치 파일의 기본 위치

     `src/main/resources/templates`

   - 머스테치 스타터는 컨트롤러에서 문자열 반환 시 앞의 경로와 뒤의 파일 확장자를 자동으로 지정함

     앞의 경로는 `src/main/resources/templates`이고 뒤의 파일 확장자는 `.mustache`가 붙는데, 컨트롤러에서 `"index"`를 반환하므로 `src/main/resources/templates.mustache`로 전환 되며 View Resolver가 처리한다.

     (View Resolver란 URL 요청의 결과를 전달할 타입과 값을 지정하는 관리자 격으로 볼 수 ㅣ있다.)

     

   4.3 게시글 등록 화면 만들기

   - 프론트 엔드 라이브러리 사용하는 방법

     1. 외부 CDN을 사용

        간단하므로 이번 프로젝트에서 사용할 것이지만, 외부에 의존하는 것이기 때문에 실제 서비스에서는 잘 사용하지 않는다. 

     2. 직접 라이브러리를 받아서 사용

        npm/bower/yarn + grunt/gulp/webpack 등

   - 레이아웃 방식의 외부 CDN 추가

     공통 영역을 별도의 파일로 분리하여 필요한 곳에서 가져다 쓰는 방식

   - 부트스트랩은 제이쿼리에 의존

   - js 파일들에 같은 이름의 함수가 있다면, 브라우저 내 공용 공간 스코프에 모두 들어가기 때문에 나중에 로딩된 함수가 이전에 로딩된 함수를 덮어쓰게 된다. 이를 해결하기 위해 파일 내에서 유효 범위를 만들어 사용한다.

     (ex) 페이지 별 init(), save() 등

     var 객체를 만들어 해당 객체 내에 필요한 모든 function을 선언하는 방식으로 사용

   - index.js의 호출 경로는 절대경로로 바로 시작한다. 기본적으로 `src/main/resources/static`에 위치한 js, css, image 등의 정적 파일은 url에서 `/`로 설정된다.

   - `index.mustache`

     1. `{{#posts}}`

        - posts라는 List를 순회한다.

        - java의 for문과 동일하게 생각하면 됨

     2. `{{id}}`등의 `{{변수명}}`

        리스트에서 뽑아낸 객체의 필드를 사용

     3. 다음과 같이 사용 가능

        ```
        {{#posts}}
            <tr>
                <td>{{id}}</td>
                <td>{{title}}</td>
                <td>{{author}}</td>
                <td>{{modifiedDate}}</td>
            </tr>
        {{/posts}}
        ```

   - 규모가 있는 프로젝트에서는 FK의 조인 등의 복잡한 조건 때문에 entity 클래스만으로 처리하기 어려우므로 조회용 프레임워크를 추가로 사용한다. 대표적으로는 querydsl, jooq, MyBatis 등이 있다. 이 프레임워크들을 이용해서 조회를 하고, SpringDataJpa를 통해 등록/수정/삭제를 진행한다.

   - Querydsl을 개인적으로 추천하는 이유

     1. 타입 안정성이 보장됨

        단순한 문자열로 쿼리를 생성하는 것이 아니라 메소드를 기반으로 쿼리를 생성하므로 오타나 존재하지 않는 컬럼명을 명시할 경우 IDE에서 자동으로 검출됨. 

     2. 국내 많은 회사에서 사용중

        쿠팡, 배민 등 JPA를 적극적으로 사용하는 회사에서는 Querydsl을 적극적으로 사용중임

     3. 레퍼런스가 많음

        많은 회사와 개발자들이 사용하므로 국내 자료가 많음

   - `@Transactional`에 옵션 `readOnly = true`를 추가하면 트랜잭션 범위는 유지하되, 조회 기능만 남겨두어 조회 속도가 개선됨. 

     등록, 수정, 삭제 기능이 전혀 없는 메소드에서 사용하면 좋다.

   - 람다식

     `.map(PostsListResponseDto::new)`는 `.map(posts -> new PostsListResponseDto(posts))`와 같다.

   - `IndexController.java` 의 Model

     - 서버 템플릿 엔진에서 사용할 수 있는 객체를 저장할 수 있음

     - 여기서는 `postsService.findAllDesc()`로 가져온 결과를 posts로 index.mustache에 전달함

       

   4.5 게시글 수정, 삭제 화면 만들기

   - 에러 발생

     ```
     {"readyState":4,"responseText":"{\"timestamp\":\"2021-01-17T02:56:56.389+0000\",\"status\":405,\"error\":\"Method Not Allowed\",\"message\":\"Request method 'DELETE' not supported\",\"path\":\"/api/v1/posts/1\"}","responseJSON":{"timestamp":"2021-01-17T02:56:56.389+0000","status":405,"error":"Method Not Allowed","message":"Request method 'DELETE' not supported","path":"/api/v1/posts/1"},"status":405,"statusText":"error"
     ```

     에러 발생 이유 : delete mapping을 아직 안한 상태에서 프론트만 만들고 ㅈㄴ 요청 보낸 것이었다..... ㅎㅎ;;

     

5. **스프링 시큐리티와 OAuth 2.0으로 로그인 기능 구현하기**

   5.1 스프링 시큐리티와 스프링 시큐리티 Oauth2 클라이언트

   - 많은 서비스에서 직접 id/password로 로그인 구현보다는 다른 사이트를 경유하는 소셜 로그인 기능을 사용함

     그 이유는 직접 구현하면 배보다 배꼽이 커질 수 가 있기 때문임 (다음과 같은 것)

     > 1. 로그인 시 보안
     > 2. 회원가입 시 이메일 혹은 전화번호 인증
     > 3. 비밀번호 찾기
     > 4. 비밀번호 변경
     > 5. 회원정보 변경

     OAuth 로그인을 구현하면 이런 것들을 다 다른 서버에 맡기면 되니까 서비스 개발에 집중할 수 있음

   - 스프링 부트 1.5 vs 스프링 부트 2.0

     - 연동 방법이 크게 변경되었으나 설정 방법에는 큰 차이가 없다.

       `spring-security-oauth2-autoconfigure` 라이브러리를 사용하기 때문

       기존에 작성했던 코드 사용 가능

     - 킹치만 이 책에서 spring security Oauth2 client 라이브러리를 사용하는 이유

       1. 스프링 팀에서 기존 1.5에서 쓰던 `spring-security-oauth`프로젝트는 유지 상태로 결정함 신규 기능 추가 안하고 버그 수정 정도만 한다고 함. 새로운 기능은 oauth2 새 라이브러리에서만 지원
       2. 스프링 부트용 라이브러리 (starter) 출시됨
       3. 기존에 사용하던 방식은 확장 포인트가 적절하게 오픈되어 있지 않음. 그래서 직접 상속하거나 오버라이딩 해야 함. 신규 라이브러리는 확장 포인트를 고려해서 설계했음

     - 인터넷에서 스프링부트 2 방식의 자료 찾고 싶은 경우 확인해야 할 것

       1. `spring-security-oauth2-autoconfigure` 라이브러리를 썼는지 확인

       2. `application.properties` 혹은 `application.yml` 정보 확인

          > **Spring Boot 1.5**
          >
          > google : 
          >
          > ​	client :
          >
          > ​		clinetId : 인증 정보
          >
          > ​		accessTokenUri: ...	
          >
          > ​		...
          >
          > <u>url 주소를 모두 명시해야 함</u> 
          >
          > **Spring Boot 2.x**
          >
          > spring:
          >
          > ​	security:
          >
          > ​	oauth2:
          >
          > ​		client:
          >
          > ​			...
          >
          > <u>client 인증 정보만 입력해야 함. 직접 입력했던 값들은 enum으로 대체</u>
          >
          > `CommonOAuth2Provider`라는 enum이 새로 추가됨. 구글, 깃허브, 페이스북, 옥타의 기본 설정값은 모두 여기서 제공함

   5.2 구글 서비스 등록

   - 구글 서비스 등록을 할 때에는 인증 정보를 발급받아야 함

   - `application-oauth.properties`

     - `spring.security.oauth2.client.registration.google.scope=profile,email`로 지정해 주었는데, scope를 지정해주지 않으면 기본값이 `openid, profile, email`로 된다. 이 경우 `openid`때문에 Open Id Provider로 인식됨
     - 구글은 OpenId Provider이고 네이버, 카카오는 아니므로 openid를 포함시키면 두 서비스를 나누어서 각각 OAuth2Service를 만들어야 함
     - 준나 귀찮으므로 한번에 하기 위해서 scope를 지정해준다.

   - 스프링 부트에서는 properties의 이름을 `application-xxx.properties`로 만들면 xxx라는 이름의 profile이 생성되어서 이를 통해 관리 할 수 있음

     즉, profile=xxx라는 식으로 호출하면 해당 properties의 설정들을 가져올 수 있다. 

     호출 방식에는 여러가지가 있지만 이 책에서는 `application.properties`에서 `application-oauth.properties`를 포함하도록 구성한다.

   - 구글 로그인을 위한 클라이언트 ID와 비밀번호는 보안이 중요한 정보이므로 gitignore에 잊지 말고 꼭 추가해줘야 함

   5.3 구글 로그인 연동하기

   - `@Enumerated(EnumType.STRING)`

     - JPA로 데이터베이스로 저장할 때 Enum 값을 어떤 형태로 저장할지를 결정
     - 기본적으로는 int로 된 숫자가 저장되지만, 이러면 DB로 확인할 때 그 값이 무슨 코드를 의미하는 지 알 수 없음
     - 그래서 문자열로 저장하도록 선언한 것

   - 스프링 시큐리티에서는 권한 코드에 항상 `ROLE_`이 앞에 있어야 함

     → `ROLE_GUEST`, `ROLE_USER`

   - `UserRepository`

     `findByEmail` : 소셜 로그인으로 반환되는 값 중 email을 통해 이미 생성된 사용자인지 처음 가입하는 사용자인지 판단하기 위한 메소드

   - 의존성 추가 `spring-boot-starter-oauth2-client`

     `spring-boot-starter-oauth2-client`와 `spring-boot-starter-oauth2-jose`를 기본으로 관리해줌

   - `com/mingdyuo/book/springboot/config/auth/SecurityConfig.java`

     1. `@EnableWebSecurity`

        Spring Security 설정 활성화

     2. `csrf().disable().headers().frameOptions().disable()`

        h2-console 화면을 사용하기 위해 해당 옵션을 disable

     3. `authorizeRequests`

        - URL별 권한 관리를 설정하는 옵션의 시작점
        - authorizeRequests가 선언되어야 antMatchers 옵션 사용가능

     4. `antMatchers`

        - 권한 관리 대상을 지정하는 옵션
        - URL, HTTP 메소드별로 관리 가능
        - 여기서 `"/"` 등 지정된 URL들은 `permitAll()`옵션을 통해 전체 열람 권한을 줬음
        - 여기서 `"/api/v1/**"`주소를 가진 API는 USER 권한을 가진 사람만 가능

     5. `anyRequest`

        - 설정된 값들 이외 나머지 URL들을 나타냄
        - 여기서는 `authenticated()`를 추가해서 나머지 URL들은 모두 인증된 사용자에게만 허용하도록 함
        - 인증된 사용자는 로그인된 사용자를 의미

     6. `logout().logoutSuccessUrl("/")`

        - 로그아웃 기능에 대한 여러 설정의 진입점
        - 로그아웃 성공 시 `/` 주소로 이동

     7. `oauth2Login`

        OAuth 2 로그인 기능에 대한 여러 설정의 진입점

     8. `userInfoEndpoint`

        OAuth 2 로그인 성공 후 사용자 정보를 가져올 때의 설정 담당

     9. `userService`

        - 소셜 로그인 성공 시 후속 조치를 진행할 UserService 인터페이스의 구현체를 등록함
        - 리소스 서버(소셜 서비스)에서 사용자 정보를 가져온 상태에서 추가로 진행하고자 하는 기능을 명시할 수 있음.

   - `com/mingdyuo/book/springboot/config/auth/CustomOAuth2UserService.java`

     1. `registrationId`
        - 현재 로그인 진행 중인 서비스를 구분하는 코드
        - 지금은 구글만 사용하는 불필요한 값이지만 이후 네이버 로그인 연동 시에 구분하기 위해 사용
     2. `userNameAttributeName`
        - OAuth2 로그인 진행 시 키가 되는 필드값 의미. Primary Key와 같은 의미임
        - 구글의 경우 기본적으로 "sub"라는 코드를 지원하지만, 네이버와 카카오는 기본으로 지원하지 않음
        - 이후 네이버와 구글 로그인을 동시 지원할 때 사용함
     3. `OAuth Attributes`
        - OAuth2UserService를 통해 가져온 OAuth2User의 attribute를 담은 클래스
        - 이후 네이버 등 다른 소셜 로그인도 이 클래스를 사용함
     4. `SessionUser`
        - 세션에 사용자 정보를 저장하기 위한 Dto 클래스
        - **왜 User 클래스를 쓰지 않고 새로 만들어서 쓰는지 추후 설명**

   - `com/mingdyuo/book/springboot/config/auth/dto/OAuthAttributes.java`

     1. `of()`

        `OAuth2User`에서 반환하는 사용자 정보는 Map이기 때문에 값 하나하나를 변환해야 한다.

     2. `toEntity()`

        - User 엔티티를 생성
        - OAuthAttributes에서 엔티티 생성 시점은 처음 가입 시점
        - 가입할 때의 기본 권한을 GUEST로 주기 위해서 role 빌더 값에는 `Role.GUEST`를 사용
        - `OAuthAttributes` 클래스 생성이 끝났으면 같은 패키지에 SessionUser 클래스를 생성

   - `SessionUser`클래스에는 인증된 사용자 정보만 필요 

     User 클래스를 사용하지 않는 이유

     - User 클래스는 엔티티이기 때문에 다른 엔티티와 관계가 언제 형성될지 모른다.

       따라서 내부에 직렬화를 구현하기가 좀 그렇다

     - `@OneToMany`, `@ManyToMany` 등 자식 엔티티를 갖고 있다면, 직렬화 대상에 자식들이 포함된다. 따라서 성능 이슈와 부수 효과가 발생활 확률이 높음

     - 그래서 그냥 직렬화 기능을 가진 세션 Dto를 하나 추가로 만드는 게 운영과 유지보수 측면에서 낫다.

   - `src/main/resources/templates/index.mustache`

     1. `{{#userName}}`
        - 머스테치는 다른 언어와 같은 if문을 제공하지 않고 true/false 여부만 판단함
        - 따라서 항상 최종값을 넘겨줘야 함
        - userName이 있다면 userName을 노출시키도록 구성한 것
     2. `a href="/logout"`
        - 스프링 시큐리티에서 기본적으로 제공하는 로그아웃 URL
        - 개발자가 별도로 저 URL에 해당하는 컨트롤러를 만들 필요가 없음
        - `SecurityConfig` 클래스에서 URL을 변경할 수는 있지만, 기본 URL을 사용해도 충분하므로 여기서는 그대로 사용
     3. `{{^userName}}`
        - 머스테치에서 해당 값이 존재하지 않는 경우 `^`를 사용함
        - `userName`이 없다면 로그인 버튼을 노출시키도록 구성한 것
     4. `a href="/oauth2/authorization/google"`
        - 스프링 시큐리티에서 기본적으로 제공하는 로그인 URL
        - 로그아웃 URL과 마찬가지로 개발자가 별도의 컨트롤러를 생성할 필요가 없다.

   - `src/main/java/com/mingdyuo/book/springboot/web/IndexController.java`

     1. `(SessionUser) httpSession.getAttribute("user")`
        - 앞서 작성된 `CustomOAuth2UserService`에서 로그인 성공 시 세션에 SessionUser를 저장하도록 구성한 것
        - 즉, 로그인 성공 시 `httpSession.getAttribute("user")`에서 값을 가져올 수 있음
     2. `if(user != null)`
        - 세션에 저장된 값이 있을 때만 model에 userName으로 등록함
        - 세션에 저장된 값이 없으면 model엔 아무런 값이 없는 상태이니 로그인 버튼이 보이게 됨

   5.4 어노테이션 기반으로 개선하기

   - `src/main/java/com/mingdyuo/book/springboot/config/auth/LoginUser.java`
     1. `@Target(ElementType.PARAMETER)`
        - 이 어노테이션이 생성될 수 있는 위치를 지정
        - `PARAMETER`로 지정했으니, 메소드의 파라미터로 선언된 객체에서만 사용할 수 있음
        - 이 외에도 클래스 선언문에 쓸 수 있는 TYPE 등이 있음
     2. `@interface`
        - 이 파일을 어노테이션 클래스로 지정
        - LoginUser라는 이름을 가진 어노테이션이 생성되었다고 보면 됨
   - `src/main/java/com/mingdyuo/book/springboot/config/auth/LoginUserArgumentResolver.java`
     1. `supportsParameter()`
        - 컨트롤러 메서드의 특정 파라미터를 지원하는지 판단
        - 여기에는 파라미터에 `@LoginUser`  어노테이션이 붙어 있고, 파라미터 클래스 타입이 `SessionUser.class`인 경우 `true`를 반환
     2. `resolveArgument()`
        - 파라미터에 전달할 객체를 생성
        - 여기서는 세션에서 객체를 가져옴
   - `src/main/java/com/mingdyuo/book/springboot/web/IndexController.java`
     1. `@LoginUser SessionUser user`
        - 기존에 `(User) httpSession.getAttribute("user")`로 가져오던 세션 정보 값을 개선
        - 어느 컨트롤러든 `@LoginUser`만 사용하면 세션 정보를 가져올 수 있음

   5.5 세션 저장소로 데이터베이스 사용하기

   - 우리가 만든 서비스는 세션이 내장 톰캣의 메모리에 저장

     - 애플리케이션을 재실행하면 로그인이 풀림
     - 세션은 WAS(Web Application Server)의 메모리에서 저장되고 호출됨
     - 애플리케이션 실행 시 실행되는 구조에서는 세션 초기화가 됨
     - 즉, 배포할 때마다 톰캣이 재시작 되는 것
     - 2대 이상의 서버에서 서비스 한다면, 톰캣마다 세션 동기화 설정을 해야 함

   - 현업에서 사용하는 세션 저장소

     1. 톰캣 세션

        - 일반적으로 별다른 설정 하지 않을 시 기본적으로 사용
        - 톰캣(WAS)에 세션이 저장되므로 2대 이상의 WAS가 구동되는 환경에서는 세션 공유를 위한 추가 설정이 필요

     2. MySQL과 같은 DB

        - 여러 WAS 간의 공용 세션을 사용할 수 있는 가장 쉬운 방법

        - 많은 설정이 필요 없으나, 로그인 요청마다 DB IO가 발생

          → 성능상 이슈 발생 가능

        - 보통 로그인 요청이 많이 없는 백오피스, 사내 시스템 용도에서 사용

     3. Redis, Memcached와 같은 메모리 DB

        - B2C 서비스에서 가장 많이 사용하는 방식
        - 실제 서비스로 사용하기 위해서는 Embedded Redis와 같은 방식이 아닌 외부 메모리 서버가 필요

   - 이 책에서 사용할 세션 저장소는 2번 선택지, 그 이유는

     - 설정이 간단함
     - 사용자가 많은 서비스가 아님
     - 비용 절감

   - AWS에서 서비스를 배포, 운영할 때를 생각하면 Redis같은 메모리 DB는 별도 사용료를 지불해야 하므로 부담스러움

   5.6 네이버 로그인

   - `src/main/resources/application-oauth.properties`

     1. ` user_name_attribute=response`
        - 기준이 되는 `user_name`의 이름을 네이버에서는 response로 해야 함
        - 왜냐면 네이버의 회원 조회 시 반환되는 JSON 형태 때문

   - 스프링 시큐리티에서는 하위 필드를 명시할 수 없고, 최상위 필드만 user_name으로 지정 가능

     네이버의 응답값 최상위 필드는 `resultCode, message, response` 세개가 전부이므로 `response`를 `user_name`으로 지정하고 받아오는 것

   5.7 기존 테스트에 시큐리티 적용하기

   - 기존 테스트에 시큐리티 적용으로 문제가 되는 부분

     - 기존에는 바로 API를 호출할 수 있으므로, 테스트 코드 역시 바로 API를 호출하도록 구성함

       하지만 시큐리티 옵션이 활성화 되면 인증된 사용자만 API를 호출할 수 있음

     - 기존 API 테스트 코드들이 모두 인증에 대한 권한을 받지 못하였으므로, 테스트 코드마다 인증한 사용자가 호출한 것처럼 작동하도록 수정해야 함

     - `src/main `환경과 `src/test` 환경은 각자만의 환경 구성을 가짐. `test`에 `application.properties`가 없으면 자동으로 가져오는데, 자동으로 가져오는 옵션의 범위는 저것 까지임. `application-oauth.properties`는 자동으로 가져오지 않음. 

     - 따라서 테스트 환경을 위해서 `application.properties`를 만들어줘야 한다. 이 때에는 가짜 설정값을 등록한다.

   - `src/test/java/com/mingdyuo/book/springboot/web/PostsApiControllerTest.java`

     1. `@WithMockUser(roles="USER")`

        - 인증된 모의 사용자를 만들어서 사용

   - roles에 권한 추가 가능

     - `ROLE_USER`권한을 가진 사용자가 API를 요청하는 것과 동일한 효과

     2. `@Before`

        매번 테스트가 시작되기 전에 `MockMvc` 인스턴스를 생성

     3. `mvc.perform`

        - 생성된 MockMvc를 통해 API를 테스트
        - 본문 영역은 문자열로 표현하기 위해 ObjectMapper를 통해 문자열 JSON으로 변환

   - `@WebMvcTest`는 CustomOAuth2UserService를 스캔하지 않음

     - `@ControllerAdvice`, `@Controller`는 읽는다.
     - `@Repository`, `@Service`, `@Component`는 스캔하지 않는다.

   - `@EnableJpaAuditing`을 사용하기 위해서는 최소 하나의 `@Entity` 클래스가 필요함

     `@WebMvcTest`에는 없음

   - `@EnableJpaAuditing`는 `@SpringBootApplication`과 함께 있으므로 `@WebMvcTest`에서도 스캔함. → 둘을 분리해야함



6. **AWS 서버 환경을 만들어보자 - AWS EC2**

   24시간 작동하는 서버 환경을 만드려면

   > 1. 집에 있는 PC를 24시간 구동
   >
   >    - 일반적으로 비용이 저렴
   >
   > 2. 호스팅 서비스(cafe24, 코리아호스팅 등)을 이용
   >
   >    - 일반적으로 비용이 저렴
   >
   > 3. 클라우드 서비스(AWS, AZURE, GCP 등)을 이용
   >
   >    - 유동적으로 사양을 늘릴 수 있음, 특정 시간에만 트래픽이 몰림
   >
   >    - 단순히 물리 장비만 대여하는 것이 아님
   >
   >      서버 내부의 로그 관리, 모니터링, 하드웨어 교체, 네트워크 관리 등을 지원

   클라우드의 형태

   > 1. Infrastructure as a Service (IaaS, 아이아스, 이에스)
   >    - 기존 물리 장비를 미들웨어와 함께 묶어둔 추상화 서비스
   >    - 가상 머신, 스토리지, 네트워크, 운영체제 등의 IT 인프라를 대여해 주는 서비스
   >    - AWS의 EC2, S3 등
   > 2. Platform as a Service (PaaS, 파스)
   >    - 앞에서 언급한 IaaS에서 한번 더 추상화한 서비스
   >    - 따라서 많은 기능이 자동화되어 있음
   >    - AWS의 Beanstalk(빈스톡), Heroku(헤로쿠) 등
   > 3. Software as a Service (SaaS, 사스)
   >    - 소프트웨어 서비스
   >    - 구글 드라이브, 드랍박스, 와탭 등

   6.1 AWS 회원 가입

   6.2 EC2 인스턴스 생성하기

   - EIP 할당
     - 인스턴스 중지하고 다시 시작할 때마다 IP가 새로 할당된다. 이렇게 되면 매번 접속해야 하는 IP 바뀌어서 매우 번거로우므로 고정 IP를 할당한다.
     - AWS에서 고정 IP를 Elastic IP(EIP, 탄력적 IP)라고 한다.

   6.3 EC2 서버에 접속하기

   6.4 아마존 리눅스 1 서버 생성 시 꼭 해야 할 설정들

   - 필수로 해야 하는 설정

     1. Java 8 설치

     2. 타임존 변경

        기본 서버의 시간은 미국 시간대임. 한국 시간대가 되어야 우리가 사용하는 시간이 모두 한국 시간으로 등록되고 사용됨. 

        ```
        sudo rm /etc/localtime
        sudo ln -s /usr/share/zoneinfo/Asia/Seoul /etc/localtime
        ```

     3. 호스트네임 변경

        현재 접속한 서버의 별명을 등록함. 실무에서는 수십 대의 서버가 작동됨. IP만으로는 서버의 각 역할을 알 수 없으므로 호스트 네임을 필수로 등록한다.

        `sudo vim /etc/sysconfig/network`에서 `HOSTNAME=원하는 호스트네임`을 입력하고 저장한다.

        [호스트 네임 미등록시 발생할 수 있는 이슈](https://woowabros.github.io/experience/2017/01/20/billing-event.html)

7. AWS에 데이터베이스 환경을 만들어보자 - AWS RDS

   - 웹 백엔드에서 애플리케이션 코드만큼 데이터베이스 다루는 것이 중요하다.
   - 어느 정도의 DB 구축, 쿼리 튜닝에 대한 기본적 지식이 필요함
   - 이 책에서는 직접 DB를 설치하지는 않고, AWS에서 지원하는 관리형 서비스인 RDS(Relational Database Service)를 이용한다.

   7.1 RDS 인스턴스 생성하기

   - MariaDB 추천 이유

     1. 가격이 저렴함

     2. Amazon Aurora 교체가 용이함

        Aurora는 AWS에서 MySQL과 PostGreSQL을 클라우드 기반에 맞게 재구성한 데이터 베이스임. 호환이 잘되고 발전하고 있음. 추후에 이 Aurora로 바꾸기가 좋다. (Aurora는 비싸기 때문에 이걸로 시작하지는 않음)

   7.2 RDS 운영환경에 맞는 파라미터 설정하기

   - RDS 생성 시 해야 하는 설정
     1. 타임존
     2. Character Set
     3. Max Connection

   7.3 내 PC에서 RDS에 접속해보기

   - 보안 그룹에 IP와 EC2 추가하면 EC2와 RDS간에 접근 가능
   - EC2는 2, 3대가 될 수도 있는데, 매번 IP를 등록할 수는 없으니 보편적으로 보안 그룹 간에 연동을 진행함
   - RDS 엔드 포인트 : 접근 가능한 URL

   7.4 EC2에서 RDS에서 접근 확인

   

8. EC2서버에 프로젝트를 배포해 보자

   8.1 EC2에 프로젝트 Clone 받기

   8.2 배포 스크립트 만들기

   - 배포의 의미
     1. git clone 혹은 git pull을 통해서 새 버전의 프로젝트를 받음
     2. Gradle이나 Maven을 통해 프로젝트 테스트와 빌드
     3. EC2 서버에서 해당 프로젝트 실행 및 재실행
   - 쉘 스크립트
     - 배포할 때마다 하나하나 명령어를 실행할 필요 없도록 해주는 프로그램 파일
     - `.sh`확장자 가짐
   - `~/app/step1/deploy.sh`
     1. `REPOSITORY=/homeec2-user/app/step1`
        - 프로젝트 디렉토리 주소는 스크립트 내에서 자주 사용하는 값이므로 변수로 저장
        - `PROJECT_NAME=springboot2-webservice`도 동일하게 변수로 저장
        - 쉘에서는 타입 없이 선언 및 저장
        - 쉘에서는 `$변수명`으로 변수를 사용할 수 있음
     2. `cd $REPOSITORY/$PROJECT_NAME/`
        - 제일 처음 `git clone` 받았던 디렉토리로 이동
        - 바로 위의 쉘 변수 설명을 따라 `/home/ec2-user/app/step1/springboot2-webservice` 주소로 이동
     3. `git pull`
        - 디렉토리 이동 후, master branch의 최신 내용을 받음
     4. `./gradlew build`
        - 프로젝트 내부의 gradlew로 build를 수행
     5. `cp ./build/libs/*jar $REPOSITORY/`
        - build의 결과물인 jar 파일을 복사해 jar 파일을 모아둔 위치로 복사
     6. `CURRENT_PID=$(pgrep -f springboot_webservice)`
        - 기존에 수행 중이던 스프링 부트 애플리케이션을 종료
        - pgrep은 process id만 추출하는 명령어
        - `-f` 옵션은 프로세스 이름으로 찾는 것
     7. `if ~ else ~ fi`
        - 현재 구동중인 프로세스가 있는지 없는지 판단
        - process id 값을 보고 프로세스가 있으면 종료
     8. `JAR_NAME=$(ls -tr $REPOSITORY/ | grep jar | tail -n 1)`
        - 새로 실행할 jar 파일명을 찾음
        - 여러 jar 파일이 생기기 때문에 `tail -n`로 가장 나중의 jar 파일(최신 파일)을 변수에 저장
     9. `nohup java -jar $REPOSITORY/$JAR_NAME 2>&1 &`
        - 찾은 jar 파일명으로 해당 jar 파일을 nohup으로 실행
        - 스프링 부트의 장점으로, 외장 톰캣을 설치할 필요가 없음
        - 내장 톰캣을 사용해서 jar 파일만 있으면 바로 웹 애플리케이션 서버 실행 가능
        - 일반적으로 자바 실행 시 `java -jar`라는 명령어를 사용하지만, 이렇게 실행하면 사용자가 터미널 접속을 끊을 때 애플리케이션도 같이 종료됨
        - `nohup` 명령어 사용 시 애플리케이션 실행자가 터미널을 종료해도 애플리케이션이 계속 구동

   8.3 외부 Security 파일 등록하기

   - `~/app/step1/deploy.sh`
     1. `-Dspring.config.location`
        - 스프링 설정 파일 위치를 지정
        - 기본 옵션들을 담고 있는 `application.properties`와 OAuth 설정들을 담고 있는 `application-oauth.properties`의 위치를 지정
        - `classpath`가 붙으면 jar 안에 있는 `resources` 디렉토리를 기준으로 경로 생성
        - `application-oauth.properties`는 외부에 파일이 있기 때문에 절대 경로를 사용함 

   8.4 스프링부트 프로젝트로 RDS 접근하기

   - MariaDB에서 스프링부트 프로젝트를 실행하기 위해 필요한 작업
     1. RDS 테이블 생성
        - H2에서는 테이블을 자동 생성해주었음
        - MariaDB에서는 직접 쿼리를 이용해 생성해야 함
        - 여기에서는 JPA가 사용될 엔티티 테이블과 스프링 세션이 사용될 2종류 테이블을 생성
        - JPA가 사용할 테이블은 테스트 코드 수행시 로그로 생성되는 쿼리를 사용하면 됨
     2. 프로젝트 설정
        - 자바 프로젝트가 MariaDB에 접근하려면 데이터베이스 드라이버가 필요
        - MariaDB에서 사용 가능한 드라이버를 프로젝트에 추가
     3. EC2 (리눅스서버) 설정
        - DB의 접속 정보는 중요하게 보호해야 할 정보임
        - 공개되면 외부에서 데이터를 모두 가져갈 수 있음
        - 프로젝트 내에 접속정보가 있다면, 해킹 위험이 있으므로 EC2 서버 내부에서 접속 정보를 관리하도록 설정해야 함
   - `application-real.properties`로 파일 만들면 실제 운영할 `profile=real`인 환경이 구성된다고 보면 된다. 보안/로그 상 이슈가 될 만할 설정을 모두 제거하는 RDS 환경 profile을 설정한다. 
   - `application-real-db.properties`
     1. `spring.jpa.hibernate.ddl-auto=none`
        - JPA로 테이블이 자동 생성되는 옵션을 None으로 지정 → 자동으로 생성하지 않음
        - RDS에는 실제 운영으로 사용될 테이블이므로 절대 스프링부트에서 새로 만들지 않도록 한다.
        - 이 옵션을 하지 않으면 자칫 테이블이 모두 새로 생성될 수 있음
        - 주의해야 하는 옵션임
   - `deploy.sh`
     1. `-Dspring.profiles.active=real`
        - `application-real.properties`를 활성화
        - `application-real.properties`의 `spring.profiles.include=oauth,real-db` 옵션 때문에 `real-db` 역시 활성화 대상에 포함됨

   8.5 EC2에서 소셜 로그인하기

   - AWS 보안 그룹 변경
   - AWS EC2 도메인으로 접속
   - 구글에 EC2 주소 등록

