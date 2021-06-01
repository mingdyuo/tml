## 토스 서비스를 구성하는 서버 기술

[영상](https://www.youtube.com/watch?v=YBXFRSAXScs&t=204s)

1. 토스 서비스를 구성하는 서버 기술들

   - 2개의 데이터 센터에서 active-active로 서비스를 운영하고 있으며, AWS의 일부 서비스를 이용하고 있다. 

     2개의 데이터 센터를 운영하다 보니 여러 컴포넌트에서 운영 이슈가 발생한다. 이에 대한 대응 방법을 소개하겠다.

     <img src="https://user-images.githubusercontent.com/41130448/116890615-c795ed80-ac68-11eb-9105-aea8ab815f88.png" alt="image" style="zoom: 25%;" />

     <img src="https://user-images.githubusercontent.com/41130448/116890689-d9779080-ac68-11eb-8637-e2f27dd0809a.png" alt="image" style="zoom: 25%;" />

     <img src="https://user-images.githubusercontent.com/41130448/116890979-2b201b00-ac69-11eb-8a37-a39ea6a31102.png" alt="image" style="zoom:25%;" />

     <img src="https://user-images.githubusercontent.com/41130448/116891274-789c8800-ac69-11eb-88b9-464d96f71b49.png" alt="image" style="zoom:25%;" />

     <img src="C:\Users\pc\AppData\Roaming\Typora\typora-user-images\image-20210503234645274.png" alt="image-20210503234645274" style="zoom:25%;" />

   - 토스는 MSA 구성이 되어 있어서 많은 서버가 뜨게 된다. 그런 많은 서버를 잘 관리하고 효율적인 **인프라** 활용을 위해 쿠버네티스를 container orchestration으로 도입했다. 이와 함께 calico cni와 service mesh로 Istio를 사용하는 중이다. 또한 민감자료 저장 용도로 ceph를 이용해 내부 스토리지 서비스를 제공하고 있다.  

   - 예전에는 Memcached를 **캐시**로 많이 사용했었는데, 데이터 보존 문제와 optimistic lock 구현 편이성 때문에 현재는 Redis cluster를 캐시로 더 많이 사용하는 중이다. 

     Kafka는 로그 데이터 파이프라인으로 사용하는 로그용 클러스터 하나, 서비스에서 메세지 큐로 사용하는 클러스터 하나를 운영 중이다. 

     로깅과 모니터링으로는 elk + filebeat, thanos, grafana를 사용하고 있다. 

   - 토스에는 은행과 연동되는 송금 외에 많은 서비스가 존재한다. 

     예를 들면 송금 이후에 채팅을 할 수 있는 서비스, 카드사 알림을 토스로 받는 서비스, 은행 대출 추천서비스처럼 외부 연동이 많은 서비스도 있다.

     또한 홈 탭이나 내소비 처럼 여러 서비스의 데이터를 합쳐서 보여주는 서비스도 있다. 

     이런 여러 서비스의 특성에 맞춰서 **Spring** 계열 프로젝트를 선택하여 개발하고 있다. 개발 언어로는 예전에는 Java를 많이 썼지만, 최근 개발되는 소스들은 Kotlin을 사용하고 있다. 

   - **AWS**는 DNS 설정을 하거나 이미지 검수, static file 서빙하는 기능 등으로 사용하고 있다. 

2. 데이터 센터 트래픽 조절

   - 토스는 데이터 센터 2개를 운영하며 트래픽을 돌릴 수 있다. 시스템을 안정적으로 운영하기 위함. 

     Route53 → (DC 분기) → L7 → L7 → K8S

     <img src="https://user-images.githubusercontent.com/41130448/120100937-5f123180-c17e-11eb-9906-310a506c31ec.png" alt="image" style="zoom:67%;" />

   - 평소에는 50:50으로 운영하나, 한쪽이 100%가 되는 트래픽을 옮기는 작업을 자주 한다. 

     따라서 devops 팀에서 자동화도 많이 해두었다. 

   - 트래픽을 한쪽으로 옮기는 이유

     1. 장애가 났을 때 반대편 데이터 센터로 트래픽을 옮겨 장애 시간을 줄인다. 

     2. 새로운 시스템 도입이나 쿠버네티스 설정 변경 시 테스트 범위와 예상 밖의 문제가 발생할 수 있으므로 트래픽을 한쪽으로 옮겨놓음. 

        새로운 설정으로 1%의 트래픽을 받은 후 문제가 없다면 점진적으로 늘린다. 문제가 발생하면 바로 트래픽을 제거한다. 장애를 겪는 고객 수를 최소화하는 것이 목적 

        서비스 카나리 배포를 데이터 센터에 적용했다고 생각할 수 있다. 

   - 트래픽 방향은 Route53과 L7에서 전환할 수 있다. 

     1. L7에서 트래픽을 돌릴 경우 모든 트래픽이 곧바로 옮겨지는 장점이 있다. 따라서 대부분에서는 L7에서 돌린다. 
     2. Route53의 경우에는 변경 사항이 다른 라우터에 전파가 된 후에 반영되므로 시간이 오래 걸리는 단점이 있다. (?? 이해가 잘 안된다)

3. K8S + Istio

   - 19년도 중순에 DC/OS에서 K8S로 마이그레이션하기로 함

   - Container orchestration에서 가장 중요한 것

     1. Service discovery

        DC/OS에서 카나리 배포로 사용하던 vamp는 이 기능이 비효율적이고 한계가 존재했음. 그래서 마이그레이션을 결정함.

     2. Container lifecycle management

   - DC2에서 K8S를 도입, DC1에서는 DC/OS를 동시에 운영했다. 그리고 DC1의 DC/OS 일부 노드를 제거하고 쿠버네티스를 설치. 혹시 모를 문제 발생 시 롤백을 하기 위해서 조금씩 작업함. 

   - DC1, DC2 모두에서 쿠버네티스를 운영해보고 안정적으로 운영할 수 있다고 판단한 후 DC1, DC2에서 모두 K8S를 운영중

   ![image](https://user-images.githubusercontent.com/41130448/120101217-fa57d680-c17f-11eb-9874-01c1c21b5254.png)

   - 쿠버네티스 도입 시 service mesh의 장점이 크다고 판단된 Istio도 함께 도입함

     1. monolithic에서 msa로 넘어가게 되면서 하나의 서비스는 여러 서버들의 호출로 구성되었다.

        그러면서 서버들 간에 network 처리가 필요하게 되었다.

        circuit breaker, retry, fallback 등을 애플리케이션에서 처리했다. 

     2. Istio를 도입하면 istio-proxy가 sidecar 형태로 붙어서 모든 네트워크를 proxy하게 된다. 

        이 과정에서 애플리케이션의 일을 대신할 수 있기 때문에 애플리케이션의 언어와 무관하게 개별 어플리케이션에서 처리하는 것이 아니라 infra 차원에서 한번에 해결할 수 있다. 

     3. Client side load balancing

        msa는 외부에서 내부로 들어오는 요청보다 내부 서비스 간의 요청이 많다. 

        이 경우 내부 요청을 중앙 집중 형태로 처리한다면 장애에 취약한 부분이 생기고, 부하도 많이 받는다. 

        Istio에서는 이런 내부 요청이 중앙 집중 형태가 아닌 client side에서 처리할 수 있다. 

     4. dc/os보다는 k8s가 더 넓은 생태계를 가지고 있고, 더 큰 규모의 서비스에서 검증되기도 했다. 

   - Istio 도입 후 느낀 점

     1. Observability가 높아졌다.

        - Istio는 서비스에 sidecar로 붙어서 실행되고 iptable을 통해 모든 트래픽을 제어한다. 이렇게 제어된 트래픽은 모두 istio-proxy를 통해서 나가게 되고 또 istio-proxy를 통해서 받게 된다. 

        - Istio-proxy는 이런 envoy proxy를 Istio에서 래핑한 것이다. 

          envoy는 탄생 배경 중 하나가 'proxy가 어떻게 동작하는지 명확히 보여주자'이므로 많은 통계를 가지고 있다. 예를 들면 connection pool 동작 방식, 어디에서 connection이 끊겼는지 등을 통게로 확인할 수 있다. 이런 통계를 보내는 쪽과 받는 쪽을 합쳐서 보면 문제의 원인이 되는 곳을 파악하기가 쉽다. 

          기존에 tcpdump로 네트워크 문제를 해결할 때 보내는 쪽과 받는 쪽 모두 dump를 떠서 확인했던 것을 대신해주는 개념이다.

        ![image](https://user-images.githubusercontent.com/41130448/120101575-a1893d80-c181-11eb-9b94-f19ae1deb96e.png)

        2. Application Concern을 Infra concern으로 변경하기 어려운 부분도 존재한다. 

           - Istio를 사용하면서 infra 쪽으로 네트워크 처리를 넘겨보려고 했지만 circuit breaker는 host 별로 설정을 해야 하므로 세부적인 설정이 어려웠다. 

             예를 들면 retry도 api의 트랜잭션 처리 방식과 응답값에 따라 시도 여부가 달라진다. 

             세부 설정을 하기 어려우므로 이런 부분은 어플리케이션 단에서 처리한다. 

           - 대신 mTIs 설정을 통해 A 서비스는 B를 호출하고, C는 B 호출을 할 수 없도록 서비스별로 권한을 다르게 적용시킬 수 있다.

             혹은 envoy filter를 작성하여 애플리케이션을 변경하지 않고 dynamic하게 네트워크 기능을 추가할 수 있다.

             토스는 envoy filter로 헤더 처리와 접근 제어를 하고 있다.

           ![image](https://user-images.githubusercontent.com/41130448/120101687-3b50ea80-c182-11eb-8eca-5868aaf35ec7.png)

   - Istio 장점

     1. 1% 카나리 배포 기능

        - 인스턴스 단위로 카나리를 하면 트래픽을 세밀하게 조절하기 어렵다. 

          예를 들어 10개 인스턴스에서 1개만 카나리를 나가면 10% 카나리가 된다. 1% 카나리 배포를 하려면 100개의 인스턴스가 필요하다. 

        - Istio는 router의 weight으로 카나리를 할 수 있으므로 인스턴스 개수와 무관하게 1% 카나리 배포가 가능하다. 

        ![image-20210530200712375](C:\Users\pc\AppData\Roaming\Typora\typora-user-images\image-20210530200712375.png)

     2. failure injection test, squeeze test를 할 수 있다.

        - failure injection test

          특정 조건에 맞는 요청의 경우 실패나 응답을 늦게 하도록 만들어 서비스에서 실패에 대한 처리가 제대로 되어 있는지 확인할 수 있다. 

        - squeeze test

          하나의 인스턴스에 요청 비율을 높혀서 부하를 약간 주는 테스트

          라이브에서 진행되므로 서비스의 최대 성능을 알아내기보다는 어느 정도 요청부터 서비스가 부하를 느끼는지 알아보고 지표를 비교하며 새로운 배포로 인해서 성능 하락이 있는지 확인할 수 있다. 

4. api-gateway

   - 도입 배경

     원래 클라이언트의 인증과 암호화/복호화를 담당하는 서비스가 존재했다. 그러나 점점 트래픽이 많아지면서 성능을 높일 필요성이 대두되었다. 

     따라서 신규 gateway 도입을 검토했고, zuul, kong, spring cloud gateway 와 같은 후보가 있었으며 이 중 spring cloud gateway를 선택했다. 

   - spring cloud gateway를 선택한 이유

     WebFlux 도입을 하면서 reactor에 대한 운영 자신감이 생김

     성능이 검증되어 있음

   - 추가 설명

     1. api-gateway는 트래픽을 여러 필터로 처리하는데 토스에서는 custom filter를 만들어서 [기존 인증/oauth2/암복호화]를 구현함

     2. gateway에 라우팅은 static이나 dynamic으로 추가할 수 있다. 

        그러나 dynamic의 경우에는 routing 검증 프로세스가 아직 확립되지 않아서 대부분 static으로 추가한다. 

     3. spring의 내부 코드가 reactor이지만 유지보수를 편리하게 하기 위해서 코루틴을 사용하고 있다. 

   ![image](https://user-images.githubusercontent.com/41130448/120101968-d5fdf900-c183-11eb-840e-a4d2c30bf68a.png)

   - 처음에는 하나의 gateway로 서비스를 했는데, gateway에 요청을 보내는 서비스들이 다양해졌다. 클라이언트에 맞는 처리를 해줘야 하면서 gateway가 무거워지기 시작했다.

     따라서 각 클라이언트에 대응하고 gateway 로직을 분리하기 위해서 새로운 gateway를 추가했다. 

     Netflix에서는 gateway를 개발하면서 어느 팀이 gateway를 관리할지, 공통 로직을 어떻게 처리할지 고민을 많이 했다고 한다. 

     현재 토스는 각 팀의 PR을 받아 platform 팀에서 승인하는 구조로 관리하고 있다. 공통 로직의 경우에는 모듈화 해서 처리하고 있다. 

     ![image](https://user-images.githubusercontent.com/41130448/120102083-5c1a3f80-c184-11eb-9d7d-b17230a7b5e3.png)

5. WebFlux

   - 토스의 많은 서비스들은 mvc로 되어 있다. 그러나 홈탭이나 내 소비와 같이 여러 데이터를 모아서 보여주는 I/O가 많은 프로젝트에서는 WebFlux를 이용하고 있다. 
   - 최근에서는 스프링에서 코틀린 dsl을 잘 지원하고 R2DBC를 사용하면서 많은 프로젝트가 WebFlux 로 시작하고 있다. 
   - 초기에는 WebFlux를 reactor로 개발했다. reactor를 사용하면 지금까지 알던 direct style로 control flow를 짜는 것이 아니라 reactor가 제공하는 operator를 사용해야 한다. reactor에서 제공하는 가이드를 통해 각 경우에 사용하는 operator를 따로 살펴보고 테스트를 통해 의도된 동작인지 확인해야 한다. 또한 비즈니스 로직까지 추가해야 한다. 이 때문에 조직 이동이나 신규 입사시에 러닝커브가 크기 때문에 코루틴을 도입했다. 코루틴을 사용하면 기존처럼 direct style로 개발을 할 수 있다. 

6. monitoring (로그 시스템)

   - 오픈 소스를 많이 사용하여 모니터링한다. 

   - msa 구조 때문에 여러 곳에 서버가 존재하므로 log를 중앙에 집중시킬 필요가 있다. 

     logback과 filebeat을 통해 애플리케이션 로그를 Kafka로 보내고 Kibana에서 검색해 볼 수 있다.

   - 로그에는 메시지와 함께 다양한 데이터가 있다.

     - 컨테이너 ID, 서비스 ID로 어느 데이터센터에서 어느 컨테이너가 남긴 로그인지 확인할 수 있다.

       이 데이터를 통해 특정 장비의 문제인지, 혹은 특정 서비스의 문제인지 구분할 수 있다. 

     - 배포 ID도 로그에 남겨서 어느 배포 버전에서 발생한 문제인지 알 수 있다. 에러 로그가 신규 배포에만 발생한다면 바로 롤백한다. 

     - MSA 구조에서는 문제 발생의 원인이 요청을 받는 서비스일 수도 있고, 요청을 주는 서비스일 수도 있다. 전체적으로 어느 서비스에서 발생한 문제인지 확인하기 위해서 Request ID도 함께 쌓고 있다.

     - apm으로 pinpoint를 사용한다. 해당 요청이 pinpoint의 어떤 요청과 매칭되는지 확인하기 위해서 pinpoint ID도 같이 쌓는다.

     - 금융회사로 5년간 장기 보관해야 하는 데이터가 있다. 이런 경우에는 Kafka에서 하둡으로 데이터를 쌓아서 보관한다. 

   - 데이터 센터급 장애가 발생하더라도 로그를 확인할 수 있어야 하므로, 이중화되어있다. 

     ES 경우에는 각 데이터 센터 별로 50대씩, 총 100대를 운영한다. 

     하루에 15TB 정도의 로그가 쌓이고 있다. ( ㄷㄷㄷㄷㄷㄷㄷㄷㄷ;;;; )

   ![image](https://user-images.githubusercontent.com/41130448/120320162-d41b6d80-c31c-11eb-8add-ea367fd7e381.png)

   - metric으로는 쿠버네티스와 잘 맞는 prometheus를 사용한다. 

     HA와 데이터 장기보관을 위해서 thanos를 사용한다.

   - 데이터센터별 thanos에 쿼리를 하기 위해서 global thanos가 따로 존재한다. 

   - ceph를 내부 저장소로 사용하며 grafana로 데이터를 확인한다. 

     grafana에 있는 대시보드와 알림 데이터들도 중요하므로 저장소로 사용하는 mysql은 서비스 DB와 마찬가지로 이중화가 되어 있다. 

   ![image](https://user-images.githubusercontent.com/41130448/120320487-34121400-c31d-11eb-8048-3505e07ae4e9.png)

   - 알림

     - 다양한 알림을 통해서 시스템 문제 상황을 파악한다. 

     - 개별 서비스에서는 sentry로 설정한다.

       전체적인 관리는 es로그를 활용하는 프로젝트를 만들어 사용하고 있다. 

     - 에러나 워닝 로그가 많이 발생하거나, 특정 로그가 많아지는 경우 Slack으로 알림을 준다.

       grafana에도 알림이 걸려있어서 http error code가 발생하거나 Istio error flag가 발생하면 Slack으로 알림을 준다. 

     ![image-20210601210850497](C:\Users\pc\AppData\Roaming\Typora\typora-user-images\image-20210601210850497.png)

     - flink

       서비스들의 응답을 파싱해서 응답 시간이 튀거나 응답 실패율이 높아지는 경우에도 알림을 보낸다.

7. kafka

   - 토스에 데이터 센터가 2개일 때 카프카 운영을 안정적으로 운영하기 위해 고민했다. 
     1. 카프카에는 자체적으로 HA 기능이 있으므로 하나의 클러스터로 두 개의 데이터 센터를 묶는다. 
     2. 다른 클러스터로 구성해 relication을 건다. 

   - 우버에서 사용하고 있는 방식을 보고 두 개의 클러스터로 운영하는 것이 좋겠다고 판단함. 

     1. 하나의 클러스터로 운영 시 데이터 센터급 장애가 발생하면, 다른 센터에 있는 서비스에서도 일부 장애가 발생할 수 있다.
     2. 두개의 클러스터로 운영 시 다른 데이터 센터에서는 장애가 발생하지 않는다. 따라서 트래픽을 옮기면 빠르게 복구할 수 있다.

   - 두 개의 클러스터를 운영하면 추가적인 기능이 필요하다. 

     active/stanby인 consumer가 다른 데이터 센터로 active하게 넘어갈 때, 두 개의 카프카는 consumer offset이 다르기 때문에 어느 지점부터 consume을 할지 모른다. 

     따라서 두 개의 kafka offset을 sync해주는 애플리케이션이 필요하다. 

     토스에서는 데이터플랫폼 팀에서 wakuwaku라는 이름으로 sync 애플리케이션을 개발하여 사용중이다. (ㅋㅋㅋㅋㅋ와쿠와쿸ㅋ)

     producer는 active/active라서 장애 발생 시 추가적인 대응이 필요하지 않다. 

     consumer는 active/stanby로 평소에는 한 쪽 데이터센터의 Kafka를 바라보다가 장애가 발생하면 반대편 데이터센터의 Kafka를 바라보도록 변경시키고 장애에 대응한다. 

     > active/active, active/standy가 모지

   ![image-20210601211643111](C:\Users\pc\AppData\Roaming\Typora\typora-user-images\image-20210601211643111.png)

   - consumer에서 메시지 처리를 실패하는 부분을 어떻게 처리할지 고민했다. 

     우버에서 사용하는 topic별 retry와 dead letter를 본떠서 spring Kafka를 래핑하여 구현했다. 

     같은 토픽에서 retry로 처리 시간이 오래 걸리면 뒤에 있는 메시지 처리가 밀린다. 따라서 다른 topic에서 retry를 하는 것이 성능면에서 좋다. 

   - requeue를 할 때 delay는 필연적으로 따라온다.

     consume하는 서비스가 정상화가 되기 전에 requeue를 하면 다시 에러가 발생하기 때문이다. 

     topic이 나뉘어져야 retry할 때 delay를 주는 것도 자유롭다. (토픽이 뭐징???)

   - 토스는 topic별로 자동 delay requeue를 설정할 수 있고, dead letter topic에서는 수동 배치로 requeue하고 있다. 

   ![image](https://user-images.githubusercontent.com/41130448/120322251-278ebb00-c31f-11eb-866b-40ba5d05d6cd.png)

   - producer 쪽에서는, Kafka A에서 실패할 경우 Kafka B로 보내서 retry하도록 처리했다. 

     여러 종류의 Kafka를 운영하고 있기 때문에 서로 백업으로 사용할 수 있다. 

   - producer는 대부분 바로 재발행 하기 원하므로 약간의 delay 후에 자동으로 requeue를 시도한다. 

     자동 requeue도 실패할 경우 dead letter topic으로 이동해서 수동 배치로 requeue를 하고 있다. 

   - 메시지 발행은 DB 처리 후 이벤트 발행용으로 많이 사용하고 있다. 

     DB와 Kafka 이기종 트랜잭션에 대한 고민을 해결하기 위하여 Kafka cluster 이중화로 대응했다. 

     하지만 여전이 하나의 트랜잭션이 아니다. 완벽한 트랜잭션 처리가 필요한 서비스의 경우에는 outbox 패턴을 통해서 동일 트랜잭션 처리를 하고 retry를 통해 전송한다. 

   ![image](https://user-images.githubusercontent.com/41130448/120322783-c0253b00-c31f-11eb-8b62-5f31b9a33d95.png)

8. redis

   - 예전에는 Memcached를 사용했지만 지금은 Redis cluster를 사용한다. 

   - cluster이므로 데이터 보존이 용이하다. 

     장기 보관이 필요없는 단발성 데이터 저장과 캐시로 사용하고 있다. 

   - 현재 데이터 센터가 2개밖에 없어서 한 쪽에 master가 많다.

     데이터 센터 간 네트워크가 끊겼을 때 split brain이 발생해 양쪽 다 master가 되는 상황이 발생할 수 있다. 

     이런 부분은 인프라 이중화로 대응하고 있으며, 3번째 데이터 센터도 준비하고 있다. 

   - 데이터센터가 2개라서 한쪽이 장애가 나면 반대편에서도 master 3대, slave 3대를 구성하기 위해 각각 6대씩 총 12대로 하나의 클러스터를 구성했다. 

     3번째 데이터 센터가 생긴다면 master 1대, slave 2대를 각 데이터 센터별로 구성해 9대로 클러스터를 구성할 계획이다. 

   - 운영하다보면 메모리 확장이 필요하다. 그 때마다 slot rebalance로 대응한다. 

     AWS에서는 분산락으로 redlock을 구현해서 이용한다. 

     데이터센터에서는 3번째 DC가 완성되면 redlock을 적용할 예정이다.

   - Redis client로 사용하기 위해 고민한 후보

     1. redisson
     2. jedis
     3. lettuce → 선택함

   - lettuce 선택한 이유

     - WebFlux와 mvc에서 공통으로 사용할 수 있음
     - spring에서 잘 지원해줌

   ![image-20210601213136570](C:\Users\pc\AppData\Roaming\Typora\typora-user-images\image-20210601213136570.png)

9. 마치며

   토스 플랫폼은 전통적인 금융 기업보다는 인터넷 기업과 비슷한 구조를 갖는다. 

   신기술이라고 무조건 도입하지 않는다. 문제 정의를 명확히 하고 나서 기술 도입을 결정한다. 

   앞으로 발전해야 할 일이 계속 있는데, 지금처럼 여러 회사에게 배우며 지속적으로 성장할 예정이다. 

   멀티 데이터 센터를 운영하는 엔지니어에게 도움이 되기를 ~

