### 스레드 프로그래밍: 21.03.09 - 10

[C++ 스레드](https://modoocode.com/269)

[C++ 뮤텍스와 조건변수](https://modoocode.com/270)

[C++ memory order와 atomic 객체](https://modoocode.com/271)

[C++ future, promise, packaged_task, async](https://modoocode.com/284)

[C++ 스레드 풀 만들기](https://modoocode.com/269)

<br>

### 멀티 스레드 프로그램

제대로 만들지 않으면 디버깅이 겁나 어렵다

왜냐면 race condition이 발생하기 때문

이를 해결하기 위해서 뮤텍스를 씀

<br>

### 뮤텍스 사용하기

1. mutex는 `lock()`하면 꼭 `unlock()`해줘야 한다. 안그러면 데드락 걸려서 프로그램 강제종료 시켜야함

2. 이를 대신 해결할 수 있는 방법이, 소멸자에서 처리하는 패턴을 이용하는 것이다.

   `std::lock_guard<std::mutex> lock(m);`

   여기서 `lock_guard`라는 객체는 뮤텍스를 인자로 받아서 생성된다. 생성하면서 자동으로 lock 된다. 그리고 scope를 빠져나가면서 lock_guard는 소멸되고, 알아서 해당 뮤텍스를 unlock한다. 사용자가 unlock을 따로 신경쓰지 않아도 되어 편리하다.

3. 하지만 `lock_guard`를 쓴다고 데드락이 항상 방지되는 것은 아니다. 서로 다른 객체에서 다른 순서로 lock을 획득하면 데드락이 발생할 수 있다.

4. `try_lock()`을 사용해서 해당 뮤텍스에 락이 걸려있는지 체크하고 가지고 있던 lock을 풀어서 데드락을 방지할 수 있다.

<br>

### 데드락 방지하기

1. 중첩된 Lock을 사용하는 것을 피해라

   대부분의 디자인에서는 1개의 lock으로도 충분함

2. Lock을 소유하고 있을 때 유저 코드를 호출하는 것을 피해라.

3. Lock들을 언제나 정해진 순서로 획득해라

<br>

### 생산자-소비자 패턴

1. 생산자와 소비자가 각각 공유 자원에 접근할 때에는 뮤텍스를 걸어야 함

2. 소비자는 계속 할일이 있는지 확인하는 작업함

   하지만 이는 비효율 적이므로 특정 시간 주기로 하도록 할 수 있음

   하지만 이것도 비효율적임

   그래서 할일이 있을때 깨우도록 조건을 걸어놓을 수 있음 → 조건 변수의 사용 (`condition_variable`)

   (참고) C++20에서는 semaphore가 추가되었음

<br>

### condition_variable

1. wait함수

   어떤 조건이 참이 될 때까지 기다리도록 함

   해당 조건 만족 시 깨우기

   해당 조건 만족 못할 시, 깨우지 않음. 뮤텍스를 넣어줘서 lock이 되어 있는걸 풀어줌

2. `notify_one()`

   자고있는 스레드 중 하나를 깨우기

3. `notify_all()`

   할일을 다 마친 후 자고있는 스레드를 모두 깨우기

   `join`을 모두 하기 위해서는 스레드를 모두 깨워야 함

<br>

### atomic

- C++에서 원자적인 연산을 쉽게 할수 있도록 지원하는 도구

- 뮤텍스로 감싸지 않고 스레드 신경안쓰면서 연산 가능

- 속도가 더 빠름

- 어셈블리 상에서 해당 명령어는 한줄이고, 메모리 읽기 & 연산 & 메모리 쓰기를 한번에 수행

  이는 컴파일러가 CPU 아키텍쳐를 미리 알고 있기 때문에 가능함. cpu에 따라 이 명령이 없는 경우도 있음. `var.is_lock_free()`를 실행해보면 ~~원자적 수행이 가능한지 확인할 수 있음.~~ 여기서의 lock은 뮤텍스의 lock 행위를 의미하는 것이 아님

  ```c++
  std::atomic<int> counter(0);
  
  // counter라는 int형 atomic 변수에 0을 할당
  ```

<br>

### memory_order

- `atomic`객체들이 원자적 연산을 할 때 어떤 방식으로 접근하는지 지정할 수 있음.

1. `memory_order_relaxed`

   - 가장 느슨한 조건임
   - 주위의 다른 메모리 접근과 순서가 바뀌어도 무방. 연산 순서에 자유를 허하노라
   - 매우 빠른 속도로 실행 가능
   - 생산자-소비자 관계에서는 사용할 수 없음

2. `memory_order_acquire`, `memory_order_release`

   - relaxed보다 조금 엄격한 애들. 동기화 해줌

   - release : 해당 명령 이전의 모든 메모리 명령들이 해당 명령 이후로 재배치 되는 것을 금지

     같은 변수를 acquire로 읽는 쓰레드 있으면 release 이전에 오는 모든 명령이 해당 쓰레드가 관찰할 수 있어야 함

   - acquire : 해당 명령 뒤에 오는 모든 메모리 명령이 이녀석 앞으로 재배치 되는 것을 금지

3. `memory_order_acq_rel`

   - acquire와 release를 모두 수행
   - 읽기와 쓰기를 모두 수행하는 함수에서 사용할 수 있음

4. `memory_order_seq_cst`

   - 메모리 명령의 순차적 일관성을 보장

   - 재배치 하지 않고 우리가 쓴 코드대로 cpu가 동작하도록 해줌

   - 비싼 연산임

     근데  amd x86(-64) 아키텍처는 거의 순차적 일관성 보장되기 때문에 이 조건을 강제해도 차이가 크지 않음

     arm 계열은 동기화 비용이 크기 때문에 꼭 필요할 때만 사용해야 함

   - `atomic`객체 사용 시 memory_order를 지정하지 않으면 디폴트로 얘가 지정됨

5. `memory_order_consume`

   얘는 C++17에서 수정중이므로 사용을 권장하지 않음 지금은 모르겠네용?

<br>

### 비동기 수행하기

- 원래 C++에서는 thread를 명시적으로 선언해서 비동기 작업을 했어야 했다

- C++11에서는 비동기 도구를 제공한다.

  → Promise & Future

  → 생산자 & 소비자의 역할

- `promise`객체를 생성하고, `get_future()`함수를 호출해 `future` 객체에 넣는다.

  `promise` 객체에서 작업이 완료되면 설정된 객체가 이동하고 `future`객체에서 가져온다.

- `future`객체에서 `get()`을 호출하면 `promise` 객체에서 설정한 객체를 이동시킨다.

  따라서 한번만 호출해야 한다. 

- 조건 변수를 통해서도 promise & future 패턴을 만들 수 있지만, promise & future 패턴을 직접 사용하는 것이 더 깔끔하고 이해하기 쉽다.

  또한 future에는 예외도 전달할 수 있다.

  ```c++
  void worker(std::promise<string>* p){
  	try{
          throw std::runtime_error("Some Error!");
      }catch(...){
          p->set_exception(std::current_exception());
      }
  }
  ```

  `set_exception`에는 예외 객체가 아닌 `exception_ptr`을 전달해야 한다.

- `future_status`는 3가지 상태를 가질 수 있음

  1. `future_status::ready`

     future에 값이 설정됨

  2. `future_status::timeout`

     `wait_for`함수에 설정한 시간이 지났지만, future에 값이 설정되지 않아서 리턴

  3. `future_status::deferred`

     결과값을 계산하는 함수가 채 실행되지 않음

<br>

### shared_future

- `future`는 `get()`을 한번만 할 수 있음

  하지만 여러 개의 다른 쓰레드에서 `get()`을 해야 하는 경우가 있음

  이 때 사용한다.

<br>

### packaged_task

- promise-future 패턴을 비동기적 함수(Callable, 람다 함수)의 리턴 값에 간단히 적용할 수 있도록 함

- 비동기로 수행할 함수 자체를 인자로 받아서 생성자 호출함

  템플릿 인자로 해당 함수의 타입 명시해야 함

- `packaged_task`에 전달된 함수는 리턴 시

  리턴값을 `promise`에 `set_value`

  예외 발생시 `promise`에 `set_exception` 한다.

- 스레드에 `promise`를 전달하지 않아도 알아서 리턴값을 처리해 주므로 편리하다.

<br>

### std::async

- `promise`, `packaged_tsdk`는 스레드를 명시적으로 생성해서 실행해야 비동기적 실행이 가능

- `std::async`는 함수 전달 시 스레드 알아서 만들어서 해당 함수를 비동기적으로 실행하고, 결과값을 `future` 형태로 리턴하여 전달함

- ```c++
  std::async((a) , 실행함수, cref(v), 0, v.size() / 2);
  ```

  (a) 는 어떤 형태로 실행할지를 설정하는 곳이다.

  1. `std::launch::async`
     바로 스레드를 생성해서 함수 실행
  2. `std::launch::deferred`
     `future`의 `get`이 호출되었을 때 (동기적으로) 실행. 새로운 스레드를 생성하지 않음
     바로 당장 비동기적으로 실행할 필요가 없으면 이 옵션을 준다.

- 람다로 전달도 가능

  ```c++
  auto f = std::async([](){ do_work(something); });
  ```



<br>

### 걍 몰랐던 것

1. `std::cerr`으로 출력할 수 있음

   `std::cout`과 다르게, 버퍼를 사용하지 않음. 따라서 터미널에 바로 출력함

2. `const`의 레퍼런스는 `cref()`를 호출해서 넘겨주면 된다.

3. 람다 함수 안에서 멤버 함수에 접근하기 위해서는 `this`를 전달해 주어야 한다.

   ```c++
   foo.bar([this](){this->foobar();});
   ```

4. `this`는 레퍼런스로 전달할 수 없음

5. `emplace_back`과 `push_back`

   - `push_back`은 객체를 집어 넣는 형식, 객체 없이 삽입하려면 임시객체가 필요

   - `emplace_back`은 C++11에서 도입, 가변 인자 템플릿 사용해서 객체 생성시 필요한 인자만 받음. 함수 내에서 객체를 생성해서 삽입함

     임시 객체를 만들 필요가 없으므로 함수 내부에서 삽입에 필요한 생성자를 한번만 호출하면 됨

   - 보통은 `emplace_back`이 더 빠름. 컴파일러가 최적화하면 비슷하거나 `push_back`이 더 빠를 수도 있음

   - `push_back`은 암시적 생성자만 호출하고 `emplace_back`은 모든 유형의 생성자를 호출함

   - 근데 좀 헷갈림 `unique_ptr` 개념을 확실하게 잘 몰라서 나중에 [여기](https://openmynotepad.tistory.com/10)를 다시 읽어 보자 

6. 스레드에 변경시킬 변수를 전달할 때에는 포인터로 전달하거나 레퍼런스를 사용한다면 `ref()`로 감싸서 넘겨줘야 한다.

7. `broken promise` 예외는 `promise`에 `set_value`를 하기 전에 이미 `promise`의 `future` 객체가 파괴되면 발생하는 예외임
