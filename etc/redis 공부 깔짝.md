## Key words

- 샤딩
  - index 방식
    - 많은 데이터가 있을 때 이를 분리해서 저장하는 방식입니다. Index 방식은 복잡하지만, 각 데이터 마다 어떤 서버에 할당되어야 할지 지정해주기 때문에, 데이터의 이동을 최소화할 수 있는 방식입니다.
  - range 방식
    - 가장 쉽게 적용할 수 있는 Sharding 방식입니다. Key 의 Range에 따라서 서버를 할당합니다.
  - modular 방식
    - Range에 비해서 데이터를 균등하게 Sharding 할 수 있는 방식입니다. 다만 서버의 확장성이 뒤로 가면 제한되게 됩니다.
- Redis caching
  - Look-aside 방식
    - 일반적으로 Cache는 Cache에 데이터가 없으면 해당 데이터를 DB에서 읽어서 Cache를 만들게 됩니다. 이를 우리는 Look-Aside Cache 방식이라고 합니다.
  - Write back 방식
    - 일단 DB에 Write를 해야하면 DB에 부하가 발생하기 때문에 이를 위해서 먼저 Cache 에 써 놓고, 차후에 실제 데이터를 스토리지에 저장하는 것이 Write-Back 방식입니다. 상황에 따라서 데이터 유실의 위험이 있습니다.
- Hot key 이슈
  - Hot key는 많은 데이터들이 Cache 서버나 DB서버의 성능의 한계 이상으로 특정 Key에 몰릴 경우 문제가 발생합니다. Hot Key 해결을 위한 방법에는 Local Cache, Read Replica 를 이용하는 방법, Multi Write Read One 등이 방식이 있습니다.
- Consistent Hashing 이론