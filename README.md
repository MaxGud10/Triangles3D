# Описание
Проект реализует пересечение треугольников в пространстве. Важно также отметить, что если треугольник вырожденный, то это всё ещё треугольник (то есть треугольник может быть и прямой, и точкой).


# Ввод/вывод
Со стандартного ввода программа получает число $0 < N \leqslant 10^6$, а потом $N$ наборов из $9$ точек, представляющих вершины трёхмерных треугольников:
```
2

0 0 0
2 0 0
0 2 0

1 0 1
1 0 -1
1 2 0
```

На выходе получаем номера всех треугольников, которые пересекаются хотя бы с одним другим треугольником.

```
0
1
```

## Зависимости

- CMake >=3.11
- Компилятор C++20
- GoogleTest
- Python 3 (для e2e-test)


# Компиляция
```bash
cmake -S ./ -B build/ -DCMAKE_BUILD_TYPE=Release
cmake --build build/
```

Запуск проекта:
```bash
cd build/
./triag < test1.txt
```



Тестирование проекта (интеграция google-тестов и end2end-тестов):
```bash
cd build/
ctest --output-on-failure

# только unit-тесты (GoogleTest)
ctest -L unit --output-on-failure

# только end-to-end (Python e2e-test)
ctest -L e2e --output-on-failure
```

Можно запустить тесты напрямую

**Unit tests**
```bash
./build/google_test

./build/google_test --gtest_list_tests

```

**E2E test**
```bash
python3 end2end/run_e2e_tests.py
```


