### `std::chrono::high_resolution_clock` 와 시간재기

- `std::chrono::time_point now()` 함수로 해당 시점의 시각을 가져온다. 

  해당 플랫폼에서 가장 짧은 단위의 시각을 가져온다.

  ```c++
  high_resolution_clock::time_point Time = high_resolution_clock::now();
  ```

- 시간 종류

  1. 물리적 고정 시간 - `steady_clock::now()`
  2. 컴퓨터 시스템 시간 - `system_clock::now()`

- 시간 계산하기

  시간차 값에 `duration_cast를 해줘야 한다.`

  ```c++
  nanoseconds durationSeconds = duration_cast<nanoseconds>(endTime - startTime);
  seconds durationSeconds = duration_cast<seconds>(endTime - startTime);
  minutes durationSeconds = duration_cast<minutes>(endTime - startTime);
  ```

<br>

### `queue::empty()`는 thread-safe할까?

[이 사람의 말](https://stackoverflow.com/questions/4355399/why-is-stdqueueempty-not-thread-safe-shouldnt-const-functions-be-thread)에 따르면

다른 어떤 메소드가 **절대 변화시키지 않는 객체**에 대해서'만' thread-safe하다고 한다. 근데 queue 같은 경우에는 상태가 계속 변하니까 `empty()`에도 락을 걸지 않으면 race condition 유발하겠쥬?

<br>

### 스마트 포인터와 RAII

[공부한 원 소스](https://modoocode.com/229)

- RAII 디자인 패턴
  - 자원의 획득은 초기화다 (Resource Acquisition Is Initialization)
  - C++ 맹근 사람이 제안함
  - **자원 관리**를 **스택에 할당한 객체를 통해 수행**하는 것.

- 포인터 객체를 만들어서, 이 객체가 소멸될 때 사용한 자원을 해제하는 개념이다.

- 메모리를 동적으로 할당할 때 발생할 수 있는 문제들

  1. 해제를 제대로 안할 시 메모리 누수 발생

     간단한 프로그램은 사실상 문제가 안된다. 그러나 서버처럼 장시간 작동하는 프로그램은 시간이 지날수록 점점 사용하는 메모리의 양이 늘어나서 시스템 메모리가 부족해진다. 결과는 으앙 쥬금 

  2. 이미 해제된 메모리를 다시 참조하는 경우 (다시 해제하는 경우)

     해제된 메모리를 다시 해제하면 `double free` 라는 버그가 발생하여 프로그램이 으앙 쥬금

- 그래서 이런 문제들이 왜 발생하냐면, 객체의 소유권이 명확하지 않기 때문이다. 

  포인터 객체는, 유일한 소유권을 가지고 있으므로 소멸시킬 권리도 얘만 가지고 있다. 따라서 `double free` 버그 발생을 예방한다.

  → `unique_ptr`

<br>

### `std::unique_ptr`

- 복사생성자가 명시적으로 삭제되었다.

  (`C++11`에서는 원치 않는 함수를 다음과 같이 명시적으로 삭제할 수 있다.)

  ```c++
  class A {
   public:
    A(int a){};
    A(const A& a) = delete; // 복사 생성자 삭제, 사용 불가능
  };
  ```

  따라서 `std::unique_ptr` 는 객체 복사가 불가능하다.

- 복사는 안되지만, 소유권 이전은 가능하다.

  ```c++
  std::unique_ptr<T> pta(new T());
  std::unique_ptr<T> ptb = std::move(pta);
  ```

  이렇게 되면 `new T()`로 생성된 객체는 `ptb`가 갖게 되고, `pta`는 아무것도 가리키고 있지 않는다.

  → dangling pointer가 되었다.

  ```c++
  pta.get();
  ```

  으로 가리키는 실제 주소값을 확인하면 `0 (nullptr)`이 나온다. 

  소유권 이동 이후에 이전의 `unique_ptr`을 **절대** 접근하지 않도록 주의하자 !

- 함수에 넘길 때에 `unique_ptr`의 레퍼런스를 사용하는 것은 적절하지 않다. 소유권을 공유하게 되는 것과 마찬가지이다. 단순히 `Wrapper`로 사용하게 되는 것이다.

  함수에 올바르게 `unique_ptr`을 전달하는 방법은 원래의 포인터 주소값을 전달하는 것이다. 단순히 view 하는 경우에는 `const`로 받도록 하자
  
- 사용하기 적합한 경우

  - 클래스 생성자/소멸자
  - 지역변수
  - STL 벡터에 포인터 지정하기

- `unique_ptr`의 생성자는 `noexcept`로 선언되어서 절대로 예외를 던지지 않는다. 따라서 `new`를 했을 때 처럼 메모리 할당에 성공했는지 여부를 확인할 필요가 없다. 만약 메모리 할당이 실패하면 바로 `std::terminate`가 호출된다.

- 스마트 포인터를 생성할 때에는 `C++14`의 `std::make_unique`, `std::make_shared`를 적극적으로 사용하자.

### `std::vector::emplace_back()`

- 일반적으로 `push_back()`보다 성능적으로 뛰어나지만, 잘못 쓰는 경우 큰일날 수 있다.

  ```c++
  std::vector<int> numVec;
  numVec.emplace_back(1 << 16);
  ```

  65536 값을 갖는 `int`를 `numVec`에 단순히 추가하는 구문

  ```c++
  std::vector<std::vector<int>> confusingVec;
  confusingVec.emplace_back(1 << 16);
  ```

  "크기"가 65536인 `std::vector<int>`를 `confusingVec`에 추가하는 구문.

  이 컨테이너가 살아 있는 동안 메모리 소모가 엄청 늘어날 것이다. 이 경우에는 컴파일에 문제가 없으므로 버그를 잡기가 어렵다.

- 따라서 `emplace_back()`을 사용할 때에는 타입 `T`의 생성자가 뭐가 실행되는지 **반드시** 염두해야 한다.

<br>

### 복사 생략 (copy elision)

- 임시 객체를 만들고 복사 생성해야 하는 경우, 컴파일러가 알아서 만들어진 임시 객체 자체를 가지고 가는 것.
- 생성자가 중복으로 호출되는 것을 방지한다.

<br>

### `constexpr` 키워드

- `C++11`에서 도입

- 객체나 함수 앞에 붙힐 수 있는 키워드. 해당 객체나 함수의 리턴값을 컴파일 타임에 알 수 있다는 의미를 전잘한다.

  → 해당 객체가 상수이라는 뜻 (특히, 값이 정수인 경우 정수 상수식임)

- `const`는 컴파일 타임에 그 값을 알 필요가 없다. `constexpr`의 경우에는 컴파일 타임에 반드시 다른 상수식이 오른쪽에 와야 한다.

- `constexpr`는 항상 `const`이다. `const`는 `constexpr`이 아니다.

- `C++ 14`부터 아래 조건에 해당되지 않으면 `constexpr`를 함수에 사용할 수 있다.

  1. `goto`문 사용
  2. 예외 처리 (`try`문; `C++ 20`부터는 가능함)
  3. 리터럴 타입이 아닌 변수의 정의
  4. 초기화되지 않는 변수의 정의
  5. 실행 중간에 `constexpr`이 아닌 함수를 호출

### `C++`에서 정의하는 리터럴 타입

- `void`형

- 스칼라 타입 (`char, int, bool, long, float, double` 등)

- 레퍼런스 타입

- 리터럴 타입의 배열

- 아래 조건을 만족하는 타입

  1. 디폴트 소멸자를 가지고 있으며

  2. 다음중 하나를 만족

     - 람다 함수

     - `Aggregate`타입  

       (사용자 정의 생성자, 소멸자가 없으며 모든 데이터 멤버들이 `public` ; 예: `pair`)

     - `constexpr` 생성자를 가지며, 복사 및 이동 생성자가 없음

<br>

### 포인터인지 확인하는 함수

`<type_traits>`에 있는 `std::is_pointer<T>::value`의 반환값을 통해 확인 가능

`std::is_pointer_v<T>`를 써도 동일한 효과임

<br>

### Unique_ptr 내부 구조 보면서

- `std::pair`는 `_Compressed_pair`라는걸로 만들어졌나봄 
- `explicit` 키워드 : 파라미터를 넘길 때 자동으로 생성자가 적용되서 형변환이 일어나는 것을 방지
- `noexcept` 키워드
- `nullptr_t`는 `nullptr`의 자료형 이름인 것 같다. `int`, `void*`와도 모두 다르게 취급한다. 크기는 `sizeof(void*)`와 동일하게 4바이트이며, 0 값만 저장 가능하다.

