# Уровень 1

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

Формат e2e test и запуск своих тестов.

В репозитории есть простая программма написанная на Python.

Скрипт читает [файл](end2end/e2e_tests.txt) и по очереди гоняет программу на каждом кейсе, сравнивая построчно stdout c ожидаемым выводом.
Формат файла — блоки вида:

<details>
<summary>Посмотреть формат файла</summary>

```
TEST_NUMBER: <int>
INPUT:
<многострочный ввод, как для stdin программы>
OUTPUT:
<ожидаемый многострочный вывод или <empty>>
---
```

Пример:
```
TEST_NUMBER: 1
INPUT:
2

1 0 1
1 0 5
5 0 4

5 0 2
2.1 0 8
1 0 -1

OUTPUT:
0
1

---

TEST_NUMBER: 2
INPUT:
1

0 0 0
1 0 0
0 1 0

OUTPUT:
<empty>
---
```

### Как добавить свои тесты?
1) Откройте [end2end/e2e_tests.txt](end2end/e2e_tests.txt)
2) Вставьте новый блок по шаблону выше: уникальный `TEST_NUMBER`, секции `INPUT`: и `OUTPUT`: и разделитель `---`.
- Если ожидаемого вывода нет — поставьте в секции `OUTPUT`: строку `<empty>`.


</details>


# Уровень 2
![OpenGL Badge](https://img.shields.io/badge/OpenGL-5586A4?logo=opengl&logoColor=fff&style=for-the-badge)![C++ Badge](https://img.shields.io/badge/C%2B%2B-00599C?logo=cplusplus&logoColor=fff&style=for-the-badge)![CMake Badge](https://img.shields.io/badge/CMake-064F8C?logo=cmake&logoColor=fff&style=for-the-badge)

>В данном уровне реализуется визуализация треугольников с помощью **OpenGL** 3.3.

###### Структура проекта
```
├── CMakeLists.txt
├── include
│   ├── camera.hpp
│   ├── EBO.hpp
│   ├── line.hpp
│   ├── plane.hpp
│   ├── shader.hpp
│   ├── VAO.hpp
│   ├── VBO.hpp
│   └── vector.hpp
├── shaders
│   ├── fragment.frag
│   └── vertex.vert
└── src
    ├── camera.cpp
    ├── EBO.cpp
    ├── google_test.cpp
    ├── main.cpp
    ├── shader.cpp
    ├── VAO.cpp
    └── VBO.cpp

```

`shader.hpp` - класс на C++ загрузки и компиляции шейдеров в OpenGL. Примеры шейдеров можно найти [здесь](./OpenGL/shaders/).

Шейдеры делятся на два типа - вершинные (vertex) и фрагментные (fragment).

| Vertex Shader | Fragment Shader |
|---------------|-----------------|
| Принимает исходные 3D-координаты вершины -> преобразует их в 2D-координаты на экране -> передает информацию _fragment shader_ |Определяет итоговый цвет пикселя, используется для  наложения текстур |


`VAO.hpp` - класс для работы с **Vertex Array Object** (объект массива вершин). Это массив внутри которого указатели на буферы **координат вершин**, **цветов**, **текстурныъ координат**.

`VBO.hpp` - класс для работы с **Vertex Buffer Objects**. Информация для VAO как раз хранится в буферах - VBO.

`EBO.hpp` - класс для работы с **Element Buffer Object**. Используется для _оптимизации_ отрисовки геометрии (например, когда одни и те же вершины используется в одних и тех же треугольниках.)
![](./img/ebo%20explanation.jpeg)

`Camera.hpp` - класс для работы с камерой в OpenGL. Задается тремя параметрами - `camPos` (координаты камеры), `camDir` (направление взгляда камеры), `upDir` (вектор, который мы задаем для определения _верха_)

Камерой можно управлять с помощью горячих клавиш.
| Input | Desc |
|-------|------|
| `W`   |  Вперед    |
| `A`   |  Влево    |
| `S`   |  Назад    |
| `D`   |  Вправо    |
| `SPACE` |Вверх    |
| `LEFT_CTRL` | Вниз |
| `LEFT_SHIFT` |Ускорение |


