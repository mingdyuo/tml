## Golang

- 컴파일 언어지만 컴파일 속도가 빨라서 인터프리터처럼 쓸 수 있음
- 정적 타입 검사가 가능
- 간결한 코드 작성과 풍부한 라이브러리가 장점
- 비동기 메커니즘으로 고루틴을 제공 : 병렬로 동작하며 메시지 채널을 통해 값을 주고 받음 / 이벤트 처리와 병렬 프로그래밍이 간단해진다.
- 키워드가 적다. 입문하기 좋음 : break, default, func, interface, select, case, defer, go, map, struct, chan, else, goto, package, switch, const, fall through, if, range, type, continue, for, import, return, var
- 바이트 코드를 생성하는 언어가 아니므로 바이너리만 배포할 경우, 타깃 머신에 맞춰서 각각 컴파일해야 한다. 다중 플랫폼을 지원하려면 소스 코드째로 배포해야 한다.
- 개발 속도, 실행 속도, 병행성 사이의 적정 지점이 필요한 서버 애플리케이션 개발에 많이 사용되는 편이다. 처음 설계 지향점은 시스템 프로그래밍 언어였음. 하지만 GC의 지원이나 제네릭의 부재로 박싱/언박싱이 불필요하게 많이 일어남. 그래서 C/C++을 대체하기는 무리데스. 고성능 연산에는 씨 계열보다는 느리며 저수준 시스템 개발에서는 무거운 런타임으로 사용이 불가능

<br>

### Gvm 설치하기

