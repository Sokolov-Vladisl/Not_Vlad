#include <iostream>
#include <locale>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include "Header.h"
using namespace std;



// надо замениь std::numeric_limits<double>::epsilon()  на 0.0   Но я думаю, что всё испорчу :(




void Task1a(ofstream& out,const size_t Nmax, const double b, double x, int Number_of_x/*НОВОЕ*/, double v, double h, double(*f)(double, double))// НОВОЕ Number_of_x 
{
    //------------Новое-------------
    cout << endl;
    cout << "(b-a)/N = h";   //Показываем как мы находим шаг.  Он зависит от кол-ва шагов
    cout << endl;

    h = b / Number_of_x;

    //------------------------------

    out <<"i" << setw(20) << "xi" << setw(20) << "vi" << setw(20) << "hi" <<setw(20) << "ui" << setw(20) << "|ui - vi|" << endl;
    
    size_t N = 0;                       //Номер шага
    double u = v;                       //Начальная точка истинной траектории
    const double C = u/testTrueTr(x,u);//Постоянная интегрирования для истинной траектории
    double E = 0;                       //Расстояние численной траектории до истинной траекторией
    double E_max = 0;                   //Максимум расстояний
    double x_E_max = 0;                 //Точка, в которой достигается максимум расстояний
    
    for (; v < std::numeric_limits<double>::max() && N < Number_of_x ; ++N)// Так мы точно доходим до правой границы  (Не работает с задачей Б)
    {
        v += RK4(x, v, h, f);
        x += h;
        u = testTrueTr(x,v) * C;
        E = u - v < 0.0 ? v - u : u - v;
        if (E_max - E < std::numeric_limits<double>::epsilon())
        {
            E_max = E;
            x_E_max = x;
        }
        out << N+1 << setw(20) << x << setw(20) << v << setw(20)<< h << setw(20) << u << setw(20) << E << endl;
    }

    out << "n = " << N << std::endl
        << "b - xn = " << b - x << std::endl
        << "max|ui - vi| = " << E_max << " on " << x_E_max << std::endl;
};

