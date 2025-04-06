#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <chrono>
#define pi 3.14159265

//using namespace std;
// kabancheg kabancheg kabancheg kabancheg kabancheg kabancheg
double last=0;
double GetTime(){
    double t = std::chrono::duration_cast<std::chrono::microseconds>
    (std::chrono::steady_clock::now().time_since_epoch()).count()/1e6;
    double res=t-last;
    last = t;
    return res;
}

void projectEllipse(float* c, float* u, float* v, float* n, const int N, float norm, sf::VertexArray& lines) {
    float** r3 = new float*[N]; //массив точек проекции эллипса в пространстве
    float** r2 = new float*[N]; //r3 в локальных координатах плоскости проектирования
    for(int i = 0; i < N; i++) {
        r3[i] = new float[3];
        r2[i] = new float[2];
    }
    for (int i = 0; i < N; i++) {
        float t = 2 * pi * i / (N - 1); //параметр уравнения эллипса
        float a1[3]; //a1=c-(c*n)*n
        float a2[3]; //a2=u-(u*n)*n
        float a3[3]; //a3=v-(v*n)*n
        for(int j = 0; j < 3; j++) {
            a1[j] = c[j] - (c[0]*n[0] + c[1]*n[1] + c[2]*n[2]) * n[j];
            a2[j] = u[j] - (u[0]*n[0] + u[1]*n[1] + u[2]*n[2]) * n[j];
            a3[j] = v[j] - (v[0]*n[0] + v[1]*n[1] + v[2]*n[2]) * n[j];
            r3[i][j] = a1[j] + a2[j] * std::cos(t) + a3[j] * std::sin(t);
        }
        if(n[0] != 0 || n[1] != 0) {
            r2[i][0]=(n[1]*r3[i][0] - n[0]*r3[i][1])/norm;
            r2[i][1] = r3[i][0] * n[0] * n[2] / std::sqrt(n[0] * n[0] + n[1] * n[1]) + r3[i][1] * n[1] * n[2] / std::sqrt(n[0] * n[0] + n[1] * n[1]) - r3[i][2] * std::sqrt(n[0] * n[0] + n[1] * n[1]);
        }
        else {
            r2[i][0] = r3[i][0];
            r2[i][1] = r3[i][1] * -1;
        }
        /*r3[i][0] += 10;
        r3[i][0] *= 20;
        r3[i][1] += 10;
        r3[i][1] *= 20;
        r3[i][2] += 10;
        r3[i][2] *= 20;*/
        r2[i][0] += 15;
        r2[i][0] *= 50;
        r2[i][1] += 10;
        r2[i][1] *= 50;
        lines[i].position = sf::Vector2f(r2[i][0], r2[i][1]);
    }

    for(int i = 0; i < N; i++) {
        delete[] r3[i];
        delete[] r2[i];
    }
    delete[] r3;
    delete[] r2;
}

int main()
{
    //инициализация окна приложения
    sf::RenderWindow window(sf::VideoMode(), "SPUTNIK Tracker", sf::Style::Fullscreen);

    //векторные параметры эллипса и плоскости
    //cout << "Hello world!" << endl;
    float c1[3] = {0, 0, 0}; //центр эллипса
    float u1[3] = {5, 0, 0}; //большая полуось эллипса
    float v1[3] = {0, 5, 0}; //малая полуось эллипса
    float c2[3] = {1, 0, 0};
    float u2[3] = {0, 3, 0};
    float v2[3] = {0, 0, 4};
    float n[3] = {1, 0, -1}; //нормаль плоскости; плоскость проходит через начало координат, поэтому определение "опорной" точки не требуется
    float norm = std::sqrt(n[0] * n[0] + n[1] * n[1]); //нормировочный коэффициент для вектора базиса локальной системы координат, лежащего в плоскости xy
    const int N = 100; //число точек для отрисовки эллипса + 1
    /*
    получение массива точек эллипса
    уравнение проекции вектора из пространства на плоскость:
    r(t)=a1+a2*cos(t)+a3*cos(t)
    a1=c-(c*n)*n
    a2=u-(u*n)*n

    */

    GetTime(); //активация отладочного таймера
    sf::VertexArray lines1(sf::LinesStrip, N);
    sf::VertexArray lines2(sf::LinesStrip, N);

    projectEllipse(c1, u1, v1, n, N, norm, lines1);
    projectEllipse(c2, u2, v2, n, N, norm, lines2);

    std::cout << GetTime() << std::endl; //отображение времени расчётов с помощью отладочного таймера
    window.draw(lines1);
    window.draw(lines2);
    window.display();
    std::cout << GetTime() << std::endl; //отображение времени отрисовки с помощью отладочного таймера

    bool z = 0;
    float x1 = 0;
    float y1 = 0;

    while (window.isOpen())
	{
        sf::Event event;
        window.clear();
        window.draw(lines1);
        window.draw(lines2);
	    while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) // Если зажата левая кнопка мыши
        { // тогда новый угол будем отсчитывать  в зависимости от старого
            if (z == 0)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                x1 = mousePos.x;
                y1 = mousePos.y;
                //std::cout << x1 << " " << y1 << std::endl;
                z = 1;
            }
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            float dx = x1 - mousePos.x;
            float dy = y1 - mousePos.y;
            float theta = std::acos(n[2]);
            float phi = std::atan(n[1] / n[0]);
            if(n[0] < 0)
                phi = pi+phi;
            theta -= 0.01 * dy;
            phi += 0.01 * dx;
            n[0] = std::sin(theta)*std::cos(phi);
            n[1] = std::sin(theta)*std::sin(phi);
            n[2] = std::cos(theta);
            norm = std::sqrt(n[0] * n[0] + n[1] * n[1]);
            projectEllipse(c1, u1, v1, n, N, norm, lines1);
            projectEllipse(c2, u2, v2, n, N, norm, lines2);
            x1 = mousePos.x;
            y1 = mousePos.y;
            //window.draw(lines);
            //std::cout << dx << " " << dy << std::endl;
        }
        else // момент отжатия мыши
        {
            z = 0;
        }

        window.display();
	}

    return 0;
}

