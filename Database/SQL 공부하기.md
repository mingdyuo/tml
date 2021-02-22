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
   -- 이 부분은 테이블에 한글 데이터가 들어갈 수 있도록 캐릭터 셋을 잡아주는 명령어이다.
   -- 어떤 캐릭터 셋으로 데이터베이스가 설치되었는지 모를때만 쓰면 됨
   
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
   -- 둘 다 동일한 기능이지만 가독성을 위해 후자를 쓰자
   ```

7. DISTINCT로 중복된 값을 제외하고 출력하기

   말 그대로 중복 없이 집합 형태로 unique한 원소를 모아서 출력하는 기능이다.

   ```sql
   select distinct dept_cd from test.select_test;
   ```

8. 연결 연산자 함수로 칼럼 값을 붙혀서 출력하기.

   여러 칼럼의 값이나, 문자열로 된 표현식을 붙혀서 하나의 column으로 표현할 수 있다.

   ```sql
   -- 연결 연산자 함수 concat(something, something)
   select concat(name,'의 부서코드는 ',dept_cd,' 입니다.') from test.select_test;
   -- oracle의 연결 연산자
   select name||'의 부서코드는 '||dept_cd||' 입니다.' from test.select_test;
   ```

   ```sql
   -- 이름이 엄준식인 사람의 부서코드 찾아서 출력하기
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
    -- sql 실행 결과의 3번째 컬럼을 기준으로 정렬 (권장하지 않음)
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

<br>

### SQL 함수

- DBMS의 함수 

  [정의 주체에 따른 분류]

  1. 내장 함수

     RDBMS에 내장되어 있는 함수

  2. 사용자 정의 함수

     내장함수를 제외하고 `create function`문을 사용해서 필요한 변환 규칙을 적용해 사용자가 직접 만든 함수

  [실행에 따른 분류]

  1. 단일행 함수

     한 행 값을 받아서 특정 규칙과 정의를 통해 변환시키는 함수

  2. 복수행 함수

     여러 행의 값을 한꺼번에 받아서 하나의 행의 결과 값으로 되돌려주는 함수

     예) count()

  [기능에 따른 분류]

  1. 문자 함수
  2. 숫자 함수
  3. 날짜 함수
  4. 형 변환 함수
  5. 일반 함수

<br>

### 내장, 단일행, 문자 함수를 보자

1. `lower( )`, `upper( )`

   소문자, 대문자로 출력

2. `length( )`

   데이터의 길이

3. `concat( )`

   mysql에서 문자열이나 컬럼 값을 출력하는 방법

   오라클에서는 `||`연산자로 이어붙일 수 있음

4. `substr( )`, `mid( )`, `substring( )`

   셋 다 똑같은 기능이다. `substr([컬럼명], [시작할 문자열의 위치 값], [리턴시킬 값의 길이])`와 같이 사용

   시작할 문자열의 인덱스는 0이 아니라 1부터 시작한당,,ㅎㅎ

   리턴시킬 값의 길이가 시작한 위치부터 끝까지의 길이보다 짧으면 끝까지만 리턴한다.

   ```sql
   select continent as 원본, substr(continent, 2, 2) as substr
   from test.country;
   ```

5. `instr( )`

   특정 문자열의 위치를 숫자로 리턴. `instr([컬럼명], '찾는 문자')`로 사용

   찾는 문자가 없으면 0이 나온다. 여러 개 있으면 가장 앞에 있는 찾는 문자가 나온다.
   
   *대소문자 구분을 하지 않는다!*

   ```sql
   select continent as 원본, instr(continent,'A') as instr from test.country;
   ```
   
6. `lpad( )`, `rpad( )`

   데이터가 어떤 기준보다 짧을 경우에 왼쪽이나 오른쪽으로 기준만큼 자릿수를 채워준다.

   `lpad([컬럼명], [기준 자릿수], [채워넣을 숫자 혹은 문자])`로 사용

   ```sql
   select lpad(continent, 7, 'X') as lpad, rpad(continent, 9, 'O') as rpad
   from test.country;
   ```

7. `trim( )`, `ltrim( )`, `rtrim( )`

   문자열의 양쪽, 왼쪽, 오른쪽의 공백을 없애는 함수

   `trim([컬럼 명])`으로 사용한다.

   ```sql
   select capital_city as 원본, trim(capital_city) as trim, ltrim(capital_city) as ltrim
   from test.country;
   ```

8. `replace( )`

   특정 문자열을 찾아서 다른 문자열로 치환하는 함수

   `replace([컬럼 명], '찾을 문자', '치환할 문자')`와 같이 사용

   *대소문자를 구분한다.*

   ```sql
   select replace(capital_city, 'a', '@') as 'replace'
   from test.country;
   ```

<br>

### 내장, 단일행, 숫자 함수를 보자

1. `round( )`

   숫자를 **반올림** 후 출력하는 함수

   옵션을 통해 반올림 자릿수 설정 가능

   `round([숫자], [소수점 아래 자릿수])`와 같이 사용

   ```sql
   select round(112.3456,1),round(112.3456,2),round(112.3456,-1) from dual;
   ```

2. `truncate( )`

   `round( )`와 사용법이 같다.

   입력된 값을 옵션에 따라 지정된 위치에서 **버리고** 결과를 출력한다.

   오라클에서는 `trunc( )`함수로 사용

   ```sql
   select truncate(112.3456,1),truncate(112.3456,2),truncate(112.3456,-1) from dual;
   ```

3. `mod( )`

   나머지를 출력하는 함수

   `mod([나눌 값], [몫])`와 같이 사용한다.

   ```sql
   select mod(26,3),mod(10,9),mod(4,2) from dual;
   ```

4. `ceil( )`

   입력된 숫자보다 크면서, 가장 가까운 정수를 출력

   ```sql
   select ceil(12.6),ceil(11.5),ceil(16.3) from dual;
   ```

5. `floor( )`

   입력된 숫자보다 작으면서, 가장 가까운 정수를 출력

   ```sql
   select floor(12.6), floor(11.5), floor(16.3) from dual;
   ```

6. `power( )`

   제곱하는 함수

   ```sql
   select power(1,2),power(2,3),power(3,5) from dual;
   ```

<br>

