#### 스프링 부트의 배포 옵션

스프링 부트 애플리케이션을 실행, 배포하는 방법은 다음과 같다. **메이븐, 혹은 그래들을 이용해서** 애플리케이션을 **<u>빌드 및 실행</u>** 혹은 **<u>실행 가능한 파일을 생성</u>**하는 것이다.

1. 애플리케이션을 빌드 및 실행하거나

   - IDE(STS, 인텔리제이 등)에서 애플리케이션을 빌드하고 실행

   - 메이븐의 `springboot:run`, 혹은 그래들 `bootRun` 태스크로 빌드 및 실행

2. 실행 가능한 파일을 생성하기

   - 메이븐이나 그래들을 사용해서 <u>실행 가능한</u> JAR 파일 생성

     (<u>명령행에서 실행</u>되거나 클라우드에 배포될 수 있음)

   - 메이븐이나 그래들을 사용해서 WAR 파일 생성

     (자바 애플리케이션 서버에 배포될 수 있음)

프로덕션 환경에서는 2번 방법을 이용하여 애플리케이션을 배포한다. 그렇다면, JAR과 WAR 중 어느 파일 형식을 선택해야 할까? 이는 배포하는 환경에 따라 선택한다.

1. 자바 애플리케이션 서버에 배포하는 경우 (Tomcat, WebSphere, WebLogic 등)

   WAR 파일을 선택한다.

2. 클라우드에 배포하는 경우 (Cloud Foundry, AWS, Azure, Google cloud platform 등)

   <u>실행 가능한</u> JAR 파일을 선택하는 것이 좋다. 

   > WAR 형식보다 JAR 형식이 훨씬 간단하므로 클라우드 플랫폼에서 WAR 파일 배포를 지원하더라도, JAR 파일으로 배포하는 것이 좋다.

##### 📌 이번 프로젝트에서는 AWS를 사용하므로 JAR 파일 배포를 선택한다.





#### 프로젝트 빌드 및 실행하기

애플리케이션 실행하기

```shell
java -jar demo.jar // JAR 파일인 경우
java -jar demo.war // WAR 파일인 경우
```



#### AWS EC2 생성 및 설정

1. 아마존 리눅스 서버를 선택, 스토리지는 30GB혹은 그 이상으로 설정 (30GB는 프리 티어에서 사용할 수 있는 최대 용량임)

2. 태그 추가

   키는 `Name`으로 값은 `Baechoo-market` 등의 서비스 이름으로 설정

3. 보안 그룹 생성

   인바운드 그룹에 규칙을 추가한다.

   > 1. SSH, TCP, 포트 22로 서버에 접속할 IP 추가 (터미널에서 pem 사용하여 접근 시 필요)
   > 2. 사용자 지정 TCP, TCP, 포트 8080(혹은 다른 프로젝트 기본 포트)으로 `0.0.0.0/0, ::/0` 입력
   > 3. HTTPS, TCP, 포트 443으로 `0.0.0.0/0, ::/0` 입력

4. 인스턴스 생성, 키 페어 생성

5. 고정 IP 할당해야 함 

   > 1. EIP(탄력적 IP) 탭으로 들어간 후 새 주소를 할당받는다.
   > 2. 해당 EIP를 선택하고 우리의 서버 인스턴스와 주소 연결한다.

6. 이제 터미널로 접속할 때에는 이 아이피를 사용한다.

   ```shell
   ssh -i [pem 키의 위치] [EC2의 EIP 주소]
   ```

   [윈도우에서 접속](https://m.blog.naver.com/alice_k106/220882708567)

7. [pem키 접속 간소화 + 추가로 해야 할 일](https://ict-nroo.tistory.com/135)

   - java 8 설치
   - Timezone 서울로 변경
   - Hostname 변경, 등록



#### 서버에 프로젝트 배포하기

1. 프로젝트 clone 받기

   > step1 폴더는 다른 이름으로 써도 됨

   ```shell
   sudo yum install git
   git --version
   mkdir ~/app && mkdir ~/app/step1
   cd ~/app/step1
   git clone [우리의 레포 주소]
   cd [우리의 레포]
   ```

2. 테스트 코드 수행 및 검증

   ```shell
   chmod +x ./gradlew /* 실행 거부될 시 */
   ./gradlew test
   ```

3. 배포 스크립트 만들기

   다음 과정을 모두 수행하는 스크립트를 만들자

   - git clone 혹은 git pull을 통해 새 버전의 프로젝트 받기
   - gradle이나 maven을 통해 프로젝트 테스트와 빌드
   - EC2 서버에서 해당 프로젝트 실행 및 재실행

   스크립트 파일을 생성한다.

   ```sh
   vi ~/app/step1/deploy.sh
   ```

   생성한 스크립트 파일에 다음 코드를 추가한다.

   ```shell
   REPOSITORY=/home/ec2-user/app/step1
   PROJECT_NAME=[우리의 프로젝트 레포 이름]
   
   cd $REPOSITORY/$PROJECT_NAME
   
   echo "> Git Pull"
   
   git pull
   
   echo "> 프로젝트 Build 시작"
   
   ./gradlew build
   
   echo "> step1 디렉토리로 이동"
   
   cd $REPOSITORY
   
   echo "> Build 파일 복사"
   
   cp $REPOSITORY/$PROJECT_NAME/build/libs/*.jar $REPOSITORY/
   
   echo "> 현재 구동중인 애플리케이션 pid 확인"
   
   CURRENT_PID=$(pgrep -f ${PROJECT_NAME}.*.jar)
   
   echo "현재 구동 중인 애플리케이션 pid: $CURRENT_PID"
   
   if [ -z "$CURRENT_PID"]; then
   	echo "> 현재 구동 중인 애플리케이션이 없으므로 종료하지 않습니다."
   else
   	echo "> kill -15 $CURRENT_PID"
   	kill -15 $CURRENT_PID
   	sleep 5
   fi
   
   echo "> 새 애플리케이션 배포"
   
   JAR_NAME=$(ls -tr $REPOSITORY/ | grep jar | tail -n 1)
   
   echo "> JAR name: $JAR_NAME"
   
   nohup java -jar $REPOSITORY/$JAR_NAME 2>&1 &
   ```

   > nohup는 사용자가 터미널 접속을 끊어도 애플리케이션이 계속 구동되도록 해줌

   스크립트에 실행 권한 추가 후 실행, 로그 확인

   ```shell
   chmod +x ./deploy.sh
   ./deploy.sh
   vi nohup.out
   ```

   

