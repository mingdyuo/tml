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

다른 어떤 메소드가 절대 변화시키지 않는 객체에 대해서'만' thread-safe하다고 한다. 근데 queue 같은 경우에는 상태가 계속 변하니까 `empty()`에도 락을 걸지 않으면 race condition 유발하겠쥬?

<br>