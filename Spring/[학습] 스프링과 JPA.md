## JPA

- 기존에 **데이터 베이스 연동**을 위해 사용하던 기술들은

  - JDBC, 스프링 DAO, MyBatis, Hibernate(ORM) 등이 있음
  - 이중에서 **hibernate**같은 ORM은 애플리케이션에서 사용하는 SQL도 제공함. 개발자의 할일을 많이 줄여준다.
  - 여기서 또 ORM을 더 쉽게 사용할 수 있도록 표준화 한것이 바로 JPA (Java Persistence API)이다.

- Spring Data JPA

  - JPA와 다른 것임!

  - 스프링 부트에서 JPA를 쉽게 사용할 수 있도록 지원하는 모듈
  - 이걸 쓰면 JPA를 사용하는 데 필요한 라이브러리나 XML 설정을 신경쓰지 않아도 된다.
  - JPA의 개념이나 동작원리를 몰라도 사용 가능

