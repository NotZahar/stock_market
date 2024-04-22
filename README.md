### Биржа с клиент-серверной архитектурой (server side)

##### Dependencies:
- Boost v1.82
- SQLiteCpp v3.3 [ref](https://github.com/SRombauts/SQLiteCpp)
- (json v3.11 [ref](https://github.com/nlohmann/json) - header only third-party lib)
- CMake v3.28
- Doxygen v1.9

##### Build:
```
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

##### Docs:
- https://notzahar.github.io/stock_market/