void Task1b(ofstream& out, const size_t Nmax, const double b, double x, double v, double h,const double eps, double(*f)(double, double))
{
    out << "i" << setw(20) << "xi" << setw(20) << "vi" << setw(20) << "v2i"<<setw(20)
        << "ei" << setw(20) << "S" << setw(20) << "hi" << setw(20) << "C1" << setw(20)
        << "C2" << setw(20) << "ui" << setw(20) << "|ui - vi|" << endl;
    
    size_t N = 0;                       //Номер шага
    double v0 = v;                      //Численная траектория до шага
    double v1;                          //Численная траектория после половины шага
    double v2;                          //Численная траектория после двух половинных шагов
    double e;                           //Расстояние между численными траекториями, полученными с разными шагами
    double S;                           //Оцнка локальной погрешности двух половинных шагов
    double S_max = 0;                   //Максимум ОЛП
    size_t C1 = 0;                      //Счётчик удвоений шага
    size_t C2 = 0;                      //Счётчик деления шага
    double u = v;                       //Начальная точка истинной траектории
    const double C = u/testTrueTr(x, u);//Постоянная интегрирования для истинной траектории
    double E = 0;                       //Расстояние численной траектории до истинной траекторией
    double E_max = 0;                   //Максимум расстояний
    double x_E_max = 0;                 //Точка, в которой достигается максимум расстояний
    double h_max = h;                   //Максимум шага траектории
    double x_h_max = x;                 //Точка, в которой достигается максимум шага траектории
    double h_min = h;                   //Минимум шага траектории
    double x_h_min = x;                 //Точка, в которой достигается минимум шага траектории
    bool was_branch = false;            //Нужно ли пересчитывать численную траекторию с повышенной точностью
    while ( N < Nmax && b - x - h >= std::numeric_limits<double>::epsilon() && v1 < std::numeric_limits<double>::max() && v2 < std::numeric_limits<double>::max())  // Если v1 или v2 перейдут в inf то цикл завершится (нижно ли делать для min???)
    {
        if (was_branch)
            v = v1;
        else
            v = v0 + RK4(x, v0, h, f);

        v1 = v0 + RK4(x, v0, h * 0.5, f);
        v2 = v1 + RK4(x, v1, h * 0.5, f);

        e = v2 - v < std::numeric_limits<double>::epsilon() ? v - v2 : u - v2;
        S = e / 15.;
        if (S_max - S < std::numeric_limits<double>::epsilon())
            S_max = S;

        if (eps - S < std::numeric_limits<double>::epsilon())
        {
            ++C2;
            h *= 0.5;
            if (h - h_min < std::numeric_limits<double>::epsilon())
            {
                h_min = h;
                x_h_min = x;
            }
            was_branch = true;
            continue;
        }

        x += h;
        u = testTrueTr(x, v) * C;

        if (32. * S - eps < std::numeric_limits<double>::epsilon())
        {
            ++C1;
            h *= 2;
            if (h_max - h < std::numeric_limits<double>::epsilon())
            {
                h_max = h;
                x_h_max = x;
            }
        }
        
        E = u - v < std::numeric_limits<double>::epsilon() ? v - u : u - v;
        if (E_max - E < std::numeric_limits<double>::epsilon())
        {
            E_max = E;
            x_E_max = x;
        }

        out << N + 1 << setw(20) << x << setw(20) << v << setw(20) <<v2 << setw(20) << e << setw(20) << S << setw(20)
            << h << setw(20) << C1 << setw(20) << C2<< setw(20) << u << setw(20) << E << endl;
        
        was_branch = false;
        v0 = v;
        ++N;
    }

    out << "n = " << N << std::endl
        << "b - xn = " << b - x << std::endl
        << "max loc err = " << S_max << std::endl
        << "max hi = " << h_max << " on " << x_h_max << std::endl
        << "min hi = " << h_min << " on " << x_h_min << std::endl
        << "max|ui - vi| = " << E_max << " on " << x_E_max << std::endl;
};

void Task2a(ofstream& out, const size_t Nmax, const double b, double x,int Number_of_x/*НОВОЕ*/, double v, double h, double(*f)(double, double))  // НОВОЕ Number_of_x 
{
    //------------Новое-------------
    cout << endl;
    cout << "(b-a)/N = h";   //Показываем как мы находим шаг.  Он зависит от кол-ва шагов
    cout << endl;

    h = b / Number_of_x;

    //------------------------------


    out << "i" << setw(20) << "xi" << setw(20) << "vi" << setw(20) << "hi" << endl;

    size_t N = 0;                       //Номер шага
    
    for (; v < std::numeric_limits<double>::max() && N < Number_of_x; ++N)    // Так мы точно доходим до правой границы  (Не работает с задачей Б)
    {
        v += RK4(x, v, h, f);
        x += h;
        out << N + 1 << setw(20) << x << setw(20) << v << setw(20) << h << endl;
    }

    out << "n = " << N << std::endl
        << "b - xn = " << b - x << std::endl;
};