1. 일단 git을 깔아야 한다.
2. git을 깔기 위해서는 homebrew가 필요하다. (Homebrew, git 설치 : https://lsjsj92.tistory.com/596	)
3. homebrew를 깔기 위해서는 Xcode command line이 필요하다.
4. xcode-select --install 로 설치
5. 만약 4번이 안되면 [https://developer.apple.com/download/all/?q=command%20line](https://developer.apple.com/download/all/?q=command line) 여기서 11.5버전 설치(맥OS가 카탈리나 10번대 버전인 경우)

<br>

### 토막 정리

- Go 프로그램은 패키지로 구성되어 있음. main도 패키지임

- `import` 문을 묶어서 쓰는 것을 "factored"라고 한다. 한줄씩 쓰는 것보다 이걸 쓰는 것을 추천한다. 

- 대문자로 시작하는 이름이 `export` 되는 이름이다.(외부 모듈에서 사용 가능한 것) 패키지를 `import` 할 때 export된 이름만 참조할 수 있다. 

- 함수의 매개변수를 쓸 때 타입이 뒤에 온다.

- 함수 오버로딩, 제너릭 프로그래밍 안됨

- 이런 스타일으로 작성 못함

  중괄호를 리턴값 바로 옆에 놔줘야 한다.

  ```go
  func swap(x, y int) (int, int)
  {
  		// content
  }
  ```

- 반환값에 이름이 정해질 수도 있다. 그런 경우 파라미터에서 지정한 변수의 이름처럼 함수 내부에서 쓸 수 있다. 

  이 때 인자 없는 `return` 을 쓰는데 이를 "naked" return 이라고 한다. 

  naked return문은 긴 함수에서 사용시 가독성이 떨어질 수 있으므로
   주의한다.

- `var` 문을 통해 변수 목록을 선언한다.

  선언하고 사용하지 않으면 컴파일 에러가 뜬다. 

  선언하고 바로 접근하면 기본값으로 초기화된다. (`0`, `false` , "" 등 - `zero value` 라고 한다.) 

- 선언 시에 바로 초깃값을 설정할 수 있다. 초깃값을 써주면 타입을 생략할 수 있다. 초깃값의 타입이 그대로 변수의 타입이 된다.

- 짧은 변수 선언은 `:=` 로 이루어진다. 암시적 type으로 var 처럼 사용될 수 있다. 

  함수 내에서만 짧은 변수 선언을 할 수 있다.

  함수 밖에서는 선언이 모두 키워드를 통해 시작하므로 불가능하다.

- 기본 타입을 살펴 보자

  ```go
  bool
  string
  int, int8, int16, int32, int64
  uint uint8, uint16, uint32, uint64, uintptr
  byte // uint8의 별칭
  rune // int32의 별칭 (유니코드에서 code point 의미)
  float32, float64
  complex64, complex128
  ```

- `var` 을 통한 변수 선언도 `import` 선언처럼 조각낼 수 있다.

- 타입 변환을 할 때에는 명시적인 타입 함수를 사용해야 한다. 대입 자체는 `:=` 를 써서 암시적으로 할 수 있다. 

- 상수는 `const` 를 통해 선언될 수 있으며, `:=` 로 초기화할 수 없다. `=` 로 해야 한다.

- `for` 문을 쓸 때에는 `(,)` 를 쓰지 않으며 `{, }` 는 항상 필수이다.

- for문의 초기화, 사후 구문은 필수가 아니다.

  ```go
  for ;sum < 1000; {
  	sum += sum
  }
  // while처럼 쓰인 것
  for sum < 1000 {
  	sum += sum
  }
  ```

- `if` 문도 `for` 문과 비슷하다. 내부에서 지역변수 선언 가능. 여기서 선언된 변수들은 `else` 에서까지 사용 가능하다.

- `switch` 문은 케이스 하나가 끝나면 자동으로 `break` 가 제공된다. `case` 의 값은 꼭 상수나 정수가 아니어도 된다. 

  조건을 아예 안써도 되는데, 이 경우에는 `case` 부분에 조건을 작성할 수 있다. 길이가 긴 if-else 체인을 작성하는 경우 효과적일 수 있다.

- `defer` 을 어떤 실행문 앞에 써놓으면, 해당 문장이 속한 함수가 종료하기 전까지 실행을 연기한다. 파라미터의 값은 실행문을 지날 때 캡쳐되며, 속한 함수가 종료하면 그제서야 실행한다.

  `defer` 시키는 경우 스택에 호출이 쌓인다. 함수가 종료되면 LIFO 방식으로 수행된다.

- 포인터의 zero value는 `nil`이다. 

  `&`를 통해서 피연산자에 대한 포인터를 생성한다. 

  `*`를 통해서 역참조한다.

  포인터 산술은 지원되지 않는다.

- 구조체 포인터를 사용시 원래는 역참조를 해야 하지만, 역참조를 명시하지 않고 쩜만 써도 바로 필드에 접근을 할 수 있게 되어 있다.

- 슬라이스는 단순히 배열의 영역을 참조하는 것이다. 따라서 슬라이스의 값 수정시 원본 배열의 값도 함께 수정된다.

  슬라이스에는 길이(length, 슬라이스가 포함하는 요소의 개수)와 용량(capacity, 슬라이스의 첫 번째 요소부터 기본 배열의 요소까지의 개수)가 모두 있다.

  슬라이스에 충분한 용량이 있을 때 다시 슬라이싱을 하면 길이를 연장시킬 수 있다.

- 슬라이스 리터럴은 길이가 없는 배열 리터럴과 같다.

- `make` 함수로 슬라이스를 만들면 동적 크기의 배열이 생성된다. 배열의 원소 값은 0으로 이루어진다. 반환은 해당 배열을 참조하는 슬라이스이다. 

  ```go
  func main() {
  	var s []int
  	printSlice(s)
  
  	// append works on nil slices.
  	s = append(s, 0)
  	printSlice(s) // 1 1
  
  	// The slice grows as needed.
  	s = append(s, 1)
  	printSlice(s) // 2 2
  
  	// We can add more than one element at a time.
  	s = append(s, 2, 3, 4)
  	printSlice(s) // 5 6 <- 여기가 왜 5 5가 아닐까
  }
  
  func printSlice(s []int) {
  	fmt.Printf("len=%d cap=%d %v\n", len(s), cap(s), s)
  }
  
  ```

- 맵 만들기 예시

  ```go
  var m map[int]string
  m := make(map[int]string)
  ```

- 함수 리터럴과 고계함수를 이용한 추상화 (132p)

  ```go
  package main
  
  import (
      "fmt"
  )
  type MultiSet map[string]int
  type SetOp func(m MultiSet, val string)
  
  //1
  func Insert(m MultiSet, val string){
      ...
  }
  
  //2
      func InsertFunc(m Multiset) func(val string){
          return func(val string){
              Insert(m, val)
          }
      }
  
  //3
  func BindMap(f SetOp, m MultiSet) func(val string){
      return func(val string){
          f(m, val)
      }
  }
  
  func main(){
      //1
      m := NewMultiSet()
      ReadFrom(r, func(line string){
          Insert(m, line)
      })
      
      //2
      m := NewMultiSet()
      ReadFrom(r, InsertFunc(m))
      
      //3 
      m:= NewMultiSet()
      ReadFrom(r, BindMap(Insert, m))
      
  }
  ```

<br>

<br>

<br>

### 참고

- 패닉은 다른 언어들의 예외(`exception`)과 비슷하지만, 잘 쓰지 않는다. 심각한 에러일 때 패닉을 발생시킨다.

  일반적으로는 에러를 반환하는 방식을 사용한다.
  
- map은 채널처럼 포인터를 쓰지 않고 함수에 그대로 인자를 넘겨서 쓸 수 있다. 컴파일 타임에 런타임 패키지를 이용한 함수로 변경된다.

<br>

### 궁금점 정리

- 슬라이스 리터럴이 배열과 같다면 내부를 수정할 수 있는것인데 왜 이름이 리터럴일까?

  그냥 새로 선언할때 원소를 넣어서 생성해준 것을 array/slice literal이라고 한다.

  ```go
  [5]int{2, 4, 6, 8, 10} // array literal
  []int{2, 4, 6, 8, 10} // slice literal
  ```

  그냥 직접 원소를 지정해서 설정해주므로 리터럴이라고 부르는 것 같다.

- 콘크리트 타입이 무엇인가

  메소드 구현과 데이터의 표현이 완전히 표현되어 있는 것 

  > 이와 반대되는 개념이 인터페이스 타입이다.
  >
  > 메소드 시그니쳐들만 나와 있음. 데이터 자체가 명시되어 있지 않음
  >
  > 메소드에도 구현이 명확히 되어 있는 것이 아니라 추상화되어 있음

- 인터페이스 타입과 인터페이스 value

  인터페이스는 dynamic type과 dynamic value를 가질 수 있다.

  ```go
  type Speaker interface {Speak()}
  
  type Dog struct {name string} // Dog는 concrete 타입임
  
  func (d Dog) Speak(){ // dog type satisfies the speak interface type
  	fmt.Println(d.name) 
  }
  func main(){
  	var s1 Speaker // s1이 interface value
  	var d1 Dog{"Brian"}
  	s1 = d1 // dog가 speak 인터페이스를 satisfy 시키므로 대입가능
    // 여기서 dynamic type은 Dog이며, dynamic value는 d1이다
    s1.Speak() 
  }
  ```

  ```go
  var s1 Speaker
  var d1 *Dog // d1은 concrete value를 가지고 있지 않다. 하지만 Dog라는 concrete type을 가지고 있다. 포인터일뿐 actual data는 아니다.
  s1 = d1 // s1은 dymanic type을 가지고 있지만, dynamic value를 가지고 있지 않다.
  // 이 경우를 nil dynamic value를 갖고 있는 인터페이스라고 볼 수 있다.
  // 이 경우에도 여전히 s1.Speak()과 같이 메소드를 호출할 수 있다.
  ```

  [dynamic type & dynamic value](https://www.coursera.org/lecture/golang-functions-methods/m4-1-3-interface-vs-concrete-types-7oDet)

- 소수점 맞춤과 같은 출력 형식을 지정하는 경우를 제외하고 일반적인 값을 출력할 때 `%v` 만 써도 상관 없는지? 다른 정해진 지정자를 쓰면 뭔가 이점이 있는지 

  1. int를 출력할 때 %d를 쓰면 base10의 숫자로 출력한다.

  2. %v를 쓰면 default format으로 출력하게 해준다. 이 default format은 따로 override하여 다르게 출력할 수도 있다.

     fmt.Stringer 인터페이스를 확인하고 string 메소드가 구현되어 있으면 그걸 이용해서 출력한다.

  > %d, %v 이런것은 formatting verb라고 부른다.

- `package` 가 정확히 어떤 방식으로 쓰이는지 헷갈리니 알아보자

- 유니코드, UTF-8 인코딩 방식과 go에 대해서 좀더 알아보자.

  유니코드는 국제 표준 문자표이고, UTF-8은 인코딩 방식이다. 유니코드는 글자와 코드를 1:1로 매핑한 것이고, UTF-8은 그 유니코드를 인코딩 하는 방식으로 글자의 종류마다 바이트 길이가 다르다.

- `fmt.Errorf()` 의 반환이 문자열인지 아니면 `err` 형태인지?

- `New` 치면 다 새로 만들어 주는건가?

  자료형 이름이 소문자인 경우에는 패키지 밖에서 생성할 수 없음. 이 경우 `NewX()` 형태의 생성자를 만들어놓아서 다른 패키지에서 호출할 수 있게 하는 경우 존재. 해당 자료형의 오브젝트를 바로 반환하거나, 자료형이 구현하는 인터페이스를 돌려준다.

- go 의 GC 방식이나 원리가 어떻게 되는지 알아보자

- `iota`는 뭐의 줄임말이지

- 구조체 A 안에 구조체 B, C가 각각 내장되어 있다. 그리고 구조체 B, C에 D라는 동일한 이름을 갖는 필드가 각각 있다면, `A.D`로 내장된 구조체 접근을 할 시 어떤 일이 일어나는지?