![image](https://user-images.githubusercontent.com/41130448/107464338-6c8db680-6ba3-11eb-9b58-96b0180f6bf4.png)

![image](https://user-images.githubusercontent.com/41130448/107464720-3997f280-6ba4-11eb-9588-e978db50d755.png)

<br>아 개재미없다...

### 내장, 단일행, 날짜 함수

1. 지금 현재의 날짜, 시간 출력하기

   - 사용할 수 있는 SQL 명령이 여러가지가 있다.

     | SQL                           | 결과                  |
     | ----------------------------- | --------------------- |
     | `select now();`               | `2021-02-14 15:02:23` |
     | `select sysdate();`           | `2021-02-14 15:02:38` |
     | `select current_timestamp();` | `2021-02-14 15:02:48` |
     | `select curdate();`           | `2021-02-14`          |
     | `select current_date();`      | `2021-02-14`          |
     | `select current_time();`      | `15:03:33`            |

   - `+0`이라는 연산을 추가하면 형식을 빼고, 숫자를 나열한 형태로 출력할 수 있다.

     | SQL                        | 결과             |
     | -------------------------- | ---------------- |
     | `select now() + 0;`        | `20210214150359` |
     | `select current_time()+0;` | `150415`         |

2. 날짜, 시간에 따른 특정 정보 출력하기

   | SQL                                         | 결과     | 설명                          |
   | ------------------------------------------- | -------- | ----------------------------- |
   | `select dayofweek('2021-02-14 15:02:48');`  | 1        | 1:일요일, 2:월요일...7:토요일 |
   | `select weekday('2021-02-14 15:02:48');`    | 6        | 0:월요일, 1:화요일...6:일요일 |
   | `select dayofmonth('2021-02-14 15:02:48');` | 14       | 일자를 출력                   |
   | `select dayofyear('2021-02-14 15:02:48');`  | 45       | 한해의 몇번째 날인지 출력     |
   | `select month('2021-02-14 15:02:48');`      | 2        | 월을 출력                     |
   | `select dayname('2021-02-14 15:02:48');`    | Sunday   | 요일을 영문으로 출력          |
   | `select monthname('2021-02-14 15:02:48');`  | February | 월을 영문으로 출력            |
   | `select quarter('2021-02-14 15:02:48');`    | 1        | 분기를 출력 (1분기~4분기)     |
   | `select week('2021-02-14 15:02:48');`       | 7        | 한해의 몇번째 주인지 출력     |
   | `select year('2021-02-14 15:02:48');`       | 2021     | 년도를 출력                   |
   | `select hour('2021-02-14 15:02:48');`       | 15       | 시간을 출력                   |
   | `select minute('2021-02-14 15:02:48');`     | 2        | 분을 출력                     |

3. 날짜, 시간을 연산하여 출력하기

   `date_add([날짜], interval [더할 값] [더할 값의 날짜 단위])`

   `date_sub([날짜], interval [더할 값] [더할 값의 날짜 단위])`

   `adddate([날짜], interval [더할 값] [더할 값의 날짜 단위])`

   `subdate([날짜], interval [더할 값] [더할 값의 날짜 단위])`

   - 날짜 단위

     | type변수 (날짜 단위)                             | 입력 형태                  |
     | ------------------------------------------------ | -------------------------- |
     | second, minute, hour, day, month, year           | 초, 분, 시, 일, 월 년      |
     | minute_second, hour_minute, day_hour, year_month | 분:초, 시:분, 일 시, 년 월 |
     | hour_second, day_minute                          | 시:분:초, 일 시:분         |
     | day_second                                       | 일 시:분:초                |

4. 시간과 초 데이터 변환하여 출력하기

   `select sec_to_time(3000);`

   `select time_to_sec('20:21:20');`

5. 년월에 데이터에 개월을 더해서 출력하기

   YYMM 형태 : `select period_add(2001, 15);` : 2020년 01월 + 15월

   YYYYMM 형태 : `select period_add(202001, 15);` : 2020년 01월 + 15월

6. 파라미터를 조정하여 데이터를 변경하여 출력하기

   `select date_format('date', 'format');`과 같이 사용

   ```sql
   select date_format('2021-02-02 11:11:11', '%p');
   select date_format('2021-02-02 11:11:11', '%w');
   select date_format('2021-02-02 11:11:11', '%u')
   ```

   | format | 출력 형태                                                    |
   | ------ | ------------------------------------------------------------ |
   | %W     | 요일 (Monday....Sunday)                                      |
   | %D     | 일자 (1st, 2nd.....)                                         |
   | %Y     | 년도 (YYYY)                                                  |
   | %y     | 년도 (YY)                                                    |
   | %a     | 요일 영문 약어 (Sun, Mon..)                                  |
   | %d     | 일자 (01..02..31)                                            |
   | %e     | 일자 (1..2..31)                                              |
   | %m     | 월 (01..02..12)                                              |
   | %c     | 월 (1..2..12)                                                |
   | %b     | 월 (Jan...Dec)                                               |
   | %j     | 해당년에서 몇번째 날인지 (1...366)                           |
   | %H     | 시 (00..01..02..23)                                          |
   | %k     | 시 (0..1..2..23)                                             |
   | %h     | 시 (01..02..12)                                              |
   | %l     | 시 (1..2..12)                                                |
   | %i     | 분 (01..02..59)                                              |
   | %r     | 시각(12) (hh:mm:ss [A/P])                                    |
   | %T     | 시각(24) (hh:mm:ss)                                          |
   | %S,s   | 초 (00..01..59)                                              |
   | %p     | 오전/오후 (A/P)                                              |
   | %w     | 해당 요일중 몇번째 날인지 (0:일요일, 1:월요일.....6:토요일)  |
   | %U,u   | 해당년에서 몇번째 주 인지 (U:일요일이 주의 시작, u:월요일이 주의 시작) |

<br>

### MySQL의 데이터 타입과 형변환

- Database에 데이터를 저장할 때에는 그냥 text 형태로 넣을 수도 있지만, 각 컬럼에 대해 데이터 형을 정의하고 형태에 맞도록 넣어 관리한다. 
- 이는 데이터의 정합성을 지키고 관리를 수월하게 하기 위함이다.
- 그러나 데이터를 조작할 때에 형태를 변환해서 사용해야 하는 경우가 생긴다.

1. 데이터 타입

   - 문자형 데이터 타입

     CHAR, VARCHAR, TINYTEXT, TEXT, MEDIUMTEXT, LONGTEXT, ENUM, SET

   - 숫자형 데이터 타입

     BIT, (BOOL, BOOLEAN, TINYINT(1)), TINYINT, SMALLINT, MEDIUMINT, INT, BIGINT, FLOAT, (DOUBLE, DOUBLE PRECISION, REAL), FLOAT, DECIMAL, (DEC, NUMERIC, FIXED)

   - 날짜형 데이터 타입

     DATE, TIME, DATETIME, TIMESTAMP, YEAR

   - 이진형 데이터 타입

     BINARY, VARBINARY, TINYBLOB, BLOB, MEDIUMBLOB, LONGBLOB

2. 묵시적 형 변환

   - 사용자가 의도한 것에 따라 db가 알아서 변환하여 출락하는 것

     ```sql
     SELECT 100 + 200 FROM DUAL;
     SELECT '100' + '200' FROM DUAL; -- 답은 300으로 출력됨
     
     SELECT CONCAT(82, '는 대한민국 국가 식별 전화번호') FROM DUAL; -- 숫자를 문자열로 바꿔줌
     ```

3. 형 변환 함수 사용하기

   ```sql
   CAST(표현할 값 AS 데이터형식[길이]);
   CONVERT(표현할 값 AS 데이터형식[길이]);
   ```

<br>

![image](https://user-images.githubusercontent.com/41130448/107871644-e34deb00-6ee6-11eb-8a51-510581caee9a.png)

### 일반 함수

1. `ifnull`

   - `ifnull(data, [null 대신 넣을 값 혹은 컬럼명])`와 같이 사용

   - 오라클에서 NVL함수와 같은 기능이다
   - null인 데이터 값이 있을 때 null으로 출력하지 않도 지정하는 다른 특정 값으로 출력하게 한다.
   - null은 아무 것도 없는 것을 의미한다. inner join을 해도 null끼리는 연결되지 않는다.

   ```sql
   select name, dept, salary, ifnull(bonus, 0) from class.salary;
   select name, dept, salary, ifnull(bonus, name) from class.salary;
   ```

2. `if`

   - `if([조건], [조건 성립 시 출력할 내용], [조건 미성립시 출력할 내용])`와 같이 사용

   ```sql
   select name, dept, salary, if(bonus is null, '해당없음', bonus) from class.salary;
   select name, dept, salary, if(salary >= 300, '고액연봉자', '일반연봉자') from class.salary;
   ```

3. `case`

   - 오라클과 비슷하다
   - 특정 값의 여러 경우를 고려해서 출력

   ```sql
   select name
   	, case  when dept = 'A' then '경영지원부'
   			when dept = 'B' then '영업부'
   			else '회계팀' end as dept
   	, salary
   	, bonus
   from class.salary;
   -- 위와 같이 써도 되지만 아래가 좀 더 명확하다.
   select name
   	, case  when dept = 'A' then '경영지원부'
   			when dept = 'B' then '영업부'
   			when dept = 'C' then '회계팀' end as dept
   	, salary
   	, bonus
   from class.salary;
   ```

4. 복합적으로 사용하기

   ```sql
   select name
   	, case  when dept = 'A' then '경영지원부'
   			when dept = 'B' then '영업부'
   			when dept = 'C' then '회계팀' end as dept
   	, salary
   	, if(salary >= 300, '고액연봉', '일반') as salary_type
   	, ifnull(bonus, 0)
   	, case  when ifnull(bonus, 0) = 0 then '해당없음'
   			else '보너스 해당자' end as bonus_type
   from class.salary;
   ```

<br>

### 복수 행 함수

- window 함수, 그룹 함수라고도 한다.

- 한번에 여러 데이터에 대한 결과를 출력한다.

- 복수 행 함수에 컬럼 명을 넣었을 때 해당 컬럼에 null 값이 있다면 이것을 제외하고 결과가 나오므로 주의해야 한다.

  예를 들면 5개의 row가 있는데, 어떤 컬럼의 평균을 구하려고 한다. 그런데 이 중에 null값이 있다면 이들을 제외하고 구한다. 따라서 아래와 같이 null 값을 미리 처리하고 계산해야 한다.

  ```sql
  select avg(ifnull(bonus, 0)) from class.salary;
  ```

1. `count`

   - 입력되는 데이터의 총 건수를 반환한다.
   - `*`를 넣어서 사용할 수 있는데, 이 경우 전체 컬럼을 대상으로 총 건수를 계산한다.
   - 특정 칼럼명을 넣는다면 해당 칼럼에서 null 값을 제외하고 데이터 건수를 반환한다.

   ```sql
   select count(*) from class.salary;
   ```

2. `sum`

   - 입력된 데이터들의 합계 값을 구해서 반환

   ```sql
   select sum(salary) from class.salary;
   ```

3. `max`, `min`

   - 최댓값과 최솟값을 구함

   ```sql
   select max(salary), min(salary) from class.salary;
   ```

4. `stddev`

   - 표준 편차를 구하는 함수

   ```sql
   select stddev(salary) from class.salary;
   ```

5. `variance`

   - 분산을 구하는 함수

   ```sql
   select variance(salary) from class.salary;
   ```

<br>

### GROUP BY 사용하기

1. 평균 및 합계 구하기

   ```sql
   select do, avg(budget_value) as 예산평균, sum(budget_value) as 예산합계
   from class.budet
   group by do;
   ```

   `do`가 동일한 것끼리 budget_value의 합과 평균을 구하기

2. 주의할 점

   - 컬럼 명을 그대로 쓸 수 있지만, 함수를 이용해야 할 수도 있다. 이 때는 select 절에도 group by 절에서 쓴 함수를 그대로 써줘야 정상적으로 작동한다.

   ```sql
   -- grouping을 잘못 한 경우 (컬럼 값이 원하지 않는 방식으로 나온다)
   
   select do, avg(budget_value) as 예산평균, sum(budget_value) as 예산합계
   from class.budget
   group by if(do in ('서울특별시', '경기도'), '수도권', '지방');
   
   -- select 절을 group by 절과 같도록 작성한 경우
   
   select if(do in ('서울특별시', '경기도'), '수도권', '지방') as 지역구분, avg(budget_value) as 예산평균, sum(budget_value) as 예산합계
   from class.budget
   group by if(do in ('서울특별시', '경기도'), '수도권', '지방');
   ```


<br>

### Join하기

- 관계형 데이터베이스는 테이블이나 스키마들이 서로 관계를 가지고 있다. 이런 관계를 이용해서 SQL을 작성할 수 있는데 이 때 join을 사용한다.

- 여러 테이블이나 스키마에 분산되어 있는 데이터를 하나의 view로 출력하는 것이다.

- Join은 여러 가지 방법이 존재하는데, 각 방법의 특징을 알고 있어야 한다.

- Join의 종류를 살펴보자

  1. 카티션 곱 join

     - 테이블들을 join할 때 join 조건을 기술하지 않고 하는 join

     - 결과는 두 테이블의 row 개수를 곱한 만큼 나온다.
     - 업무에서 흔히 사용되지는 않음
     - 데이터를 많이 불려야 하거나, 특정한 조건 안에서 사용한다.
2. Equi join (Inner join)
  
   - 양쪽 테이블의 어떤 칼럼에 같은 값이 존재할 때 equal 연산자를 이용하여 양쪽에 다 존재하는 값만 결과로 출력
     - 가장 보편적인 join 방식
  3. Non Equi join

     - Equi join과 반대 개념
   - 두 테이블을 join할 때 서로 다른 값을 가지거나, 한쪽 데이터가 다른 쪽 테이블의 데이터 범위 내에 있는 것만 출력을 원할때 사용
     - inner join에 속한다
  4. Outer join

   - left outer join, right outer join, full outer join으로 구분됨
     - left, right join의 경우 어느 한쪽의 데이터를 모두 출력한 뒤 조건에 맞는 데이터만 다른 쪽에 출력
   - 조건이 맞지 않는 경우 null 값을 표시
  5. Self join
     - 자기 자신과 join하는 경우
     - 일반적으로 사용하지는 않음
   - 그러나 꼭 필요한 경우가 있다고 함

<br>

### 카티션 곱 join (Cartesian product)

- where절이나 on 절에 join 조건을 주지 않고 join을 수행

- cross join이라고도 한다.

- 사실 이 연산은 RDBMS의 개념과 상충하는 개념이 된다. 키를 이용해서 다른 테이블과 관계를 맺는 것이 아니라, 그냥 모든 데이터를 1:1로 연결하기 때문이다.

- 그래도 이 연산이 필요한 경우

  1. 데이터를 대량으로 복제해야 할 때
  2. 특정 데이터 튜플만 복제되어야 할 때
  3. 연결고리가 없는 두 테이블의 데이터를 무작위로 합쳐야 할 때

- MySql

  `,`를 넣어서 구분

  ```sql
  select m.major_id, m.major_title, p.prfs_id, p.name 
  from class.major m, class.professor p;
  ```

- ansi SQL

  `cross join` 명령어를 사용

  ```sql
  select m.major_id, m.major_title, p.prfs_id, p.name 
  from class.major m cross join class.professor p;
  ```

<br>

### Inner join

- 가장 일반적인 조인

- 보통 특별한 이야기 없이 join이라고 하면 요놈을 말하는 것이다

- 두 테이블 간에 연결되는 키가 있다고 가정하고, 그 키 값이 같은 데이터를 가지고 와서 출력하는 것

- Equi join, join, 등가 조인 모두 같은 표현이다.

- MySQL

  ```sql
  select p.name as 교수이름, m.major_title as 학과명 
  from class.professor p , class.major m 
  where p.bl_major_id = m.major_id;
  ```

- Ansi SQL

  cross join, join, inner join 모두 사용할 수 있다.

  ```sql
  -- 1
  select p.name as 교수이름, m.major_title as 학과명 
  from class.professor p 
  	join class.major m 
  		on p.bl_major_id = m.major_id;
  
  -- 2
  select p.name as 교수이름, m.major_title as 학과명 
  from class.professor p 
  	cross join class.major m 
  		on p.bl_major_id = m.major_id;
  
  -- 3 inner join으로 쓰는게 아무래도 제일 보기 명확하다.
  select p.name as 교수이름, m.major_title as 학과명 
  from class.professor p 
  	inner join class.major m 
  		on p.bl_major_id = m.major_id;
  ```

- 3개의 테이블을 join하기

  - 키를 연결하는 방법은 다음과 같다.

    ![image](https://user-images.githubusercontent.com/41130448/107904742-fcf73d00-6f8f-11eb-9217-5789d933bbaa.png)

    테이블 1과 2의 공통키를 찾아 연결후 이 결과물이 되는 테이블을 다시 테이블 3과 1:1 조인한다고 생각한다.

  ```sql
  -- MySql
  select s.name as 학생이름, p.name as 교수이름, m.major_title as 학과명 
  from class.student s, class.major m, class.professor p 
  where s.bl_prfs_id = p.prfs_id and p.bl_major_id = m.major_id;
  
  -- Ansi Sql
  select s.name as 학생이름, p.name as 교수이름, m.major_title as 학과명 
  from class.student s 
  	inner join class.major m 
  		inner join class.professor p 
  			on s.bl_prfs_id = p.prfs_id and p.bl_major_id = m.major_id;
  ```

<br>

### Non equi Join

- 두 테이블을 join할 때 값이 서로 같지는 않지만 join 조건에서 지정한 어느 범위에 일치할 때 서로 데이터를 join해주는 것이다.

- 예를 들어 선물에 5, 10, 15...의 값을 갖는 포인트가 매겨져 있다. 고객이 8포인트를 가지고 있다면 10포인트의 선물은 가져갈 수 없고 5포인트의 선물과 매칭이 되어야 한다. 이런 때에 non equi join을 사용할 수 있다. 각 고객이 받을 수 있는 선물을 non qui join을 이용해서 출력 가능하다.

  between, and를 사용하며 이 조건에 해당하면 조인한다.

  ```sql
  -- MySQL
  select c.name as 고객명, c.point as 고객_point, g.name as 상품명
  from class.customer c, class.gift g
  where c.point between g.point_s and g.point_e;
  
  -- Ansi SQL
  select c.name as 고객명, c.point as 고객_point, g.name as 상품명
  from class.customer c join class.gift g
  on c.point between g.point_s and g.point_e;
  ```

<br>

### Outer join

- Inner join 다음으로 많이 사용하는 기법이다.

- 두 테이블에서 일치하는 키 값이 없는 데이터도 함께 출력하는 기법이다.

- 누락 없이 데이터를 모두 보고자 할 때 쓸 수 있다.

- outer join은 모든 데이터를 다 가지고 올 때 full scan을 하기 때문에 DB에 무리가 갈 수 있으므로 꼭 필요할 때만 사용한다.

- left, right, full outer join이 있는데, mysql에서는 full outer join을 지원하지 않는다. 필요 시 union을 사용하면 우회적으로 구현할 수 있다.

  ![image](https://user-images.githubusercontent.com/41130448/107905594-5b251f80-6f92-11eb-990e-102ea03c7afc.png)

  ```sql
  select * 
  from A full outer join B 
  on A.a = B.b; 
  -- 위의 full outer join 연산을 아래의 union all을 통해 구할 수 있음
  select * 
  from A left outer join B 
  on A.a = B.b 
  union 
  select * 
  from B left outer join A 
  on A.a = B.b;
  ```


<br>

### 서브 쿼리

- SQL 내에서 또 다른 select 절을 사용하는 문법을 의미한다.

  1. 장점
     - 데이터를 폭넓게 사용할 수 있다.
     - 복잡한 쿼리를 좀더 단순화 할수 있다.
  2. 단점
     - 조인을 이용해서 풀 수 있는데, 서브 쿼리를 이용한다면 성능이 저하할 수 있다.
     - 꼭 필요한 상황에서만 쓰도록 한다.

- 서브 쿼리의 종류는 select, from, where 중 어느 절에서 사용하느냐에 따라 구분한다.

  1. select 절

     스칼라 서브쿼리 (Scalar subquery)

  2. from 절

     인라인 뷰 (Inline view)

  3. where 절

     중첩 서브쿼리, 서브쿼리

![image](https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2Fp3g69%2FbtqD4VzuaGh%2F3NHLU0z2oOM7kHcHEMs0N1%2Fimg.png)

<br>

### 스칼라 서브쿼리

- select 절에서 사용

- 괄호 사이에 서브쿼리를 넣는다. 

- 쿼리의 결과가 하나의 행으로만 나와야 한다. 두 개 이상의 행이 나오게 된다면 SQL이 실행되지 않는다.

  ```sql
  -- 1
  SELECT
  	(SELECT B.NAME
  	 FROM DEPT B
  	 WHERE B.DEPT_NO = A.EMP_NO)
  FROM EMP_NO A;
  
  -- 2 학생 테이블의 major_id 값을 학과 테이블에서 검색하여 학과명을 출력
  SELECT NAME as 학생이름,
  	(SELECT major_title
       from class.major b
       where b.major_id = a.major_id) as 학과명
  FROM class.student a;
  ```

<br>

### 인라인 뷰

- from 절에 사용됨

- 괄호 사이에 서브쿼리를 넣는다.

- 하나의 테이블이라고 생각할 수 있다. 인라인 뷰를 이용해서 join, where 조건절 등을 사용할 수 있다.

- 메인쿼리에서 서브쿼리의 컬럼을 사용하려면, **서브 쿼리 내의 select로 가져온 컬럼만 사용할 수 있다.**

  (메인 쿼리의 select에서 사용, where에서 조건으로 사용, join시 key로 사용 시)

  ```sql
  -- 1
  SELECT cs.name, SUM(od.saleprice) "total"
  FROM (
  	SELECT custid, name
  	FROM Customer
  	WHERE custid <= 2
  )
  WHERE cs.custid = od.custid
  GROUP BY cs.name;
  
  -- 2
  SELECT a.name as 학생이름, b.major_title as 학과면
  FROM class.student a, 
  	(
  		SELECT major_title, major_id
          FROM class.major 
  	) b
  WHERE a.major_id = b.major_id;
  ```

<br>

### 서브 쿼리

- where 절에서 사용

- 쿼리의 결과 행 수에 따라 단일행 서브 쿼리와 복수행 서브 쿼리로 나눌 수 있다.

- where절 내의 비교 연산자 사용 시 주의해야 한다.

  1. 단일 행 결과가 나올 시 사용할 수 있는 비교 연산자

     =, <>, >, >=, <, <=

  2. 복수 행 결과가 나올 시 사용할 수 있는 연산자

     IN(NOT IN), EXIST, NOT EXIST

  ```sql
  -- 단일행 서브쿼리
  SELECT name as 학생이름
  FROM class.student
  WHERE major_id = 
  	(select major.major_id 
       from class.major 
       where major_title='컴퓨터공학과')
  	
  -- 복수행 서브쿼리
  SELECT name as 학생이름
  FROM class.student
  WHERE major_id in 
  	(select major.major_id 
       from class.major 
       where major_title in ('컴퓨터공학과', '국문학과'))
  ```

<br>

### 서브쿼리를 join으로 출력하기

- 서브 쿼리보다 join을 사용하는 것이 대부분의 경우 좋은 성능을 갖는다.

  오라클의 경우 스칼라 서브쿼리 캐싱 기능이 있어 nested loop와 전반적으로 비슷하고, 더 빠를 수도 있다.

- 대용량 hash join의 경우 스칼라 서브 쿼리는 조인보다 빠를 수 없다.

  ```sql
  SELECT a.name as 학생이름, a.major_title as 학과명
  FROM class.student a, class.major b
  WHERE a.major_id = b.major_id;
  ```

<br>

### Insert

- 기존에 없는 새로운 데이터를 database의 테이블에 넣는 명령

- 한번에 하나의 테이블에만 수행 가능

- 문자 형태로 정의된 컬럼은 `'`로 데이터 양쪽을 감싸야 한다.

- 가장 보편적이고 일반적인 방법이다.

  1. 단일 행 입력하기 (컬럼 미지정)

     - insert 대상 테이블의 칼럼을 지정하지 않고 insert하는 방법이다.
     - 테이블의 컬럼 순서대로 데이터를 넣는다.

     ```sql
     INSERT into class.insert_test values (1, '이준영', '안양시', '01015881588', now());
     ```

  2. 단일 행 입력하기 (컬럼 지정)

     - insert 대상 테이블의 칼럼을 지정하고 insert하는 방법이다.
     - 지정한 컬럼의 순서대로 데이터를 넣는다. 
     - 컬럼 일부만 데이터를 갖도록 넣을 수 있다. 값을 안쓴 컬럼은 null이 들어간다. 

     ```sql
     INSERT into class.insert_test (seq, name, place) values (1, '이준영', '안양시');
     ```

  3. 복수 행 입력하기

     - 하나의 insert문으로 여러 행의 데이터 입력하기

     ```sql
     INSERT into class.insert_test
     values  (1, '이준영', '안양시', '01015881588', now()),
     		(2, '이세화', '안산시', '01015881588', now()),
     		(3, '엄준식', '서울시', '01015881588', now()),
     		(4, '이병건', '안산시', '01015881588', now()),
     ```

  4. insert select문 활용하기

     - select한 결과를 바로 입력하는 것이다.

     ```sql
     INSERT into class.insert_test SELECT * FROM class.insert_test2;
     ```

<br>

### Update

- 기존 데이터의 행 수는 변하지 않지만, row 내의 특정 칼럼의 값을 바꾸는 작업

  1. 기본 사용법

     - where 절을 빼고 넣으면 모든 행의 지정한 컬럼 값이 변경된다.

     ```sql
     UPDATE [테이블명] SET [컬럼명] = [값] WHERE [키 컬럼] = [키 값];
     -- 예시
     UPDATE class.update_test SET name = '엄준식' WHERE seq = 26;
     -- 비교 연산자 사용
     UPDATE class.update_test SET name = '진용원' WHERE seq in (11, 12, 13, 14, 15);
     ```

  2. 2개 이상의 칼럼 값 변경하기

     - SET 부분에 변경할 컬럼을 콤마로 나열해서 작성하면 된다.

     ```sql
     UPDATE class.update_test SET name = '옥냥이', content = '먼저함' WHERE seq in (11, 12, 13, 14, 15)
     ```

<br>

### Delete

- 테이블에서 데이터를 삭제할 때 사용

  1. 조건을 넣어서 사용

     ```sql
     DELETE FROM [테이블명] WHERE [조건문];
     DELETE FROM class.delete_test WHERE seq = 10;
     ```

  2. 테이블 내 모든 데이터 삭제

     ```sql
     DELETE FROM [테이블명];
     DELETE FROM [테이블명] WHERE 1=1;
     ```

  3. select의 결과로 삭제

     ```sql
     DELETE FROM class.delete_test WHERE seq in (select seq from class.delete_test2)
     ```

<br>

### Merge

- merge문은 오라클에서 부르는 호칭이며, MySql에서는 `insert into on duplicate key`와 같은 방식으로 실행

- 어떤 데이터를 입력할 때, 대상 테이블에 해당 키를 갖는 데이터가 없으면 insert문을 실행하여 입력하고 이미 데이터가 존재하면 update하여 값을 갱신

  ```sql
  -- select문 사용하기
  INSERT INTO class.insert_test
  SELECT *
  FROM class.insert_test2 b
  ON DUPLICATE KEY UPDATE cont = b.cont,
  						name = b.name,
  						tel_num = b.tel_num,
  						input_date = now();
  						
  -- 데이터 넣기
  INSERT INTO insert_test 
  VALUES (3,'먼저함', '옥냥이', 1012345678, now()) 
  ON DUPLICATE KEY UPDATE cont = '먼저함', 
  						name = '옥냥이', 
  						tel_num = 1012345678, 
  						input_date = now();
  ```


<br>

### DDL문

- DBMS를 사용하고 관리하는 데 있어서 가장 중요한 것

- DB 내의 많은 오브젝트를 생성, 변경, 관리하는 쿼리문이다.

- 4종류가 있다. CREATE, ALTER, TRUNCATE, DROP 

  1. CREATE

     - 새로운 오브젝트나 스키마 등을 생성할 때 사용

     - MySQL의 오브젝트 대부분을 생성할 수 있다. (Table, Procedure, Function, Index, User 등)

     - primary key가 되는 컬럼은 not null로 정의되어 있어야 한다.

       복수 컬럼을 pk로 설정하고 싶다면 `primary key([컬럼 1], [컬럼 2], ...)`와 같이 쓰면 된다.

     - 코멘트는 논리 컬럼명이나 column description라고 표현 가능하다.

     - `engine = InnoDB`는 MySQL의 스토리지 엔진 중 InnoDB를 사용한다는 뜻이다. 

     ```sql
     CREATE TABLE [스키마].[테이블명](
     	[컬럼명] [자료형] [null 옵션] comment '[코멘트]'
     	primary key([컬럼명])
     ) comment '[코멘트]' engine = InnoDB;
     ```

     ```sql
     -- DB 계정(user) 관리
     -- localhost에서만 접속 허용
     CREATE user 'user'@'127.0.0.1' identified by 'password';
     -- 모든 곳에서 접속 허용
     CREATE user 'user'@'%' identified by 'password';
     ```

     > MySQL은 2가지 구조로 되어 있다.
     >
     > 1. 서버 엔진
     >
     >    - 클라이언트에서 쿼리를 요청시, 파싱을 하고 스토리지 엔진에 요청을 보내는 역할
     >
     > 2. 스토리지 엔진
     >
     >    - 물리적 저장장치에서 데이터 읽어오는 역할
     >    - 엔진의 종류마다 동작 원리가 다르다.
     >
     >    - 데이터를 직접적으로 다루므로 가장 중요한 엔진이다. 트랜잭션 등의 성능과 밀접한 연관이 존재한다.

  2. ALTER

     - 이미 만들어져 있는 오브젝트의 내용을 변경할 때 사용
     - 컬럼명, 컬럼의 데이터 타입, 코멘트 등을 변경할 수 있다.
     - 신규 컬럼 추가나, 기존 컬럼 삭제도 가능하다.

     ```sql
     -- 컬럼명 변경
     ALTER TABLE [테이블명] CHANGE [이전컬럼명] [바꿀컬럼명] [데이터타입] [null 옵션];
     
     -- 컬럼 추가
     ALTER TABLE [테이블명] ADD [컬럼명] [데이터타입] [NULL 옵션] AFTER [앞에 위치할 컬럼명]
     ```

  3. TRUNCATE

     - 테이블의 데이터를 전부 삭제하고 테이블이 차지하고 있던 공간을 반납

     - 테이블 자체를 DROP한 후 다시 CREATE하는 방식이다. (DELETE와 다름)

       따라서 전체 데이터 삭제시 **DELETE보다 빠르지만, 데이터 복구가 불가능**하다. 

     - 오라클에서는 인덱스가 DROP되며, MySQL에서는 인덱스가 DROP되지 않는다.

     - `AUTO_INCREMENT`로 지정하여 증가하던 컬럼이 초기화된다.

     ```sql
     TRUNCATE TABLE [스키마].[테이블];
     ```

  4. DROP

     - 테이블 자체를 삭제, 인덱스도 함께 삭제
     - 저장공간 또한 함께 반납한다.
     - 실수로 테이블을 DROP했을 시 MySQL에서는 BINLOG 파일을 이용해서 데이터를 살려야 한다.

     ```sql
     DROP TABLE [스키마].[테이블];
     ```

<br>

### Data dictionary

- MySQL 서버 내에 있는 데이터베이스 개체에 관한 정보가 모두 모아져 있는 곳이다. 

- 해당 DB에 생성된 유저, 스키마, 테이블, 컬럼, 테이블 내의 데이터, 오브젝트 간의 종속성 등의 정보가 들어 있다. 

- 4가지 종류가 있다. 

  information_schema, mysql, sys, performance_schema 

- performance_schema는 DB 내 성능 지표를 확인할 수 있는 스키마이다. MySQL 소스 코드 곳곳에 심어 있는 여러 instrunment를 통해 수집된다. 

- 여기서는 많이 사용되는 information_schema, mysql을 보자.

1. information_schema

   - Data Dictionary, System Catalog라고도 한다.

   - `select TABLE_SCHEMA, TABLE_NAME from information_schema.TABLES where TABLE_SCHEMA = 'information_schema';`를 치면 겁나 많은 테이블 목록이 나오는데, 자주 쓰이는 system view만 살펴보자.

     아래와 같이 사용한다.

     ```sql
     select * from [table 이름];
     select * from information_schema.SCHEMATA;
     ```

   - `information_schema.SCHEMATA`

     MySQL 내부에 있는 스키마(db) 목록을 볼 수 있고, 스키마별 캐릭터 셋을 확인할 수 있다.

   - `information_schema.TABLES`

     MySQL 내부에 생성되어 있는 SYSTEM VIEW 및 테이블에 관한 정보를 볼 수 있다.

     생성일자, 수정일자, 테이블 내 row 개수, 테이블 코멘트, 스토리지 엔진 타입 정보를 한번에 확인 가능하다.

   - `information_schema.COLUMNS`
   
     MySQL 내부에 생성되어 있는 칼럼 정보를 확인할 수 있다.
   
     컬럼명, 초기입력값, 컬럼 순서, 테이터 타입, 데이터 길이, 컬럼 코멘트 등의 정보를 확인 가능하다.
   
   - `information_schema.ROUTINES`
   
     MySQL 내부에 생성되어 있는 function과 procedure에 관한 정보들이 있다.
   
     각 프로그램별 입출력 데이터 정보와 프로그램 소스 정보가 저장되어 있다.
   
   - `information_schema.KEY_COLUMN_USAGE`
   
     테이블 별 pk 컬럼이나 unique 제약조건 목록을 확인할 수 있다.
   
   - `information_schema.PROCESSLIST`
   
     현재 MySQL에 접속되어 있는 세션 정보를 확인할 수 있다.
   
     세션 상태와 접속한 user, host, 어떤 sql을 실행하고 있는지 등을 확인할 수 있다.
   
   - 더 자세한 정보는 [여기](https://dev.mysql.com/doc/mysql-infoschema-excerpt/8.0/en/information-schema.html)서 확인할 수 있다.
   
2. mysql

   - information_schema를 통해 조회할 수 있는 내용과 중복된 내용도 있다.

   - 자주 쓰이는 것만 살펴보자.

   - `mysql.user`

     MySQL 내부에 생성된 user에 관한 정보가 들어 있다.

     host 정보, 암호화된 비밀번호, 기본 권한에 대한 정보가 있다.

   - `mysql.general_log`

     DB로그를 Table 타입으로 저장할 때 쌓이는 테이블

     로그 설정을 하지 않았다면 이 테이블에는 정보가 없을 것이다.

     sql 실행 시간, host 및 user 정보, 상세 sql이 저장되어 있다.

   - `mysql.slow_log`

     long_query_time 파라미터 값보다 오래 걸리는 쿼리가 저장된다.

     성능 분석시 용이하게 사용 가능하다.

<br>

### 제약 조건

- contraint라고 하며 데이터를 입력받는 규칙을 의미한다. 규칙에 어긋나는 데이터는 입력 및 변경이 이루어지지 않도록 한다.

- 특정 컬럼에 중복값이 들어오는 것을 막거나, 값이 범위를 벗어나지 않도록 하거나, 상위 테이블에 등록된 값만 하위 테이블에 입력이 되도록 지정하는 것 등이 있다.

- DB 테이블의 정확성과 신뢰도를 증가시킨다.

- 제약 조건은 테이블 생성시 포함하거나, 추후 컬럼에 추가, 변경, 삭제할 수 있다.

- 설계 단계에서 모든 제약조건을 고려하면 좋으나 현실적으로 어렵다.

- 실무에서는 foreign key는 잘 적용하지 않는 추세라고 한다.

- MySQL의 제약 조건

  1. primary key

     - 컬럼의 중복을 막고, null을 허용하지 않는다. (unique + not null)
     - 각 row를 특정할 수 있는 구분키이다.
     - 테이블당 하나만 생성 가능하며, pk 자체가 복수 컬럼이 되는 것은 가능하다.

     ```sql
     alter table schema_name.table_name add primary key (col1, col2, ....); -- drop 
     alter table schema_name.table_name drop primary key;
     ```

  2. foreign key

     - 어떤 테이블의 컬럼 값은, 다른 테이블의 컬럼 값을 참조하여야 한다는 제약 조건
     - 예를 들면 주문 테이블에 들어오는 고객 ID는 반드시 고객 테이블에 존재해야 한다는 것이다.

     ```sql
     alter table order add constraint order_customer_id_fk foreign key (customer_id) references customer (customer_id); 
     -- drop 
     alter table order drop key order_customer_id_fk;
     ```

  3. Not null

     - 해당 칼럼에는 null 값이 들어올 수 없다고 명시하는 것

     ```sql
     alter table schema_name.table_name modify col1 int not null; 
     -- not null 해제 
     alter table schema_name.table_name modify col1 int null;
     ```

  4. Unique

     - 해당 컬럼에는 중복값이 들어가지 못하게 설정하는 것
     - primary key가 아닌 컬럼에 사용
     - 복수 컬럼을 한번에 지정할 수 있는데, 이 경우 두 컬럼의 값이 모두 같은 경우 중복값으로 인식된다.
     - 이 제약 조건을 잘못 설정하면 해당 테이블에 insert가 되지 않는 상황도 발생할 수 있으므로 조심해야 한다.

     ```sql
     alter table schema_name.table_name add constraint table_pk unique (col1, col2); 
     -- drop 
     alter table schema_name.table_name drop key table_pk;
     ```

  5. Check

     - 해당 컬럼에는 check 제약 조건으로 지정된 값만 들어오도록 설정하는 것이다.
     - 특정 값만 들어와야 하는 여부, 성별, 범위, 수량 정보를 저정하는 컬럼에 설정할 수 있다.
     - 무결성을 보장해주지만, unique처럼 신중하게 사용해야 한다.

     ```sql
     ALTER TABLE schema_name.table_name ADD CONSTRAINT CHK_PersonAge CHECK (col1 >=18); 
     -- drop 
     ALTER TABLE schema_name.table_name DROP CONSTRAINT CHK_PersonAge;
     ```

  6. Default

     - 규칙보다는 초기값을 의미하므로 엄밀히 말하면 제약조건은 아니다.
     - 특별한 값을 지정하지 않을 때 row를 생성하면 미리 지정한 값이 들어간다.

     ```sql
     alter table customer alter column name set default 'N'; 
     -- drop 
     alter table customer alter column name drop default;
     ```

<br>

### 인덱스

- 쿼리의 성능을 높이기 위해서 사용한다. 테이블 내의 데이터를 정렬하고, 필요한 데이터만 빠르게 가져온다.

- 인덱스의 종류

  1. 클러스터 인덱스
     - primary key를 설정하면 자동으로 생성된다.
     - 해당 pk 컬럼 데이터가 변경되더라도 항상 정렬을 유지한다.
     - 테이블당 1개의 클러스터 인덱스를 생성할 수 있다.
     - 지정된 컬럼을 기준으로 데이터가 물리적으로 정렬된다. 따라서 리프 노드와 추가적인 공간이 필요하지 않다.
     - 조회 성능은 빠르지만, insert, update, delete와 같은 변경 작업 시 정렬을 하므로 성능이 떨어질 수 있다.
     - 클러스터 인덱스 생성 시 테이블 내의 다른 보조 인덱스에 pk 값을 포함하게 된다. 이로 인해 보조 인덱스들의 크기가 커질 수 있다.
     - 클러스터 인덱스를 제외한 다른 인덱스는 논 클러스터 인덱스라고 부르며, 테이블당 249개까지 생성 가능하다.
  2. 단일 인덱스
     - 인덱스 생성 시 하나의 컬럼만 지정하는 경우
     - 주로 데이터가 많지 않은 경우 사용
     - 주로 사용하는 sql에 조건이 하나만 걸릴 때 생성
  3. 복합 인덱스
     - 인덱스 생성 시 두 개 이상의 컬럼을 지정하는 경우
     - 주로 사용하는 조회 쿼리에 조건이 많이 걸릴 때 사용을 고려할 수 있다.
     - 테이블에서 다른 공간으로 데이터를 가지고 와서 정렬하므로 용량을 차지한다.
  4. 커버드 인덱스
     - sql 내에서 출력하는 컬럼과 조건에 삽입된 컬럼이 모두 인덱스에 정보가 있어서 실제 테이블을 조회하지 않고 데이터를 가지고 올 수 있는 경우에 사용되는 인덱스
     - 성능적인 측면만 고려할 때 가장 좋을 수 있는 방법이다.

- 인덱스 생성 기준

  1. 테이블 내에 데이터가 많을 때 

     데이터가 적은 경우 full scan이 index scan보다 빠를 수 있다. 테이블 내에 데이터가 많을수록 효과를 크게 볼 수 있다.

  2. Primary key 컬럼에는 생성할 필요가 없다.

     pk 컬럼은 클러스터 인덱스가 생성되며, unique 컬럼도 마찬가지이다. pk와 unique 컬럼은 인덱스를 따로 추가할 필요가 없다.

  3. Cardinality를 확인하고 생성하자

     cardinality는 컬럼 내 값의 분산도를 의미한다. 주민등록번호는 cardinality가 높고, 성별은 cardinality가 낮다고 할 수 있다. 분산도가 큰 데이터에 인덱스를 생성하는 것이 유리하다.

- 인덱스의 역효과

  1. 오버헤드

     데이터가 변경되는 작업(insert, update, delete) 발생시 인덱스 갱신이 함께 이루어지므로 오버헤드가 발생한다. 

  2. 오사용

     한 개의 테이블에 여러 인덱스 생성 시 옵티마이저가 실행 계획을 만들 때 의도하지 않은 인덱스를 사용하게 될 수 있다. 따라서 꼭 필요한 곳에 적당히 사용해야 한다.

- B-tree 인덱스

  - 여러 DBMS에서 가장 많이 사용하는 방식이다.

  - 컬럼의 데이터는 변형시키지 않고 인덱스 내부에서 정렬된 상태로 유지한다.

  - 전문 검색과 같이 특이한 상황이 아니면 보통 B-Tree 인덱스를 사용한다.

  - 3개의 노드로 구분할 수 있다.

    1. Root node 최상위 노드
    2. Leaf node 최하위 노드
    3. Branch node 루트와 리프 노드를 연결하는 노드

    ![image](https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2Fcf5EvN%2FbtqFyRCDpdV%2FA3ohkDXznTskWlgmkizAEK%2Fimg.png)

  - 인덱스 값이 새로 들어오면, 키값을 이용해서 적당한 위치를 찾는다. 저장될 위치가 결정되면 레코드의 키 값과 주소 정보를 B-Tree 인덱스의 leaf node에 저장한다. 이 때 leaf node가 꽉 차서 분리될 수도 있다. 

  - 일반적으로 테이블에 데이터를 저장하는 것보다 1~1.5배 비용이 더 발생한다.

  - 인덱스 내의 키가 변경될 때에는 delete and insert가 수행된다.

- 인덱스를 사용할 수 없는 SQL 조건절

  1. equal로 조건절이 작성되지 않는 경우

     <>, NOT IN, NOT BETWEEN AND, NOT NULL

  2. like를 사용하는 경우

     `like '%Test'`

  3. function을 사용하는 경우

     `substring(col_name, 1)='A'`

  4. 데이터 타입이 서로 다른 경우

     `char_col = 10`

- 인덱스 사용 예제

  ```sql
  -- 인덱스 생성하기
  CREATE INDEX <Index name> ON <Table name> ( column 1, column 2, ... ); 
  -- 단일 인덱스 
  ALTER TABLE books ADD INDEX idx_test ( writer ); 
  -- 복합 인덱스 
  ALTER TABLE books ADD INDEX idx_test ( writer, company, ... );
  
  -- 인덱스 삭제하기
  ALTER TABLE books DROP INDEX idx_test;
  
  -- 유니크 인덱스 생성
  ALTER TABLE tablename ADD UNIQUE INDEX indexname (column1, column2);
  
  -- 생성된 인덱스 확인
  SHOW INDEX FROM <Table name>;
  ```

<br>

### View

- 가상의 테이블을 의미한다. 데이터는 없고, sql만 저장되어 있는 object이다.
- select 서브 쿼리에서 사용한 inline view가 바로 요것이다.
- view를 select하게 되면 view가 가지고 있는 sql문이 실행된다.

1. 이거 왜쓰냐?

   - view라는 object를 따로 만들어서 관리하는 것은 보안과 사용자의 편의성을 높이기 위한 것이다.
   - 여러 테이블을 조인할 때 view를 이용하면 sql을 간소화 시킬 수 있다. 또한 복잡한 sql을 편하게 재생성할 수 있다.
   - 보안을 위해 쓰는 경우는 다음과 같다. 특정 유저가 어떤 스키마 테이블을 보는 권한이 없으며 보안상 공개할 수도 없지만 해당 테이블의 특정 정보가 업무상 꼭 필요한 경우가 있을 수 있다. 이 때 view를 생성하여 사용하면 꼭 필요한 정보만 보여주고, 전체 정보는 공개하지 않을 수 있다.

2. 특징

   - 일반 테이블과 같은 형태를 가지며, sql로 조작하는 방법도 흡사하다.

   - 데이터의 논리적 독립성을 가지고 있다. 

   - 필요한 데이터만 따로 정의해서 사용하므로 관리가 용이하고, sql 명령문이 간단해진다.

   - 뷰 생성시 기본 테이블의 키를 포함한다면 삽입, 삭제, 갱신, 연산이 가능하다.

   - 생성된 뷰는 다른 뷰의 기본 데이터가 될 수 있다. 기본 데이터가 되는 테이블이나 뷰를 삭제하면, 해당 데이터를 기초로 한 다른 뷰들이 자동으로 삭제된다.

   - ALTER 명령어를 사용할 수 없고, 수정을 원한다면 drop 후 create 를 해주어야 한다.

   - 원본 테이블과 같은 이름으로 생성할 수 없다.

     실무에서는 대체적으로 "vw_"와 같은 접두사를 붙여서 뷰라는 것을 명시해준다.

3. 사용 예제

   ```sql
   -- 생성
   CREATE VIEW [뷰이름] AS [SELECT 구문];
   -- 삭제
   DROP VIEW [뷰이름];
   ```

4. 실행 원리

   - 아래 쿼리로 `accounts_view` 뷰를 생성한다

     ```sql
     SELECT name,
            money_received,
            money_sent,
            (money_received - money_sent) AS balance,
            address,
      ...
       FROM table_customers c
       JOIN accounts_table a
         ON a.customer_id = c.customer_id
     ```

   - 생성한 뷰로 sql을 실행한다

     ```
     SELECT name, balance
     FROM accounts_view;
     ```

   - 실제 DBMS 옵티마이저가 인식하는 SQL은 다음과 같다.

     ```sql
     SELECT name,balance
     FROM (SELECT name,
                  money_received,
                  money_sent,
                  (money_received - money_sent) AS balance,
                  address,
      ...
           FROM table_customers c JOIN accounts_table a
             ON a.customer_id = c.customer_id);
     ```

     