void Task2b(ofstream& out, const size_t Nmax, const double b, double x, double v, double h, const double eps, double(*f)(double, double))
{
    out << "i" << setw(20) << "xi" << setw(20) << "vi" << setw(20) << "v2i" << setw(20)
        << "ei" << setw(20) << "S" << setw(20) << "hi" << setw(20) << "C1" << setw(20) << "C2" << endl;

    size_t N = 0;                       //Номер шага
    double v0 = v;                      //Численная траектория до шага
    double v1;                          //Численная траектория после половины шага
    double v2;                          //Численная траектория после двух половинных шагов
    double e;                           //Расстояние между численными траекториями, полученными с разными шагами
    double S;                           //Оценка локальной погрешности двух половинных шагов
    double S_max = 0;                   //Максимум ОЛП
    size_t C1 = 0;                      //Счётчик удвоений шага
    size_t C2 = 0;                      //Счётчик деления шага
    double h_max = h;                   //Максимум шага траектории
    double x_h_max = 0;                 //Точка, в которой достигается максимум шага траектории
    double h_min = h;                   //Минимум шага траектории
    double x_h_min = 0;                 //Точка, в которой достигается минимум шага траектории
    bool was_branch = false;            //Нужно ли пересчитывать численную траекторию с повышенной точностью
    while (N < Nmax && b - x - h >= std::numeric_limits<double>::epsilon() && v1 < std::numeric_limits<double>::max() && v2 < std::numeric_limits<double>::max())  // Если v1 или v2 перейдут в inf то цикл завершится (нижно ли делать для min???)
    {
        if (was_branch)
            v = v1;
        else
            v = v0 + RK4(x, v0, h, f);

        v1 = v0 + RK4(x, v0, h * 0.5, f);
        v2 = v1 + RK4(x, v1, h * 0.5, f);

        e = v2 - v < std::numeric_limits<double>::epsilon() ? v - v2 : v2 - v;
        S = e / 15.;
        if (S_max - S < std::numeric_limits<double>::epsilon())
            S_max = S;

        if (eps - S < std::numeric_limits<double>::epsilon())
        {
            ++C2;
            h *= 0.5;
            if (h - h_min < std::numeric_limits<double>::epsilon())
            {
                h_min = h;
                x_h_min = x;
            }
            was_branch = true;
            continue;
        }

        x += h;

        if (32. * S - eps < std::numeric_limits<double>::epsilon())
        {
            ++C1;
            h *= 2;
            if (h_max - h < std::numeric_limits<double>::epsilon())
            {
                h_max = h;
                x_h_max = x;
            }
        }

        out << N + 1 << setw(20) << x << setw(20) << v << setw(20) << v2 << setw(20) << e << setw(20) << S << setw(20)
            << h << setw(20) << C1 << setw(20) << C2 << endl;

        was_branch = false;
        v0 = v;
        ++N;
    }

    out << "n = " << N << std::endl
        << "b - xn = " << b - x << std::endl
        << "max loc err = " << S_max << std::endl
        << "max hi = " << h_max << " on " << x_h_max << std::endl
        << "min hi = " << h_min << " on " << x_h_min << std::endl;
};

void Task3a(ofstream& out,const size_t Nmax, const double b, double x, int Number_of_x /*НОВОЕ*/, double v, double z, double h, const double a, double(*f1)(double, double), double(*f2)(double, double))   // НОВОЕ Number_of_x 
{
    //------------Новое-------------
    cout << endl;
    cout << "(b-a)/N = h";   //Показываем как мы находим шаг.  Он зависит от кол-ва шагов
    cout << endl;
    
    h = b / Number_of_x;

    //------------------------------



    out <<"i" << setw(20) << "xi" << setw(20) << "vi" << setw(20) << "hi" << endl;

    size_t N = 0;                       //Номер шага

    for (; v < std::numeric_limits<double>::max() && N < Number_of_x && v<; ++N)  // Так мы точно доходим до правой границы  (Не работает с задачей Б)
    {
        z += a * RK4(v, z, h, f1);
        v += RK4(x, z, h, f2);
        x += h;
        out << N+1 << setw(20) << x << setw(20) << v << setw(20)<< h << endl;
    }

    out << "n = " << N << std::endl
        << "b - xn = " << b - x << std::endl;
};

