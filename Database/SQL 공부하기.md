## SQL 공부하기

##### Reference

[SQL 독학 강의](https://stricky.tistory.com/202)

<br>

### SQL이란

- 관계형 데이터베이스에서 데이터를 조작하고 쿼리하는 표준 언어이다.
- RDBMS에게 사용자가 원하는 것을 요청하는 방법
- RDBMS의 종류 : 오라클, MySql, mariaDB, ms-sql, postgresql, greenplum

강의는 ANSI SQL 문법을 토대로 진행



### SQL 명령어의 종류

1. DDL (Data **Definition** Language, 데이터 정의 언어)

   📢 여기서 쓰이는 개체란 테이블, 인덱스 제약 조건, 프로시져, 펑션 등을 의미한다 !

   | 명령어   | 사용                                            |
   | -------- | ----------------------------------------------- |
   | CREATE   | 데이터베이스 내 개체을 생성할 때                |
   | DROP     | 데이터베이스 내 개체를 삭제할 때                |
   | ALTER    | 데이터베이스 내 개체의 속성 및 정의를 변경할 때 |
   | RENAME   | 데이터베이스 내 개체의 이름을 변경할 때         |
   | TRUNCATE | 테이블 내 모든 데이터를 빠르게 삭제할 때        |

   ```sql
   CREATE TABLE My_table(
       my_field1 INT,
       my_field2 VARCHAR(50),
       my_field3 DATE NOT NULL,
       PRIMARY KEY (my_field1, my_field2)
   );
   
   ALTER TABLE My_table ADD my_field4 NUMBER(3) NOT NULL;
   
   DROP TABLE My_table;
   
   TRUNCATE TABLE My_table;
   
   ALTER TABLE My_table RENAME TO Your_table;
   ```

2. DML (Data **Manipulation** Language, 데이터 조작 언어)

   | 명령어 | 사용                                                         |
   | ------ | ------------------------------------------------------------ |
   | INSERT | 특정 테이블에 데이터를 신규로 삽입할 때                      |
   | UPDATE | 특정 테이블 내 데이터의 전체, 또는 일부를 새로운 값으로 갱신할 때 |
   | DELETE | 특정 테이블 내 데이터의 전체, 또는 일부를 삭제할 때          |
   | SELECT | 특정 테이블 내 데이터의 전체 또는 일부를 획득할 때           |

   ```sql
   INSERT INTO My_table([column list]) VALUES ([value list]);
   
   INSERT INTO My_table VALUES ([value list]);
   
   UPDATE My_table SET My_field2 = '갱신할 값' WHERE...;
   
   DELETE FROM My_table WHERE...;
   
   SELECT [Column list] FROM My_table WHERE...;
   ```

3. DCL (Data **Control** Language, 데이터 제어 언어)

   단독으로 사용되는 경우가 많다.

   | 명령어          | 사용                                                      |
   | --------------- | --------------------------------------------------------- |
   | GRANT           | 데이터베이스 사용자에게 특정 작업의 수행 권한을 부여할 때 |
   | REVOKE          | 데이터베이스 사용자에게 부여된 수행 권한을 박탈할 때      |
   | SET TRANSACTION | 트랜잭션 모드로 설정할 때                                 |
   | BEGIN           | 트랜잭션의 시작을 의미                                    |
   | COMMIT          | 트랜잭션을 실행할 때                                      |
   | ROLLBACK        | 트랜잭션을 취소할 때                                      |
   | SAVEPOINT       | 롤백 지점을 설정할 때                                     |
   | LOCK            | 테이블 자원을 점유할 때                                   |

   ```sql
   GRANT SELECT ON SCOTT.EMP TO EXPERT
   
   REVOKE [권한] ON [객체 이름] FROM [사용자]
   ```

<br>

### SQL 공부 요령

1. 문법을 공부하기

   SQL 도서를 고르고 순차적으로 익히기

2. 직접 SQL을 작성, 실행해보기

   다양한 케이스를 응용, 연습하며 익히기

3. 다른 사람의 SQL 구문을 확인 해보기

   다른 사람의 SQL과 데이터를 자신의 연습용 DB에 넣어 실행해보고, 왜 그런 결과가 나왔는지, 어떤 테이블끼리 조인 되었는지 등 궁금증을 가지고 확인해보기

4. 데이터베이스에 관해 이해하기

   SQL과 별개로 데이터베이스 자체를 알아야 활용할 수 있다.

5. 함수 사용법 익히기

   여러 RDBMS에서는 다양한 함수를 지원함. 이를 잘 사용하면 SQL을 더 유연하게 사용 가능. 하지만 RDBMS마다 지원하는 함수가 다르므로 내가 사용할 환경에 따라 지원하는 함수를 미리 숙지하도록 하자.

<br>

### SELECT를 학습하기 앞선 용어 정리

- 데이터베이스에서 표(테이블)의 표현 방법은 row, column을 이용하는 것이다.
- 테이블에서 데이터를 가져오는 방법은 두 가지이다.
  1. **[Projection]** Column만 가지고 오는 방법
  2. **[Selection]** Row만 가지고 오는 방법
- \* : 아스타라고 부르며 모든 칼럼을 의미함.

<br>

### SELECT

- 가장 기본적인 명령어.
- 다른 모델링과 요건, 정의, 업무 분석은 모두 select를 잘하기 위한 것이므로 중요하다.

1. DESC 사용하기

   *Describe*의 약어로, 특정 테이블에 어떤 칼럼이 있는지 조회하는 명령어이다. 

   ```sql
   create table select_test(
   	...
   ) character set utf8; 
   # 이 부분은 테이블에 한글 데이터가 들어갈 수 있도록 캐릭터 셋을 잡아주는 명령어이다.
   # 어떤 캐릭터 셋으로 데이터베이스가 설치되었는지 모를때만 쓰면 됨
   
   desc select_test;
   ```

2. 원하는 데이터만 select 하기

   ```sql
   select * from select_test;
   ```

3. WHERE 조건절 사용하기

   많은 row들 가운데 특정한 조건을 입력하여, 그에 해당하는 데이터만 가져오도록 한다. 

4. 정리

   ```sql
   SELECT [출력할 컬럼 명] FROM [데이터가 있는 테이블 명] WHERE [출력할 데이터 조건]
   ```

5. 표현식 사용하기

   - 칼럼의 데이터 외에 다른 문자열이나 내용을 출력

   - 다음 예시에서는 '님 안녕하세요!!'라는 리터럴 상수가 출력된다.

     ```sql
     select name, '님 안녕하세요!!' from test.select_test;
     ```

6. 컬럼에 별칭(alias) 붙이기

   ```sql
   select name 이름, '님 안녕하세요!!' 인사문구 from test.select_test;
   select name as 이름, '님 안녕하세요!!' as 인사문구 from test.select_test;
   # 둘 다 동일한 기능이지만 가독성을 위해 후자를 쓰자
   ```

7. DISTINCT로 중복된 값을 제외하고 출력하기

   말 그대로 중복 없이 집합 형태로 unique한 원소를 모아서 출력하는 기능이다.

   ```sql
   select distinct dept_cd from test.select_test;
   ```

8. 연결 연산자 함수로 칼럼 값을 붙혀서 출력하기.

   여러 칼럼의 값이나, 문자열로 된 표현식을 붙혀서 하나의 column으로 표현할 수 있다.

   ```sql
   # 연결 연산자 함수 concat(something, something)
   select concat(name,'의 부서코드는 ',dept_cd,' 입니다.') from test.select_test;
   # oracle의 연결 연산자
   select name||'의 부서코드는 '||dept_cd||' 입니다.' from test.select_test;
   ```

   ```sql
   # 이름이 엄준식인 사람의 부서코드 찾아서 출력하기
   select concat(name,'의 부서코드는 ',dept_cd,' 입니다.') 
   from test.select 
   where name = '엄준식';
   ```

9. 산술 연산자 사용해보기

   `+, -, *, /`를 이용한다.

   - 총합 구하기

     ```sql
     select name, math, english, math + english + korean as total 
     from test.exam_result;
     ```

   - 평균 구하기

     ```sql
     select name, math, english, (math + english + korean)/3 as avg 
     from kmong.exam_result;
     ```

10. where절에 비교 연산자를 사용해 보기

    - 사용 가능한 연산자

      | 비교 연산자           | 설명                                     |
      | --------------------- | ---------------------------------------- |
      | =                     | 같은 조건을 검색                         |
      | !=, <>                | 같지 않은 조건을 검색                    |
      | >, >=, <, <=          | 크기에 따라 검색                         |
      | BETWEEN a AND b       | a, b 사이에 있는 값을 검색               |
      | IN(a, b, c)           | a, b, c 중 어느 하나인 것을 검색         |
      | like                  | 특정 패턴을 가지고 있는 조건을 검색      |
      | is Null / is Not Null | NULL인 값이나 아닌 값을 검색             |
      | a AND b               | a, b 두 조건 모두를 만족하는 값을 검색   |
      | a OR b                | a, b 중 하나의 조건을 만족하는 값을 검색 |
      | NOT a                 | a가 아닌 모든 값을 검색                  |

11. order by 절을 사용하여 정렬된 출력하기

    - select, from, where과 같이 키워드로 분류된다.
    - 기본적으로 오름차순임, 내림차순 정렬시 desc 옵션 사용

    ```sql
    select * from test.exam_result order by math desc;
    # sql 실행 결과의 3번째 컬럼을 기준으로 정렬 (권장하지 않음)
    select * from test.exam_result order by 3;
    ```

12. 집합 연산자 사용하기

    - 데이터베이스에서 한 테이블을 집합이라고 한다.
    - SQL에서 하나의  select문으로 나오는 데이터 셋을 집합이라고 표현한다.

    | 집합 연산자 | 설명                                                         |
    | ----------- | ------------------------------------------------------------ |
    | UNION       | 두 집합을 더해서 결과를 출력. 중복 제거와 정렬을 수행        |
    | UNION ALL   | 두 집합을 더해서 결과를 출력. 중복 제거와 정렬을 하지 않음   |
    | INTERSECT   | 두 집합의 교집합 결과를 정렬하여 출력                        |
    | MINUS       | 두 집합의 차집합 결과를 정렬하고 출력. SQL의 순서가 중요하다. |



![image](https://user-images.githubusercontent.com/41130448/107183524-4bea2300-6a22-11eb-941f-d41afeb2e140.png)

