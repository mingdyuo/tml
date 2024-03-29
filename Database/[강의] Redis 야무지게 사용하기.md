# Redis 야무지게 사용하기

Ref : [[NHN FORWARD 2021\] Redis 야무지게 사용하기 - YouTube]](https://www.youtube.com/watch?v=92NizoBL4uA)



## Redis 캐시로 사용하기

- 캐시란
  - 사용자 입장에서 원래 **데이터 소스보다 빠르고** 효율적으로 액세스
  - 임시 데이터 저장소
  - 동일한 데이터에 반복적으로 접근할 때 (데이터 재사용 횟수)
- 단순한 구조 
  - 캐시로 쓰기 좋음
  - 키밸류 구조
  - 모든 데이터를 메모리에 올린다 (그래서 빠름)
  - 평균 작업 속도 1ms 보다 적음
- 캐싱 전략
  - 데이터의 유형과 데이터 액세스 패턴을 잘 고려해서 선택해야 함
  - 읽기 전략
    - Look aside (lazy loading)
      - 가장 일반적으로 사용하는 전략임
      - 어플리케이션은 데이터를 찾을 때 캐시 먼저 확인
      - 캐시에 데이터 있으면 가져오고 없으면 디비에 직접 접근 후 레디스에 올림
      - 레디스 다운되더라도 장애로 이어지지 않고, 디비에서 가져옴
      - 캐시로 붙어 있던 커넥션 많았다면 그 커넥션이 모두 디비로 붙으므로 디비에 부하가 많이 몰릴 수 있음
      - 캐시 새로 투입하고나 디비에만 새로운 데이터 저장했다면 처음에 캐시 미스 많이 발생
        - 이 때는 디비에서 캐시로 데이터 밀어넣는 cache warming 진행
  - 쓰기 전략
    - Write around
      - 디비에만 모든 데이터 저장
      - 캐시 미스 발생하면 캐시에서 데이터 끌어옴
      - 캐시 != 디비 데이터 다를 수 있음
    - Write through
      - 디비 데이터 저장할 때 캐시에도 함께 저장
      - 캐시는 항상 최신 정보 가지고 있을 수 있음
      - 저장할 때마다 두 단계의 스텝 거쳐야 함 
        - 상대적으로 느림
      - 저장하는 데이터 재사용 되지 않을 수 있는데, 무조건 캐시에 올리므로 리소스 낭비일 수도
        - exp time 설정해주는게 좋음 
        - 값의 관리를 어떻게 하는가 -> 장애 포인트가 될 수 있음

## Redis 데이터 타입 야무지게 활용하기

- 자체적으로 많은 자료 구조 제공중
  - 특정 상황에서 어떻게 효율적으로 사용할 수 있을까?
- 제공하는 데이터 타입
  - String
    - set cmd 쓰면 다 string으로 들어감
  - bitmap
    - string의 변형이라고 볼수도 있음 
    - 비트 단위의 연산 가능
    - 데이터 저장공간 절약
    - 정수로 된 데이터만 카운팅 가능 
  - List
    - 데이터 순서대로 저장 
    - 큐로 사용하기 적절
  - Hash
    - 하나의 키 안에 여러 개의 필드 (키밸류 쌍)
  - Set
    - 중복되지 않는 문자열 
  - Sorted set
    - 중복되지 않는 값 저장하지만
    - 모든 값이 score라는 숫자값으로 정렬됨
    - 데이터 저장될 때부터 스코어로 정렬
    - 스코어 같으면 사전순으로 정렬
  - HyperLogLogs
    - 굉장히 많은 데이터 다룰때 쓴다
    - 중복되지 않는 값의 개수를 count할 때 쓴다. 
  - Streams
    - 로그를 저장하기 좋음
- Counting 상황
  - String 
    - 무난한 상황
    - 키 하나를 만들어서 상황마다 증감
    - INCR, INCRBY, INCRBYFLOAT, HINCRBY, HINCRBYFLOAT, ZINCBY
  - Bitmap
    - 1천만명 유저 -> 1천만 비트로 카운팅
      - 1.5MB밖에 안됨 (정수형 유저 아이디에 해당하는 위치를 set)
      - 비트카운트 연산으로 1로 설정된 개수 셀 수 있음
    - id가 sequential한 정수형이어야 함
  - HyperLogLogs
    - 데용량 데이터를 카운팅 할 때 적절 (오차 0.81%)
      - 모든 string data를 unique하게 구분할 수 있음
    - set과 비슷하지만 저장되는 용량은 매우 작음 (12KB 고정)
    - 저장된 데이터는 다시 확인할 수 없음 (??) 
      - 경우에 따라 데이터를 보호하기 위한 목적으로도 쓸 수 있음 
    - 웹사이트 방문한 ip가 몇개가 되는지, 트롤링한 url의 개수, 검색 엔진에서 검색한 유니크한 검색어
    - PFADD, PFCOUNT, PFMERGE
    - 일별 데이터 -> 일주일 (PFMERGE)
- 메세징 상황
  - Lists
    - 메세지 큐로 사용하기 적절
    - Blocking 기능 -> 불필요한 polling 프로세스 막을 수 있음 
    - BRPOP
    - LPUSHX, RPUSHX
      - 키가 있을 때만 데이터 저장 가능
        - 키가 이미 있다는건 예전에 사용되었던 큐라는 뜻 
        - 비효율적인 데이터 이동을 막을 수 있음 
      - 트위터 타임라인에 트윗 캐싱
        - 이미 캐싱되어 있는 피드에만 신규 트윗을 저장
        - 트위터 자주 이용하는 유저에게만 새로운 트윗을 캐시
        - 자주 사용하지 않는 유저는 해싱 키 자체가 존재하지 않음 
          - 데이터 미리 쌓는 비효율적인 작업 안하기
  - Streams
    - 로그를 저장하기 가장 적절한 자료구조
    - append only
    - 중간에 데이터 바뀌지 않음
    - 검색
      - 시간 범위로 검색, 신규 추가 데이터 수신(tail같은것), 소비자별 다른 데이터 수신 (카프카처럼 소비자 개념 존재)
      - 메세징 브로커가 필요할 때 카프카 대체해서 간단하게 쓸 수 있는 자료구조
    - XADD
      - id 위치에 * 를 넣으면 알아서 id 만들고 반환함

## Redis에서 데이터를 영구 저장하려면? (RDB vs AOF)

- 인메모리

  - 서버 재시작시 모든 데이터 유실
  - 복제 기능 사용해도 사람의 실수 발생시 데이터 복원 불가
  - 캐시 이외의 용도로 사용한다면 적절한 데이터 백업 필요

- 영구 저장 옵션

  - aof (append only file)
    - 데이터 변경하는 커맨드 들어오면 그대로 저장
    - 크기가 커짐 (주기적으로 압축해서 재작성하는 과정 거쳐야 함)
    - 레디스 프로토콜 형태로 저장
    - 자동/수동
      - 자동 : redis.conf에서 `auto-aof-rewrite-percentage` 옵션 (크기 기준)
      - 수동 : `BGREWRITEAOF` 커맨드 이용, CLI 창에서 수동으로 AOF 파일 재작성
  - RDB
    - snapshot 방식으로 동작
    - 저장 당시의 데이터를 사진찍듯이 저장, 파일로 저장
    - binary 파일 형태로 저장
    - 자동/수동
      - 자동 : redis.conf 파일에서 `SAVE` 옵션 (시간 기준)
      - 수동 : `BGSAVE` 커맨드 이용해서 CLI 창에서 수동으로 RDB 파일 저장
        - `SAVE` 커맨드는 절대 사용 ㄴㄴ

- 선택 기준

  - 캐시로만 사용하면 저장 굳이 안해도 됨

  - 백업은 필요하지만 어느 정도의 데이터 손실이 발생해도 괜찮은 경우

    - RDB 단독 사용

    - redis.conf 파일에서 SAVE 옵션을 적절히 사용

      - `SAVE 900 1`

        900초동안 1개 이상의 키가 변경되었을 때 RDB 파일을 재작성

  - 장애 상황 직전까지의 모든 데이터가 보장되어야 할 경우

    - AOF 사용 (`appendonly yes`)
    - `APPENDFSYNC` 옵션이 `everysec`인 경우 최대 1초 사이의 데이터 유실 기능 (기본 설정)

  - 제일 강력한 내구성이 필요한 경우

    - RDB & AOF 동시 사용

## Redis 아키텍처 선택 노하우 (Replication vs Sentinel vs Cluster)

- Replication 구성

  - 마스터와 리플리카만 존재
  - 단순히 복제만 연결된 상태
    - 복제는 모두 비동기식으로 동작
      - 마스터에서 복제본의 데이터가 잘 전달되었는지 매번 확인하고 기다리지는 않음 
    - `replicaof` 커맨드를 이용해 간단하게 복제 연결
    - HA 기능이 없으므로 마스터에 장애 상황 시 수동으로 복구해야 할 작업이 많음 
      - 리플리카 노드에 직접 접속해서 복제 끊어야 함 
      - `replicaof no one`
      - 애플리케이션에서 연결 정보 변경 해야 해서 배포해야 하는 작업 필요함 

- Sentinel 구성

  - 마스터와 리플리카 외에 센티널 노드 필요
  - 센티널 : 일반 노드를 모니터링 하는 역할
    - 자동 페일오버 가능한 HA 구성
    - 마스터가 비정상 상태일 때 자동으로 페일 오버
      - 기존의 리플리카가 자동으로 마스터가 됨 
      - 어플리케이션에서 연결 정보 필요 없음
      - 어플리케이션에서는 센티널 노드만 알고 있으면 됨 
      - 센티널이 변경된 마스터 정보로 바로 연결해줌
    - 센티널 프로세스 따로 띄워야 함, 항상 3대 이상의 홀수로 존재해야 함
    - 과반수 이상의 센티널이 동의해야 페일오버 진행 

- Cluster 구성

  - 최소 3대의 마스터 필요
    - 리플리카 노드를 하나씩 추가하는게 일반적인 구성 
  - 모든 노드가 서로를 감시하며, 마스터 비정상 상태일 때 자동 페일오버 
  - 샤딩 기능 제공
    - 키를 여러 노드에 자동으로 분할해서 저장 

- 아키텍처 선택 기준

  - HA 필요? (자동 failover)
    - ㅇㅇ
      - 샤딩 필요? 
        - ㅇㅇ : Cluster
        - ㄴㄴ : Sentinel
    - ㄴㄴ
      - 복제 기능 필요?
        - ㅇㅇ : Replication
        - ㄴㄴ : Stand-Alone

  

## Redis 운영 꿀팁 + 장애 포인트 

- 사용하면 안되는 커맨드 (Reids는 single thread로 동작)
  - `keys *` -> `scan` 으로 대체
    - `keys` 는 모든 키를 보여주는 커맨드
      - 개발 할 때 자주 사용하다가, 운영 환경에서 실수로 사용하게 될 수 있음. 아예 사용하지 않는 것을 권장.
      - `scan`을 사용하면 재귀적으로  key들을 호출할 수 있다. 
  - Hash나 sorted set 등 자료구조
    - 내부에 여러 개의 아이템을 저장할 수 있음. 키 내부에 아이템이 많아질수록 성능이 저하됨.
    - 좋은 성능을 원하다면 하나의 키에 최대 100만개 이상은 저장하지 않도록 나누는 것을 권장. 
    - `hgetall` -> `hscan`
    - `del` -> `unlink`
      - 키에 많은 데이터가 들어있을 때 `del`로 지우면 그 키를 지우는 동안 아무 동작을 할 수 없다. 
      - 이 때 `unlink` 커맨드를 사용하면 키를 백그라운드로 지워주므로 `unlink`사용을 권장
- 변경하면 장애를 막을 수 있는 기본 설정값
  - `STOP-WRITES-ON-BGSAVE-ERROR=NO`
    - default로 yes임
    - RDB 파일 저장 실패 시 redis로 들어오는 모든 write를 차단하는 기능 
      - 레디스 서버에 대한 모니터링을 적절히 하고 있다면 이 기능을 꺼두는 것이 불필요한 장애를 막을 수 있음 
  - `MAXMEMORY-POLICY=ALLKEYS-LRU`
    - redis를 캐시로 사용할 때  expire time 설정하는 것이 좋다.
      - 메모리가 한정되어 있으므로, 이 값을 설정하지 않으면 금새 데이터가 가득 참 
    - 메모리가 가득 찼을 때 이 정책에 의해서 키가 관리된다.
      - noeviction 이 default임 
        - 삭제 안함. 더이상 새로운 키도 저장 안함. 새로운 데이터 입력 불가
      - volatile-lru
        - 가장 최근에 사용하지 않았던 키부터 삭제
        - 이 때 expire 설정이 있는 key 값만 삭제함 
      - allkeys-lru
        - 모든 키에 대해 lru 방식으로  key를 삭제하겠다는 것을 의미 
        - 데이터가 가득 차서 장애가 발생할 가능성이 적어짐
- Cache stampede
  - 대규모 트래픽 환경에서 TTL 값을 너무 작게 설정한 경우 발생 가능
  - look aside 패턴에서는 
    - Redis에 데이터가 없다는 응답을 받은 서버가 
    - 직접 DB로 데이터를 요청한 뒤 
    - 이를 다시 레디스에 저장하는 과정을 거친다.
      - key가 만료되는 순간, 많은 서버에서 이 key를 보고 있었다면, 모든 어플리케이션 서버들이 db에 가서 같은 데이터를 찾게 되는 duplicate read가 발생한다. 
      - 읽어온 값을 레디스에 각각 write 하는 duplicate write로 발생한다. 
      - 이는 굉장히 비효율적인 상황
        - 한번 발생하면 처리량이 느려지고, 불필요한 작업이 늘어나서 장애로 이어질 수 있다. 
  - 운영중 실제 발생한 현상 
    - 티켓 링크의 경우 인기 있는 공연이 오픈되면, 하나의 공연 데이터를 읽기 위해서 몇십 개의 어플리케이션 서버에서 커넥션이 연결된다. 
    - 부하가 발생했을 때 상황을 보기 위해, 개발 쪽의 스카우터 로그(?)를 확인했음 
    - 위 현상이 일어났다는 것을 알아내어 TTL 시간을 넉넉하게 늘려서 해결
- MaxMemory 값 설정
  - 레디스의 데이터를 파일로 저장할 때, 포크를 통해 자식 프로세스 생성한다. 
    - 자식 프로세스로 백그라운드에서는 데이터를 파일로 저장하고 있지만, 원래의 프로세스는 계속 일반적인 요청을 받아 데이터를 처리하고 있다. 
    - 이게 가능한 이유는 cow 기능 덕분. 메모리를 복사해서 사용
  - persistent / 복제 사용 시 MaxMemory 설정 주의
    - RDB 저장 & AOF rewrite 시 `fork()`
    - Copy-on-Write로 인해 메모리를 두배로 사용하는 경우 발생 가능
    - persistence/복제 사용 시 MaxMemory는 실제 메모리의 절반으로 설정하자.
      - 예상치 못한 상황에 메모리가 풀이 차서 장애 발생할 수 있음 
      - e.g.) 4GB -> 2048MB 
    - 복제 연결을 처음 시도하거나, 연결이 끊겨 재시도 할 때 새로 rdb 파일을 저장하는 과정을 거친다. 
- Memory 관리 - 물리적으로 사용되고 잇는 메모리를 모니터링
  - 레디스는 메모리를 사용하는 저장소 이므로, 메모리 관리가 운영에서 제일 중요하다. 
  - `used_memory` 
    - 논리적으로 Redis가 사용하고 있는 메모리
  - `used_memory_rss`
    - OS가 Redis에 할당하기 위해 사용한 물리적인 메모리 양
    - 이 값을 보는 것이 더 중요하다. 실제 저장된 데이터는 적은데 Rss 값은 큰 상황이 있을 수 있음. 이 차이가 클 때 fragmentation이 크다고 함.  
  - 주로 삭제되는 키가 많을 때 fragmentation이 증가한다.  
    - 특정 시점에 피크를 찍고 다시 삭제되는 경우
    - TTL로 인한 eviction이 많이 발생하는 경우 
  - 이 때 `activefrag`라는 기능을 잠시 켜두면 도움이 된다. 
    - 공식 문서에서도 이 값을 항상 켜두는 것 보다는 단편화가 많이 발생할 때 켜는 것을 권장. 