void Task3b(ofstream& out, const size_t Nmax, const double b, double x, double v,double z, double h,const double a, const double eps, double(*f1)(double, double), double(*f2)(double, double))
{
    out << "i" << setw(20) << "xi" << setw(20) << "vi" << setw(20) << "v2i" << setw(20)
        << "ei" << setw(20) << "S" << setw(20) << "hi" << setw(20) << "C1" << setw(20) << "C2" << endl;

    size_t N = 0;                       //Номер шага
    double v0 = v;                      //Численная траектория до шага
    double v1;                          //Численная траектория после половины шага
    double v2;                          //Численная траектория после двух половинных шагов
    double z0 = z;
    double z1;
    double z2;
    double e1;                           //Расстояние между численными траекториями, полученными с разными шагами
    double e2;
    double e;
    double S1;                           //Оценка локальной погрешности двух половинных шагов
    double S2;
    double S_max = 0;                   //Максимум ОЛП
    size_t C1 = 0;                      //Счётчик удвоений шага
    size_t C2 = 0;                      //Счётчик деления шага
    double h_max = h;                   //Максимум шага траектории
    double x_h_max = 0;                 //Точка, в которой достигается максимум шага траектории
    double h_min = h;                   //Минимум шага траектории
    double x_h_min = 0;                 //Точка, в которой достигается минимум шага траектории
    bool was_branch = false;            //Нужно ли пересчитывать численную траекторию с повышенной точностью
    while (N < Nmax && b - x - h >= std::numeric_limits<double>::epsilon() && v1 < std::numeric_limits<double>::max() && v2 < std::numeric_limits<double>::max())  // Если v1 или v2 перейдут в inf то цикл завершится (нижно ли делать для min???)
    {
        if (was_branch)
        {
            v = v1;
            z = z1;
        }
        else
        {
            z = z0 + a * RK4(v, z0,h,f1);
            v = v0 + RK4(x, v0, h, f2);
        }

        z1 = z0 + a * RK4(v, z0,h * 0.5,f1);
        z2 = z1 + a * RK4(v, z1,h * 0.5,f1);
        v1 = v0 + RK4(x, v0, h * 0.5, f2);
        v2 = v1 + RK4(x, v1, h * 0.5, f2);

        e1 = z2 - z < std::numeric_limits<double>::epsilon() ? z - z2 : z2 - z;
        e2 = v2 - v < std::numeric_limits<double>::epsilon() ? v - v2 : v2 - v;
        S1 = e1 / 15.;
        S2 = e2 / 15.;
        e = std::max(e1,e2);
        double S = std::max(S1,S2);
        double s = std::min(S1,S2);
        if (S_max - S < std::numeric_limits<double>::epsilon())
            S_max = S;

        if (eps - S < std::numeric_limits<double>::epsilon())
        {
            ++C2;
            h *= 0.5;
            if (h - h_min < std::numeric_limits<double>::epsilon())
            {
                h_min = h;
                x_h_min = x;
            }
            was_branch = true;
            continue;
        }

        x += h;

        if (32. * s - eps < std::numeric_limits<double>::epsilon())
        {
            ++C1;
            h *= 2;
            if (h_max - h < std::numeric_limits<double>::epsilon())
            {
                h_max = h;
                x_h_max = x;
            }
        }

        out << N + 1 << setw(20) << x << setw(20) << v << setw(20) << v2 << setw(20) << e << setw(20) << S << setw(20)
            << h << setw(20) << C1 << setw(20) << C2 << endl;

        was_branch = false;
        v0 = v;
        ++N;
    }

    out << "n = " << N << std::endl
        << "b - xn = " << b - x << std::endl
        << "max loc err = " << S_max << std::endl
        << "max hi = " << h_max << " on " << x_h_max << std::endl
        << "min hi = " << h_min << " on " << x_h_min << std::endl;
};


