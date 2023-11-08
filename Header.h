#pragma once

/*\brief Тестовая задача, истинная траектория
* \param x Независимая переменная
* \param v Численное решение функции
* \return Истинное значение функции в точке x
*/
double testTrueTr(double x, double v);

/*\brief Тестовая задача, численная траектория
* \param x Независимая переменная
* \param v Численное решение функции
* \return Значение v' в точке x
*/
double f_test(double x, double v);

/*\brief Первая задача, численная траектория
* \param x Независимая переменная
* \param v Функция от x
* \return Значение v' в точке x
*/
double f1(double x, double v);
/*\brief Вторая задача, численная траектория
* \param x Независимая переменная
* \param v Функция от x
* \return Значение v' в точке x
*/
double f2(double x, double v);
double f22(double x, double v);
double fu(double, double, double, double);
double f_u(double, double, double, double);

/*\brief Функция Рунге-Кутты 4-ого порядка
* \param x Независимая переменная
* \param v Функция от x
* \param h Шаг метода по x
* \param f Указатель на функцию v'(x)
* \return Значение функции v в точке x + h
*/
double RK4(double x, double v, double h, double(*f)(double, double));

/*\brief Процедура просчета траекторий и записи в файл тестовой задачи а
* \param out Поток выходного файла
* \param Nmax Максимум точек траектории
* \param b Правая граница по x
* \param x Начальное значение x
* \param v Начальное значение u
* \param h Начальный шаг метода
* \param f Указатель на функцию u'(x)
*/
void Task1a(std::ofstream& out, const size_t Nmax, const double b, double x, double v, double h, double(*f)(double, double));

/*\brief Процедура просчета траекторий и записи в файл тестовой задачи б
* \param out Поток выходного файла
* \param Nmax Максимум точек траектории
* \param b Правая граница по x
* \param x Начальное значение x
* \param v Начальное значение u
* \param h Начальный шаг метода
* \param eps Параметр контроля локальной погрешности
* \param f Указатель на функцию u'(x)
*/
void Task1b(std::ofstream& out, const size_t Nmax, const double b, double x, double v, double h, const double eps, double(*f)(double, double));

/*\brief Процедура просчета траекторий и записи в файл первой задачи а
* \param out Поток выходного файла
* \param Nmax Максимум точек траектории
* \param b Правая граница по x
* \param x Начальное значение x
* \param v Начальное значение u
* \param h Начальный шаг метода
* \param f Указатель на функцию u'(x)
*/
void Task2a(std::ofstream& out, const size_t Nmax, const double b, double x, double v, double h, double(*f)(double, double));

/*\brief Процедура просчета траекторий и записи в файл первой задачи б
* \param out Поток выходного файла
* \param Nmax Максимум точек траектории
* \param b Правая граница по x
* \param x Начальное значение x
* \param v Начальное значение u
* \param h Начальный шаг метода
* \param eps Параметр контроля локальной погрешности
* \param f Указатель на функцию u'(x)
*/
void Task2b(std::ofstream& out, const size_t Nmax, const double b, double x, double v, double h, const double eps, double(*f)(double, double));
