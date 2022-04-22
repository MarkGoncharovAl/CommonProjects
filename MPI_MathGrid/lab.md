# Марк Гончаров
### Б01-908
### 3.04.2022

# Численное решение уравнения переноса

###  Описание
Ознакомиться с [полной версией описания](https://github.com/MarkGoncharovAl/CommonProjects/tree/master/MPI_MathGrid).

Данный проект помогает сделать сеточную аппроксимацию решения 
дифференциального уравнения

![Условие задачи](1.png)

### Сборка проекта
> - *mkdir build*
> - *cd build*
> - *cmake ../*
> - *cmake --build .*
> - *mpirun -np* **threads** *./run*

### Задание граничных условий

```cpp
// main.cpp:20
auto phi = [](double x) -> double { return x; };
auto psi = [](double t) -> double { return t; };
auto f = [](double x , double t) -> double { return x + t; };
```

### Заполнение сетки

```cpp
double tauh = (scale.h + scale.tau) / (2 * scale.h * scale.tau);
for (int x = 0; x < sizeX - 1; ++x)
    data[curT + 1][x + 1] = (
        f (scale.tau * (0.5 + curT) , scale.h * (0.5 + x + offset)) +
        (data[curT][x] + data[curT][x + 1] - data[curT + 1][x]) / 2.0 / scale.tau +
        (data[curT][x] - data[curT][x + 1] + data[curT + 1][x]) / 2.0 / scale.h
        ) / tauh;
```