int main(int argc, char** argv)
{
    size_t Nmax; //
    double b ;
    double eps ;
    double a ;


    //----НОВОЕ----

    int Number_of_x = 1000;   //  Задаёт пользователь ВМЕСТО h  для задачи А  (Не работает в задаче Б)

    //-------------



    double x0 = 0.;
    double v0 ;
    double z0 ;
    double h0 ;


//    std::string url = argv[0];
//    url.erase((url.size() - 3), 3);

    ifstream readFile("param.txt", ios::in);

    if (!readFile.is_open())
    {
        cerr << "File param.txt didn't open!" << endl;
        return 2;
    }

    string buffer;
    //TODO Переделать, если есть лучший способ чтения из файла
    getline(readFile, buffer, '=');
    readFile >> z0;
    getline(readFile, buffer, '=');
    readFile >> v0;
    getline(readFile, buffer, '=');
    readFile >> h0;
    getline(readFile, buffer, '=');
    readFile >> Nmax;
    getline(readFile, buffer, '=');
    readFile >> b;
    getline(readFile, buffer, '=');
    readFile >> eps;
    getline(readFile, buffer, '=');
    readFile >> a;

    //-------------- НОВОЕ ------------------------------

    // Сначала в файл.txt Объясняем пользователю, что теперь в задаче А пользователь вводит не шаг, а кол-во шагов

    getline(readFile, buffer, '=');
    readFile >> Number_of_x;         // Кол-во шагов в задаче А

    //---------------------------------------------------

    readFile.close();


    ofstream writeFile("Results-1a.txt", ios::out);
    
    if (!writeFile.is_open())
    {
        cerr << "File 1a.txt didn't open!" << endl;
        return 2;
    }

    Task1a(writeFile, Nmax, b, x0, Number_of_x/*НОВОЕ*/, v0, h0, f_test);

    writeFile.close();

    writeFile.open("Results-1b.txt", ios::out);

    if (!writeFile.is_open())
    {
        cerr << "File 1b.txt didn't open!" << endl;
        return 2;
    }

    Task1b(writeFile, Nmax, b, x0, v0, h0, eps, f_test);

    writeFile.close();

    writeFile.open("Results-2a.txt", ios::out);

    if (!writeFile.is_open())
    {
        cerr << "File 2a.txt didn't open!" << endl;
        return 2;
    }

    Task2a(writeFile, Nmax, b, x0, Number_of_x/*НОВОЕ*/, v0, h0, f1);

    writeFile.close();

    writeFile.open("Results-2b.txt", ios::out);

    if (!writeFile.is_open())
    {
        cerr << "File 2b.txt didn't open!" << endl;
        return 2;
    }

    Task2b(writeFile, Nmax, b, x0, v0, h0, eps, f1);

    writeFile.close();


    writeFile.open("Results-3a.txt", ios::out);

    if (!writeFile.is_open())
    {
        cerr << "File 3a.txt didn't open!" << endl;
        return 2;
    }

    Task3a(writeFile, Nmax, b, x0, Number_of_x/*НОВОЕ*/, v0, z0, h0, a, f2, f22);

    writeFile.close();

    writeFile.open("Results-3b.txt", ios::out);

    if (!writeFile.is_open())
    {
        cerr << "File 3b.txt didn't open!" << endl;
        return 2;
    }

    Task3b(writeFile, Nmax, b, x0, v0, z0, h0,a, eps, f2, f22);

    writeFile.close();
}

double testTrueTr(double x, double u)
{
    return exp( -0.5 * x);
}

double f_test(double x, double u)
{
    return -0.5 * u;
}

double f1(double x, double u)
{
    return (1. / (1. + pow(x, 4))) * u * u + u - pow(u, 3) * sin(10. * x);
}

double f2(double x, double u)
{
    return -sin(u);
}

double f22(double x, double u)
{
    return u;
}

double RK4(double x, double v, double h, double(*f)(double, double))
{
    double k1 = f(x, v);
    double k2 = f(x + h * 0.5, v + h * 0.5 * k1);
    double k3 = f(x + h * 0.5, v + h * 0.5 * k2);
    double k4 = f(x + h , v + h * k3);                  
    return h / 6. * (k1 + 2. * k2 + 2. * k3 + k4);
}

