



## [네이버클라우드 개발자 스토리] 좋은 코드란 무엇일까?🤔 #클린코드 이야기

- [포스팅](https://medium.com/naver-cloud-platform/%EB%84%A4%EC%9D%B4%EB%B2%84%ED%81%B4%EB%9D%BC%EC%9A%B0%EB%93%9C-%EA%B0%9C%EB%B0%9C%EC%9E%90-%EC%8A%A4%ED%86%A0%EB%A6%AC-%EC%A2%8B%EC%9D%80-%EC%BD%94%EB%93%9C%EB%9E%80-%EB%AC%B4%EC%97%87%EC%9D%BC%EA%B9%8C-%ED%81%B4%EB%A6%B0%EC%BD%94%EB%93%9C-%EC%9D%B4%EC%95%BC%EA%B8%B0-c7811f73a46b)

<img width="731" alt="image" src="https://user-images.githubusercontent.com/41130448/152800981-101f85c6-e93f-4264-8d53-22d14fd66f67.png">



﻿<img width="792" alt="image" src="https://user-images.githubusercontent.com/41130448/152801067-b3cd0f53-7ca1-4c3f-9108-601e5162cabd.png">



<img width="736" alt="image" src="https://user-images.githubusercontent.com/41130448/152801173-65c6a032-d18e-425a-869f-b14147cc07b0.png">



신기하네 근데 그동안 리시버에 대해서 포인터로 받거나 아니거나에 대해 깊이 생각해본적이 없었다 걍 포인터로 대충 받는게 좋지않을까 정도

(자바의 메소드 리시버는 다 포인터 형태라고한다)

When defining a method on a type, the receiver (s in the above examples) behaves exactly as if it were an argument to the method. Whether to define the receiver as a value or as a pointer is the same question, then, as whether a function argument should be a value or a pointer. 

1. 리시버를 변경해야 할 필요가 있으면 포인터 리시버 사용 
2. 죨래 큰 구조체 같은거 넘길 경우 포인터가 효율적
3. 일관성 맞추기. 메소드 하나를 포인터로 받았으면 다른애들도 맞춰써라﻿