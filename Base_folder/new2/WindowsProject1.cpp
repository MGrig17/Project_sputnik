/**
  * Terminology:
  * "Physical point" -- a point located in 3-dimensional virtual space, which coordinates are stored in km units
  * "Graphical point" -- a point located in 2-dimensional space which corresponds to graphical window. GP coordinates are stored in px units (variables of integer-like type)
  */

#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif
#include <windows.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <commctrl.h>
#include "Simulation.h"
#include "Callbacks.h"
#define NUM_POINTS 100
#define R_earth 6400
#define x_c 600
#define y_c 300
HINSTANCE hInst;
HWND hPanel;
HWND knopka1;
HWND hEdit3;
HWND hEdit1;
HWND hEdit2;
HWND hEditColor1;
HWND hEditColor2;
HWND hEditColor3;
HWND hUpDown1;
HWND hUpDown2;
HWND hUpDown3;
HWND hButton;
float timeStep = -1;
HWND StepStep;
HWND hTextLine1;
HWND hTextLine2;
HWND hTextTimeStep;
HWND hTextColor;
const int CONTROL_HEIGHT = 100;

std::string name;
std::string line1;
std::string line2;
float n[3] = { 1.0f, 0.0f, -1.0f };
bool isDragging = false;
bool ytro_bobroe = false;
POINT lastMousePos;
float k_zoom = 0.04;

LONG R_e = R_earth * k_zoom;
Simulation sim(k_zoom);

#pragma comment(lib, "Comctl32.lib")
// ����� �����
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    CreateAndRedirectConsole(); // ��� ��� ������� � ������ � �������

    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icc);

    float c1[3] = { 0, 0, 0 };
    float u1[3] = { 5, 0, 0 };
    float v1[3] = { 0, 5, 0 };
    float c2[3] = { 1, 0, 0 };
    float u2[3] = { 0, 3, 0 };
    float v2[3] = { 0, 0, 4 };
    float norm = std::sqrt(n[0] * n[0] + n[1] * n[1]);

    //TLE processing begin

    // ����������� ������ ����
    hInst = hInstance;
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SputnikTracker";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"������ ����������� ������ ����!", L"������", MB_ICONERROR);
        return 1;
    }
    HWND hWnd = CreateWindow(
        L"SputnikTracker", L"SPUTNIK Tracker",
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_VISIBLE | WS_CLIPSIBLINGS,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL, hInstance, NULL
    );
    if (!hWnd) {
        MessageBox(NULL, L"������ �������� ����!", L"������", MB_ICONERROR);
        return 1;
    }

    /*
    for (int i = 0; i <= 255; ++i) {
        wchar_t buffer[10];
        swprintf(buffer, 10, L"%d", i);
        SendMessageW(hEditColor1, CB_ADDSTRING, 0, (LPARAM)buffer);
        SendMessageW(hEditColor2, CB_ADDSTRING, 0, (LPARAM)buffer);
        SendMessageW(hEditColor3, CB_ADDSTRING, 0, (LPARAM)buffer);
    }
    SendMessageW(hEditColor1, CB_SETCURSEL, 0, 0);
    SendMessageW(hEditColor2, CB_SETCURSEL, 0, 0);
    SendMessageW(hEditColor3, CB_SETCURSEL, 0, 0);*/
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif

























/*#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include <windows.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#include "sgp4/SGP4.h"
#include "sgp4/Tle.h"
#include "sgp4/Util.h"
#include "sgp4/Vector.h"
#include "sgp4/Globals.h"
#include "sgp4/DateTime.h"
#include "sgp4/TimeSpan.h"

#include <stdio.h> // это мне для консольки
#include <io.h>
#include <fcntl.h>
#include <commctrl.h>

#define pi 3.14159265f
#define NUM_POINTS 100
#define R_earth 6400 //радиус Земли в км
#define x_c 600
#define y_c 300

#define IDC_MYBUTTON 101 // которая Создать спутник
#define IDC_Name_satellite 102
#define IDC_EDITBOX1 103
#define IDC_EDITBOX2 104
#define IDC_TIMESTEP 105
#define IDC_COLOR1 106
#define IDC_COLOR2 107
#define IDC_COLOR3 108

#define ID_PANEL 200

HINSTANCE hInst;
HWND hPanel;

HWND knopka1; // кнопка "Создать спутник"
HWND hEdit3; // для ввода имени спутника
HWND hEdit1; // тле1
HWND hEdit2;// тле2
HWND hEditColor1; // выбор цвета
HWND hEditColor2;
HWND hEditColor3;
HWND hUpDown1; //это для стрелочек
HWND hUpDown2;
HWND hUpDown3;
int k1 = 875; // x-позиция первого
int k2 = 80; // ширина ячейки
int k3 = 30; // высота ячейки комбо-бокс для цвета
int color_R=255, color_G=0, color_B=0;
HWND StepStep;
//Текст на экране
HWND hTextLine1;
HWND hTextLine2;
HWND hTextTimeStep;
HWND hTextColor;
const int CONTROL_HEIGHT = 100; // высота дочернего окна

// и ничего вы мне не сделаете
std::string name;
std::string line1;
std::string line2;
float n[3] = { 1.0f, 0.0f, -1.0f };
bool isDragging = false;
bool ytro_bobroe = false;
POINT lastMousePos;
//координаты центра окна
float k_zoom = 0.04; //масштабный коэффициент, число пикселей на км
const float dk_zoom = 0.00005; //изменение масштабного коэффициента при прокрутке колёсика мыши на 1 у. е.

LONG R_e = R_earth * k_zoom; //радиус Земли в пикселях

std::string WideToUTF8(const wchar_t* wideStr) {
    int inputLen = static_cast<int>(wcslen(wideStr));
    int size = WideCharToMultiByte(CP_UTF8, 0, wideStr, inputLen, nullptr, 0, nullptr, nullptr);
    std::string utf8Str(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wideStr, inputLen, &utf8Str[0], size, nullptr, nullptr);
    return utf8Str;
}
void CreateAndRedirectConsole() { // это мне для создания консольки
    AllocConsole();

    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::cout.clear();
    std::wcout.clear();
    std::cerr.clear();
    std::wcerr.clear();
}
struct GraphPoint {
    POINT point;
    bool mask;
    float r3[3];
};
template<typename T>
struct Triplet {
    T num1;
    T num2;
    T num3;
};
Triplet<float>* ellipse(float* c, float* u, float* v, int N) {
    Triplet<float>* r3 = new Triplet<float>[N];
    for (int i = 0; i < N; i++) {
        float t = 2 * pi * i / (N - 1);
        r3[i].num1 = c[0] + u[0] * std::cos(t) + v[0] * std::sin(t);
        r3[i].num2 = c[1] + u[1] * std::cos(t) + v[1] * std::sin(t);
        r3[i].num3 = c[2] + u[2] * std::cos(t) + v[2] * std::sin(t);
    }
    return r3;
}
class PointsCloud {
public:
    PointsCloud() {
        N = 0;
        points = nullptr;
        r3 = nullptr;
    }
    PointsCloud(const int& N_) {
        N = N_;
        points = new GraphPoint[N];
        r3 = new Triplet<float>[N];
    }
    PointsCloud(PointsCloud& pc) {
        cout << "Clone constructor is called" << endl;
        N = pc.get_length();
        points = new GraphPoint[N];
        r3 = new Triplet<float>[N];
        for (int i = 0; i < N; i++) {
            points[i].point = pc.get_points()[i].point;
            points[i].mask = pc.get_points()[i].mask;
            r3[i].num1 = pc.get_r3()[i].num1;
            r3[i].num2 = pc.get_r3()[i].num2;
            r3[i].num3 = pc.get_r3()[i].num3;
        }
        cout << "Cloning has been completed" << endl;
    }
    PointsCloud& operator= (const PointsCloud& pc) {
        cout << "Custom assignment operator is called" << endl;
        delete[] points;
        delete[] r3;
        N = pc.get_length();
        points = new GraphPoint[N];
        r3 = new Triplet<float>[N];
        for (int i = 0; i < N; i++) {
            points[i].point = pc.get_points()[i].point;
            points[i].mask = pc.get_points()[i].mask;
            r3[i].num1 = pc.get_r3()[i].num1;
            r3[i].num2 = pc.get_r3()[i].num2;
            r3[i].num3 = pc.get_r3()[i].num3;
        }
        cout << "Custom assignment has been completed" << endl;
    }
    void projectCloud(const LONG& R_e, const float* n, const float& k_zoom) {
        //cout << boolalpha;
        const float norm = std::sqrt(n[0] * n[0] + n[1] * n[1]); //нормировочный коэффициент для вектора базиса локальной системы координат, лежащего в плоскости xy
        float r2[2]; //элемент r3 в локальных координатах плоскости проектирования
        for (int i = 0; i < N; i++) {
            r2[0] = 0;
            r2[1] = 0;
            float t = 2 * pi * i / (N - 1); //параметр уравнения эллипса
            if (n[0] != 0 || n[1] != 0) {
                r2[0] = (n[1] * r3[i].num1 - n[0] * r3[i].num2) / norm;
                r2[1] = r3[i].num1 * n[0] * n[2] / std::sqrt(n[0] * n[0] + n[1] * n[1]) + r3[i].num2 * n[1] * n[2] / std::sqrt(n[0] * n[0] + n[1] * n[1]) - r3[i].num3 * std::sqrt(n[0] * n[0] + n[1] * n[1]);
            }
            else {
                r2[0] = r3[i].num1;
                r2[1] = r3[i].num2 * -1;
            }
            r2[0] *= k_zoom;
            r2[0] += x_c;
            r2[1] *= k_zoom;
            r2[1] += y_c;
            points[i].point.x = (LONG)r2[0];
            points[i].point.y = (LONG)r2[1];
            points[i].mask = (std::sqrt((points[i].point.x - x_c) * (points[i].point.x - x_c) + (points[i].point.y - y_c) * (points[i].point.y - y_c)) > R_e) || (r3[i].num1 * n[0] + r3[i].num2 * n[1] + r3[i].num3 * n[2] > 0);
        }
        //cout << points[0].point.x << points[0].point.y << points[0].mask;
        //cout << points[30].point.x << points[30].point.y << points[30].mask;
        //cout << noboolalpha << endl;
    }
    int draw(const HDC& hdc, const int& red, const int& green, const int& blue) {
        if (N != 0) {
            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(red, green, blue));
            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
            MoveToEx(hdc, (int)points[0].point.x, (int)points[0].point.y, NULL);
            for (size_t i = 1; i < N; i++) {
                if (points[i].mask) {
                    if (points[i - 1].mask)
                        LineTo(hdc, (int)points[i].point.x, (int)points[i].point.y);
                    else
                        MoveToEx(hdc, (int)points[i].point.x, (int)points[i].point.y, NULL);
                }
            }
            if (points[0].mask) {
                if (points[N - 1].mask)
                    LineTo(hdc, (int)points[0].point.x, (int)points[0].point.y);
                else
                    MoveToEx(hdc, (int)points[0].point.x, (int)points[0].point.y, NULL);
            }
            SelectObject(hdc, hOldPen);
            DeleteObject(hPen);
            return 0;
        }
        else {
            cerr << "RUNTIME EXCEPTION: attempted to draw a null-PointsCloud" << endl;
            return -1;
        }
    }
    void set_r3(float**& r3_, const int& N_) {
        N = N_;
        if (points != nullptr)
            delete[] points;
        delete[] r3;
        points = new GraphPoint[N];
        r3 = new Triplet<float>[N];
        cout << N << endl;
        for (int i = 0; i < N; i++) {
            cout << i << endl;
            r3[i].num1 = r3_[i][0];
            r3[i].num2 = r3_[i][1];
            r3[i].num3 = r3_[i][2];
        }
    }
    void set_r3(Triplet<float>*& r3_, const int& N_) {
        N = N_;
        if (points != nullptr)
            delete[] points;
        delete[] r3;
        points = new GraphPoint[N];
        r3 = new Triplet<float>[N];
        cout << N << endl;
        for (int i = 0; i < N; i++) {
            cout << i << endl;
            r3[i].num1 = r3_[i].num1;
            r3[i].num2 = r3_[i].num2;
            r3[i].num3 = r3_[i].num3;
        }
    }
    int get_length() const {
        return N;
    }
    GraphPoint* get_points() const {
        return points;
    }
    Triplet<float>* get_r3() const {
        return r3;
    }
    ~PointsCloud() {
        cout << "PointsCloud destructor is called" << endl;
        delete[] points;
        delete[] r3;
    }
protected:
    GraphPoint* points; //точки для отрисовки проекции в графическом виджете
    Triplet<float>* r3;
    int N;
};
class Orbit : public PointsCloud {
public:
    Orbit() : PointsCloud() {
        //timeFirst = libsgp4::DateTime(0);
        //timeStep = 0;
        //polar.clear();
    }
    Orbit(int N_) : PointsCloud(N_) {
        //timeFirst = libsgp4::DateTime(0);
        //timeStep = 0;
        //polar.clear();
    }
    Orbit(const Orbit& o) {
        cout << "Clone constructor is called" << endl;
        N = o.get_length();
        points = new GraphPoint[N];
        r3 = new Triplet<float>[N];
        for (int i = 0; i < N; i++) {
            points[i].point = o.get_points()[i].point;
            points[i].mask = o.get_points()[i].mask;
            r3[i].num1 = o.get_r3()[i].num1;
            r3[i].num2 = o.get_r3()[i].num2;
            r3[i].num3 = o.get_r3()[i].num3;
        }
        //polar = o.polar;
        //timeStep = o.timeStep;
        //timeFirst = o.timeFirst;
        cout << "Cloning has been completed" << endl;
    }
    Orbit& operator= (const Orbit& o) {
        cout << "Custom assignment operator is called" << endl;
        delete[] points;
        delete[] r3;
        N = o.get_length();
        points = new GraphPoint[N];
        r3 = new Triplet<float>[N];
        for (int i = 0; i < N; i++) {
            points[i].point = o.get_points()[i].point;
            points[i].mask = o.get_points()[i].mask;
            r3[i].num1 = o.get_r3()[i].num1;
            r3[i].num2 = o.get_r3()[i].num2;
            r3[i].num3 = o.get_r3()[i].num3;
        }
        //polar = o.polar;
        //timeStep = o.timeStep;
        //timeFirst = o.timeFirst;
        cout << "Custom assignment has been completed" << endl;
    }*/
    /*void SkyProject(const float* obs, const libsgp4::DateTime& timeStart, const libsgp4::DateTime& timeEnd) {
        libsgp4::DateTime time = timeStart;
        while (time < timeEnd) {
            libsgp4::TimeSpan dt_raw = time - timeFirst;
            float dt_imd = dt_raw.TotalSeconds(); //dt_intermediate
            float dt_rfd = (long)dt_imd % (int)(timeStep * N); //dt_refined
            int i = dt_rfd / timeStep; //r3 array index of element which corresponds to the satellite position at 'time'
        }
    }*/
/*
    //void set_timeStep(float timeStep_) { timeStep = timeStep_; }
    //float get_timeStep() { return timeStep; }
    //void set_timeFirst(libsgp4::DateTime timeFirst_) { timeFirst = timeFirst_; }
    //libsgp4::DateTime get_timeFirst() { return timeFirst; }
    ~Orbit() {
        cout << "Orbit destructor is called" << endl;
    }

private:
    //vector<GraphPoint> polar;
    //vector<libsgp4::DateTime> polar_t; //datetimes corresponding to points in polar.
    //float timeStep; //time interval in seconds between two consequent points
    //libsgp4::DateTime timeFirst; //datetime corresponding to the first point in r3 array
};
class Simulation {
public:
    Simulation() {
        k_zoom = 0.02;
        earth = new POINTS[100];
        surface = new PointsCloud[7];
        float c[3] = { 0, 0, 0 };
        float v[3] = { 0, 0, R_earth };
        for (int i = 0; i < 4; i++) {
            float u[3] = { R_earth * std::sin(pi / 4 * i), R_earth * std::cos(pi / 4 * i), 0 };
            Triplet<float>* new_ellipse = ellipse(c, u, v, 30);
            surface[i].set_r3(new_ellipse, 30);
        }
        float u[3];
        v[0] = 0;
        v[1] = R_earth;
        v[2] = 0;
        u[0] = R_earth;
        u[1] = 0;
        u[2] = 0;
        Triplet<float>* new_ellipse = ellipse(c, u, v, 30);
        surface[4].set_r3(new_ellipse, 30);


        c[2] = R_earth / sqrt(2);
        v[0] = 0;
        v[1] = sqrt(R_earth * R_earth - c[2] * c[2]);
        v[2] = 0;
        u[0] = sqrt(R_earth * R_earth - c[2] * c[2]);
        u[1] = 0;
        u[2] = 0;
        new_ellipse = ellipse(c, u, v, 30);
        surface[5].set_r3(new_ellipse, 30);

        c[2] = -R_earth / sqrt(2);
        v[0] = 0;
        v[1] = sqrt(R_earth * R_earth - c[2] * c[2]);
        v[2] = 0;
        u[0] = sqrt(R_earth * R_earth - c[2] * c[2]);
        u[1] = 0;
        u[2] = 0;
        new_ellipse = ellipse(c, u, v, 30);
        surface[6].set_r3(new_ellipse, 30);

        updateEarth();
    }
    Simulation(const float& k_zoom_) {
        k_zoom = k_zoom_;
        earth = new POINTS[100];
        surface = new PointsCloud[7];
        float c[3] = { 0, 0, 0 };
        float v[3] = { 0, 0, R_earth };
        for (int i = 0; i < 4; i++) {
            float u[3] = { R_earth * std::sin(pi / 4 * i), R_earth * std::cos(pi / 4 * i), 0 };
            Triplet<float>* new_ellipse = ellipse(c, u, v, 30);
            surface[i].set_r3(new_ellipse, 30);
        }
        float u[3];
        v[0] = 0;
        v[1] = R_earth;
        v[2] = 0;
        u[0] = R_earth;
        u[1] = 0;
        u[2] = 0;
        Triplet<float>* new_ellipse = ellipse(c, u, v, 30);
        surface[4].set_r3(new_ellipse, 30);


        c[2] = R_earth / sqrt(2);
        v[0] = 0;
        v[1] = sqrt(R_earth * R_earth - c[2] * c[2]);
        v[2] = 0;
        u[0] = sqrt(R_earth * R_earth - c[2] * c[2]);
        u[1] = 0;
        u[2] = 0;
        new_ellipse = ellipse(c, u, v, 30);
        surface[5].set_r3(new_ellipse, 30);

        c[2] = -R_earth / sqrt(2);
        v[0] = 0;
        v[1] = sqrt(R_earth * R_earth - c[2] * c[2]);
        v[2] = 0;
        u[0] = sqrt(R_earth * R_earth - c[2] * c[2]);
        u[1] = 0;
        u[2] = 0;
        new_ellipse = ellipse(c, u, v, 30);
        surface[6].set_r3(new_ellipse, 30);

        updateEarth();
    }
    void add_orbit(const string& name, const string& tle_line1, const string& tle_line2, const float& time_step, const int& clRed, const int& clGreen, const int& clBlue) {
        const libsgp4::Tle tle(name, tle_line1, tle_line2);
        const libsgp4::SGP4 sgp4(tle);
        cout << "TLE epoch: " << tle.Epoch() << endl;
        const float minutes_in_day = 60 * 24;
        const float end_time = minutes_in_day / tle.MeanMotion();
        const int r_len = end_time / time_step;
        float** r = new float* [r_len];
        for (int i = 0; i < r_len; i++) {
            libsgp4::Eci eci = sgp4.FindPosition(i * time_step);
            //std::cout << eci.Position() << std::endl;
            //cout << i << endl;
            r[i] = new float[3];
            r[i][0] = eci.Position().x;
            r[i][1] = eci.Position().y;
            r[i][2] = eci.Position().z;
        }
        Orbit new_orbit = Orbit();
        new_orbit.set_r3(r, r_len);
        //new_orbit.set_timeStep(time_step * 60);
        //new_orbit.set_timeFirst(tle.Epoch());
        Triplet<int> new_orbit_color = { clRed, clGreen, clBlue };
        satellites.push_back(new_orbit);
        sat_color.push_back(new_orbit_color);
        for (int i = 0; i < r_len; i++) {
            delete[] r[i];
        }
        delete[] r;
    }
    int draw(const HDC& hdc) {
        int res = 0;
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        MoveToEx(hdc, earth[0].x, earth[0].y, NULL);
        for (size_t i = 1; i < 100; i++) {
            LineTo(hdc, earth[i].x, earth[i].y);
        }
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
        LineTo(hdc, earth[0].x, earth[0].y);
        for (int i = 0; i < 7; i++) {
            res += surface[i].draw(hdc, 0, 0, 0);
        }
        for (int i = 0; i < satellites.size(); i++) {
            res += satellites[i].draw(hdc, sat_color[i].num1, sat_color[i].num2, sat_color[i].num3);
        }
        if (res != 0) {
            cerr << "RUNTIME EXCEPTION: Method 'draw' of an object of 'Graph3D' class has malfunctioned " << -res << " times" << endl;
        }
        return res;
    }
    void updateGraph3D(const float* n) {
        for (int i = 0; i < satellites.size(); i++) {
            satellites[i].projectCloud(R_earth * k_zoom, n, k_zoom);
        }
        for (int i = 0; i < 7; i++) {
            surface[i].projectCloud(R_earth * k_zoom, n, k_zoom);
        }
    }
    void scroll(const float& dk_zoom, const float* n) {
        k_zoom += dk_zoom;
        updateEarth();
        updateGraph3D(n);
    }
private:
    void updateEarth() {
        for (int i = 0; i < 100; i++) {
            earth[i].x = R_earth * k_zoom * std::sin((float)i / 50 * pi) + x_c;
            earth[i].y = R_earth * k_zoom * std::cos((float)i / 50 * pi) + y_c;
        }
    }
    POINTS* earth;
    float k_zoom;
    vector<Triplet<int>> sat_color;
    vector<Orbit> satellites;
    PointsCloud* surface;
};
Simulation sim(k_zoom);

void ResizeWindowTo90Percent(HWND hWnd) {
    RECT workArea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
    int width = (workArea.right - workArea.left) * 0.9; // - вместо 90% можно сделать любое другое кол-во процентов
    int height = (workArea.bottom - workArea.top) * 0.9;
    int x = workArea.left + (workArea.right - workArea.left - width) / 2;
    int y = workArea.top + (workArea.bottom - workArea.top - height) / 2;
    SetWindowPos(
        hWnd,
        NULL,
        x, y,
        width, height,
        SWP_NOZORDER | SWP_FRAMECHANGED
    );
}
LRESULT CALLBACK PanelProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    switch (message)
    {
    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED)
        {
            if (LOWORD(wParam) == IDC_MYBUTTON)
            {
                //Собираем компроматы на спутники
                wchar_t buffer1[256];
                GetDlgItemTextW(hWnd, IDC_EDITBOX1, buffer1, 256);
                std::string text1 = WideToUTF8(buffer1);
                wchar_t buffer2[256];
                GetDlgItemTextW(hWnd, IDC_EDITBOX2, buffer2, 256);
                std::string text2 = WideToUTF8(buffer2);
                wchar_t buffer3[256];
                GetDlgItemTextW(hWnd, IDC_Name_satellite, buffer3, 256);
                std::string name_satellite = WideToUTF8(buffer3);

                wchar_t buffer4[256]; // для TimeStep
                GetDlgItemTextW(hWnd, IDC_TIMESTEP, buffer4, 256);
                // имя, возраст,зачем вылетели, куда вылетели, когда вылетел, налог на смотрение на звезды
                name = name_satellite;
                line1 = text1;
                line2 = text2;
                // Попробую, чтобы не создавать миллион переменных, переопределить буферы 1, 2, 3
                GetDlgItemTextW(hWnd, IDC_COLOR1, buffer1, 256);
                GetDlgItemTextW(hWnd, IDC_COLOR2, buffer2, 256);
                GetDlgItemTextW(hWnd, IDC_COLOR3, buffer3, 256);
                // в общем, тут либо раскомментировать первую функцию (до WideToUTF8)
                //преобразования в строку с нужной кодировкой, чтобы строка заканчивалась нулём-терминатором, 
                // либо переводить встроенной функцией tututu.c_str;
                color_R = atoi((WideToUTF8(buffer1)).c_str());
                color_G = atoi((WideToUTF8(buffer2)).c_str());
                color_B = atoi((WideToUTF8(buffer3)).c_str());
                if (color_R > 255) color_R = 255; if (color_R < 0) color_R = 0;
                if (color_G > 255) color_G = 255; if (color_G < 0) color_G = 0;
                if (color_B > 255) color_B = 255;if (color_B < 0) color_B = 0;
                
                //libsgp4::Tle tle(name, line1, line2);
                //libsgp4::SGP4 sgp4(tle);
                
                // очищаем поля вводов
                SetDlgItemTextW(hWnd, IDC_EDITBOX1, L"");
                SetDlgItemTextW(hWnd, IDC_EDITBOX2, L"");
                SetDlgItemTextW(hWnd, IDC_COLOR1, L"");
                SetDlgItemTextW(hWnd, IDC_COLOR2, L"");
                SetDlgItemTextW(hWnd, IDC_COLOR3, L"");
                SetDlgItemTextW(hWnd, IDC_Name_satellite, L"");
                SetDlgItemTextW(hWnd, IDC_TIMESTEP, L"");

                float startTime = 0.0;
                float endTime = 100.0;
                float timeStep = atof((WideToUTF8(buffer4)).c_str()); //было 0.1

                sim.add_orbit(name, line1, line2, timeStep, color_R, color_G, color_B);
                sim.updateGraph3D(n);
                InvalidateRect(hWnd, NULL, TRUE);

                //MessageBox(GetParent(hWnd), L"Ура, кнопка нажата!", L"Сообщение", MB_OK);
                return TRUE;
            }

        }
        break;
    case WM_NCDESTROY:
        RemoveWindowSubclass(hWnd, PanelProc, uIdSubclass);
        break;
    }
    return DefSubclassProc(hWnd, message, wParam, lParam);
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        hPanel = CreateWindow(
            L"STATIC", NULL,
            WS_CHILD | WS_VISIBLE | SS_SUNKEN | WS_CLIPCHILDREN,
            0, 0, 0, CONTROL_HEIGHT,
            hWnd, (HMENU)ID_PANEL, NULL, NULL
        );
        hTextLine1 = CreateWindowW(L"STATIC", L"Введите TLE Line 1",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            12, 20,  // Позиция (x,y) относительно панели
            200, 15, // Ширина и высота
            hPanel,  // Родитель - панель
            NULL, hInst, NULL);
        hTextLine2 = CreateWindowW(L"STATIC", L"Введите TLE Line 2",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            12, 55,  // Позиция (x,y) относительно панели
            200, 15, // Ширина и высота
            hPanel,  // Родитель - панель
            NULL, hInst, NULL);
        hTextTimeStep = CreateWindowW(L"STATIC", L"Шаг",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            1000, 15,  // Позиция (x,y) относительно панели
            50, 15, // Ширина и высота
            hPanel,  // Родитель - панель
            NULL, hInst, NULL);
        hTextColor = CreateWindowW(L"STATIC", L"Цвет",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            820, 55,  // Позиция (x,y) относительно панели
            50, 15, // Ширина и высота
            hPanel,  // Родитель - панель
            NULL, hInst, NULL);
        // Подклассируем панель для обработки сообщений от ее дочерних элементов
        SetWindowSubclass(hPanel, PanelProc, 0, 0);
        knopka1 = CreateWindow(
            L"BUTTON", L"Создать спутник",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            490, 50, 300, 30,
            hPanel, (HMENU)IDC_MYBUTTON, hInst, NULL
        );
        StepStep = CreateWindow(
            L"EDIT", L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            1060, 15, 50, 30,
            hPanel, (HMENU)IDC_TIMESTEP, hInst, NULL 
        );
        
        hEditColor1 = CreateWindow(
            L"EDIT", L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            k1, 50, k2, k3,
            hPanel, (HMENU)IDC_COLOR1, hInst, NULL
        );
        hUpDown1 = CreateWindow(
            UPDOWN_CLASS, NULL,
            WS_VISIBLE | WS_CHILD | UDS_ALIGNRIGHT | UDS_SETBUDDYINT | UDS_ARROWKEYS,
            0, 0, 0, 0, hPanel, NULL, hInst, NULL
        );
        SendMessage(hUpDown1, UDM_SETBUDDY, (WPARAM)hEditColor1, 0);
        SendMessage(hUpDown1, UDM_SETRANGE, 0, MAKELPARAM(255, 0));
        hEditColor2 = CreateWindow(
            L"EDIT", L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            k1+k2+5, 50, k2, k3,
            hPanel, (HMENU)IDC_COLOR2, hInst, NULL 
        );
        hUpDown2 = CreateWindow(
            UPDOWN_CLASS, NULL,
            WS_VISIBLE | WS_CHILD | UDS_ALIGNRIGHT | UDS_SETBUDDYINT | UDS_ARROWKEYS,
            0, 0, 0, 0, hPanel, NULL, hInst, NULL
        );
        SendMessage(hUpDown2, UDM_SETBUDDY, (WPARAM)hEditColor2, 0);
        SendMessage(hUpDown2, UDM_SETRANGE, 0, MAKELPARAM(255, 0));
        k1 = k1 + k2 + 5;
        hEditColor3 = CreateWindow(
            L"EDIT", L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            k1+k2+5, 50, k2, k3,
            hPanel, (HMENU)IDC_COLOR3, hInst, NULL 
        );
        hUpDown3 = CreateWindow(
            UPDOWN_CLASS, NULL,
            WS_VISIBLE | WS_CHILD | UDS_ALIGNRIGHT | UDS_SETBUDDYINT | UDS_ARROWKEYS,
            0, 0, 0, 0, hPanel, NULL, hInst, NULL
        );
        SendMessage(hUpDown3, UDM_SETBUDDY, (WPARAM)hEditColor3, 0);
        SendMessage(hUpDown3, UDM_SETRANGE, 0, MAKELPARAM(255, 0));
        

        // ДАЛЕЕ TLE 
        hEdit3 = CreateWindow(
            L"EDIT", L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            490, 15, 300, 30,
            hPanel, (HMENU)IDC_Name_satellite, hInst, NULL //для названия спутника
        );
        hEdit1 = CreateWindow(
            L"EDIT", L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            180, 15, 300, 30,
            hPanel, (HMENU)IDC_EDITBOX1, hInst, NULL //для первоого тле
        );
        hEdit2 = CreateWindow(
            L"EDIT", L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            180, 50, 300, 30,
            hPanel, (HMENU)IDC_EDITBOX2, hInst, NULL //второй тле
        );
        ResizeWindowTo90Percent(hWnd);
        break;
    case WM_SIZE: {
        RECT rc;
        GetClientRect(hWnd, &rc);
        SetWindowPos(hPanel, NULL,
            0, rc.bottom - CONTROL_HEIGHT,
            rc.right, CONTROL_HEIGHT,
            SWP_NOZORDER);
        RECT mainRc = { 0, 0, rc.right, rc.bottom - CONTROL_HEIGHT };
        InvalidateRect(hWnd, &mainRc, FALSE);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_ENTERSIZEMOVE:
        isDragging = true; // Блокируем пересчёт при перемещении окна
        break;
    case WM_LBUTTONDOWN:
        isDragging = true;
        lastMousePos.x = LOWORD(lParam);
        lastMousePos.y = HIWORD(lParam);
        SetCapture(hWnd);
        break;
    case WM_MOUSEWHEEL:
        if (k_zoom > -dk_zoom * GET_WHEEL_DELTA_WPARAM(wParam) && (k_zoom < 0.1 || GET_WHEEL_DELTA_WPARAM(wParam) < 0))
            sim.scroll(dk_zoom * GET_WHEEL_DELTA_WPARAM(wParam), n);
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_MOUSEMOVE: {
        if (isDragging) {
            POINT currentMousePos;
            currentMousePos.x = LOWORD(lParam);
            currentMousePos.y = HIWORD(lParam);
            //cout << currentMousePos.x << " " << currentMousePos.y << endl;
            float dx = lastMousePos.x - currentMousePos.x;
            float dy = lastMousePos.y - currentMousePos.y;
            float theta = acosf(n[2]);
            float phi = atan2f(n[1], n[0]);
            theta += 0.01f * dy;
            phi += 0.01f * dx;
            n[0] = sinf(theta) * cosf(phi);
            n[1] = sinf(theta) * sinf(phi);
            n[2] = cosf(theta);
            float norm = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
            n[0] /= norm;
            n[1] /= norm;
            n[2] /= norm;
            float c1[3] = { 0, 0, 0 };
            float u1[3] = { 5, 0, 0 };
            float v1[3] = { 0, 5, 0 };
            float c2[3] = { 1, 0, 0 };
            float u2[3] = { 0, 3, 0 };
            float v2[3] = { 0, 0, 4 };
            float norm2D = sqrtf(n[0] * n[0] + n[1] * n[1]);
            //TLE processing begin
            sim.updateGraph3D(n);
            //TLE processing end
            // Перерисовываем окно
            InvalidateRect(hWnd, NULL, FALSE);
            lastMousePos = currentMousePos;
        }
        break;
    }

    case WM_LBUTTONUP:
        isDragging = false;
        ReleaseCapture();
        break;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            PostQuitMessage(0);
        }
        break;
    case WM_ERASEBKGND:
        return 1;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rc;
        GetClientRect(hWnd, &rc);
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbm = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
        HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, hbm);
        FillRect(hdcMem, &rc, (HBRUSH)(COLOR_WINDOW + 1));
        RECT mainArea = rc;
        mainArea.bottom -= CONTROL_HEIGHT;

        {
            int savedDC = SaveDC(hdcMem);
            HRGN hrgn = CreateRectRgnIndirect(&mainArea);
            SelectClipRgn(hdcMem, hrgn);
            DeleteObject(hrgn);
            HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
            HPEN hOldPen = (HPEN)SelectObject(hdcMem, hPen);
            sim.draw(hdcMem);

            SelectObject(hdcMem, hOldPen);
            DeleteObject(hPen);
            RestoreDC(hdcMem, savedDC);
        }

        BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdcMem, 0, 0, SRCCOPY);

        SelectObject(hdcMem, hOldBmp);
        DeleteObject(hbm);
        DeleteDC(hdcMem);

        EndPaint(hWnd, &ps);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
#pragma comment(lib, "Comctl32.lib")
// Точка входа
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    CreateAndRedirectConsole(); // это для консоли и вывода в консоль

    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icc);

    float c1[3] = { 0, 0, 0 };
    float u1[3] = { 5, 0, 0 };
    float v1[3] = { 0, 5, 0 };
    float c2[3] = { 1, 0, 0 };
    float u2[3] = { 0, 3, 0 };
    float v2[3] = { 0, 0, 4 };
    float norm = std::sqrt(n[0] * n[0] + n[1] * n[1]);

    //TLE processing begin
    
    // Регистрация класса окна
    hInst = hInstance;
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SputnikTracker";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Ошибка регистрации класса окна!", L"Ошибка", MB_ICONERROR);
        return 1;
    }
    HWND hWnd = CreateWindow(
        L"SputnikTracker", L"SPUTNIK Tracker",
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_VISIBLE | WS_CLIPSIBLINGS,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL, hInstance, NULL
    );
    if (!hWnd) {
        MessageBox(NULL, L"Ошибка создания окна!", L"Ошибка", MB_ICONERROR);
        return 1;
    }
    */
    /*
    for (int i = 0; i <= 255; ++i) {
        wchar_t buffer[10];
        swprintf(buffer, 10, L"%d", i);
        SendMessageW(hEditColor1, CB_ADDSTRING, 0, (LPARAM)buffer);
        SendMessageW(hEditColor2, CB_ADDSTRING, 0, (LPARAM)buffer);
        SendMessageW(hEditColor3, CB_ADDSTRING, 0, (LPARAM)buffer);
    }
    SendMessageW(hEditColor1, CB_SETCURSEL, 0, 0);
    SendMessageW(hEditColor2, CB_SETCURSEL, 0, 0);
    SendMessageW(hEditColor3, CB_SETCURSEL, 0, 0);*/
/*
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif
*/



// ЭТОТ КОД РАБОЧИЙ С КНОПКОЙ И БУФЕРАМИ
/*#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include <windows.h>
#include <commctrl.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#include "sgp4/SGP4.h"
#include "sgp4/Tle.h"
#include "sgp4/Util.h"
#include "sgp4/Vector.h"
#include "sgp4/Globals.h"

#define pi 3.14159265f
#define NUM_POINTS 100

#define IDC_MYBUTTON 101 // это великач кнопка на панели "Создать спутник"
#define IDC_EDITBOX1 102 // поле ввода пользователем, edit
#define IDC_EDITBOX2 104
#define IDC_MYBUTTON2 5 // прикола ради это кнопка, которая находится на экране отрисовки

#define ID_PANEL 200

//std::ofstream file("output.txt", ios::out);
std::ofstream file("output.txt", std::ios::binary);
HINSTANCE hInst;
HWND hPanel; // это и есть дочерняя панель с кнопками

HWND knopka1;
HWND hEdit; // для ввода данных пользователем (дескриптор)
HWND knopka2;
HWND knopka3;
const int CONTROL_HEIGHT = 100; // высота дочернего окна

float n[3] = { 1.0f, 0.0f, -1.0f };
bool isDragging = false;
bool ytro_bobroe = false;
POINT lastMousePos;

//координаты центра окна
float k_zoom = 0.03;
LONG x_c = 600;
LONG y_c = 300;
LONG R_e = 6400 * k_zoom;

std::string nonane;
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

std::string WideToUTF8(const wchar_t* wideStr) {
    int size = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, nullptr, 0, nullptr, nullptr);
    std::string utf8Str(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, &utf8Str[0], size, nullptr, nullptr);
    return utf8Str;
}
void CreateAndRedirectConsole() {
    // 1. Создаём консоль
    AllocConsole();

    // 2. Перенаправляем стандартные потоки
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);

    // 3. Настраиваем кодировку для кириллицы
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // 4. Очищаем буферы
    std::cout.clear();
    std::wcout.clear();
    std::cerr.clear();
    std::wcerr.clear();
}
class PointsCloud {
public:
    PointsCloud() {
        N = 0;
        r3 = nullptr;
        points = nullptr;
        mask = nullptr;
    }
    PointsCloud(const int& N_) {
        N = N_;
        r3 = new float* [N];
        for (int i = 0; i < N; i++) {
            r3[i] = new float[3];
            r3[i][0] = 0;
            r3[i][1] = 0;
            r3[i][2] = 0;
        }
        mask = new bool[N];
        for (int i = 0; i < N; i++) {
            mask[i] = true;
        }
        points = new POINT[N];
    }
    void projectCloud(const LONG& R_e, const LONG& x_c, const LONG& y_c, const float* n) {
        const float norm = std::sqrt(n[0] * n[0] + n[1] * n[1]); //нормировочный коэффициент для вектора базиса локальной системы координат, лежащего в плоскости xy
        float r2[2]; //элемент r3 в локальных координатах плоскости проектирования
        for (int i = 0; i < N; i++) {
            r2[0] = 0;
            r2[1] = 0;
            float t = 2 * pi * i / (N - 1); //параметр уравнения эллипса
            if (n[0] != 0 || n[1] != 0) {
                r2[0] = (n[1] * r3[i][0] - n[0] * r3[i][1]) / norm;
                r2[1] = r3[i][0] * n[0] * n[2] / std::sqrt(n[0] * n[0] + n[1] * n[1]) + r3[i][1] * n[1] * n[2] / std::sqrt(n[0] * n[0] + n[1] * n[1]) - r3[i][2] * std::sqrt(n[0] * n[0] + n[1] * n[1]);
            }
            else {
                r2[0] = r3[i][0];
                r2[1] = r3[i][1] * -1;
            }
            r2[0] *= k_zoom;
            r2[0] += x_c;
            r2[1] *= k_zoom;
            r2[1] += y_c;
            points[i].x = (LONG)r2[0];
            points[i].y = (LONG)r2[1];
            mask[i] = (std::sqrt((points[i].x - x_c) * (points[i].x - x_c) + (points[i].y - y_c) * (points[i].y - y_c)) > R_e) || (r3[i][0] * n[0] + r3[i][1] * n[1] + r3[i][2] * n[2] > 0);
        }
    }

    void set_r3(float**& r3_, const int& N_) {
        N = N_;
        r3 = r3_;

        if (points != nullptr)
            delete[] points;
        if (mask != nullptr)
            delete[] mask;
        points = new POINT[N];
        mask = new bool[N];
        r3_ = nullptr;
    }
    int get_length() {
        return N;
    }
    POINT* points; //точки для отрисовки проекции в графическом виджете
    bool* mask; //маска массива point: true - точка видна, false - точка закрыта Землёй
private:
    float** r3;
    int N;
};
PointsCloud pc1 = PointsCloud();

void ResizeWindowTo90Percent(HWND hWnd) {
    RECT workArea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
    int width = (workArea.right - workArea.left) * 0.9;
    int height = (workArea.bottom - workArea.top) * 0.9;
    int x = workArea.left + (workArea.right - workArea.left - width) / 2;
    int y = workArea.top + (workArea.bottom - workArea.top - height) / 2;
    SetWindowPos(
        hWnd,
        NULL,
        x, y,
        width, height,
        SWP_NOZORDER | SWP_FRAMECHANGED
    );
}

LRESULT CALLBACK PanelProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    switch (message)
    {
    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED)
        {
            if (LOWORD(wParam) == IDC_MYBUTTON)
            {
                
                wchar_t buffer1[256];
                GetDlgItemTextW(hWnd, IDC_EDITBOX1, buffer1, 256);
                //std::wstring text1 = buffer1;
                std::string text1 = WideToUTF8(buffer1);
                cout << text1;
                //wprintf(L"Текст: %s\n", text.c_str());

                const wchar_t newline[] = L"\r\n";
                file.write(reinterpret_cast<const char*>(newline), 4);

                wchar_t buffer2[256];
                GetDlgItemTextW(hWnd, IDC_EDITBOX2, buffer2, 256);
                file.write(reinterpret_cast<const char*>(buffer2), wcslen(buffer2) * sizeof(wchar_t));

                MessageBox(GetParent(hWnd), L"Ура, кнопка нажата!", L"Сообщение", MB_OK);
                return TRUE;
            }

        }
        break;
    case WM_NCDESTROY:
        RemoveWindowSubclass(hWnd, PanelProc, uIdSubclass);
        break;
    }
    return DefSubclassProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        hPanel = CreateWindow(
            L"STATIC", NULL,
            WS_CHILD | WS_VISIBLE | SS_SUNKEN | WS_CLIPCHILDREN,
            0, 0, 0, CONTROL_HEIGHT,
            hWnd, (HMENU)ID_PANEL, NULL, NULL
        );

        // Подклассируем панель для обработки сообщений от ее дочерних элементов
        SetWindowSubclass(hPanel, PanelProc, 0, 0);

        knopka2 = CreateWindow(
            L"BUTTON", L"кnopka",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            600, 50, 450, 30,
            hWnd, (HMENU)IDC_MYBUTTON2, hInst, NULL
        );
        knopka1 = CreateWindow(
            L"BUTTON", L"Создать спутник",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            600, 50, 450, 30,
            hPanel, (HMENU)IDC_MYBUTTON, hInst, NULL
        );

        hEdit = CreateWindow(
            L"EDIT", L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            600, 15, 450, 30,
            hPanel, (HMENU)IDC_EDITBOX1, hInst, NULL
        );
        hEdit = CreateWindow(
            L"EDIT", L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            120, 15, 450, 30,
            hPanel, (HMENU)IDC_EDITBOX2, hInst, NULL
        );
        ResizeWindowTo90Percent(hWnd);
        break;

    case WM_SIZE: {
        RECT rc;
        GetClientRect(hWnd, &rc);
        SetWindowPos(hPanel, NULL,
            0, rc.bottom - CONTROL_HEIGHT,
            rc.right, CONTROL_HEIGHT,
            SWP_NOZORDER);
        RECT mainRc = { 0, 0, rc.right, rc.bottom - CONTROL_HEIGHT };
        InvalidateRect(hWnd, &mainRc, FALSE);
        break;
    }
    case WM_DESTROY:
        if (file.is_open()) {
            file.close();
        }
        PostQuitMessage(0);
        break;
    case WM_ENTERSIZEMOVE:
        isDragging = true;
        break;
    case WM_LBUTTONDOWN:
        isDragging = true;
        lastMousePos.x = LOWORD(lParam);
        lastMousePos.y = HIWORD(lParam);
        SetCapture(hWnd);
        break;
    case WM_MOUSEMOVE: {
        if (isDragging) {
            POINT currentMousePos;
            currentMousePos.x = LOWORD(lParam);
            currentMousePos.y = HIWORD(lParam);
            float dx = lastMousePos.x - currentMousePos.x;
            float dy = lastMousePos.y - currentMousePos.y;
            float theta = acosf(n[2]);
            float phi = atan2f(n[1], n[0]);
            theta += 0.01f * dy;
            phi += 0.01f * dx;
            n[0] = sinf(theta) * cosf(phi);
            n[1] = sinf(theta) * sinf(phi);
            n[2] = cosf(theta);
            float norm = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
            n[0] /= norm;
            n[1] /= norm;
            n[2] /= norm;
            pc1.projectCloud(R_e, x_c, y_c, n);
            InvalidateRect(hWnd, NULL, TRUE);
            lastMousePos = currentMousePos;
        }
        break;
    }
    case WM_COMMAND: {
        if (HIWORD(wParam) == BN_CLICKED) {
            switch (LOWORD(wParam)) {
            case IDC_MYBUTTON2:
                file << "F dlheu hf,jnftn" << endl;
                MessageBox(hWnd, L"Другая кнопка!", L"OK", MB_OK);
                break;
            }
        }
        break;
    }
    case WM_LBUTTONUP:
        isDragging = false;
        ReleaseCapture();
        break;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            PostQuitMessage(0);
        }
        break;
    case WM_ERASEBKGND:
        return 1;
    case WM_PAINT: {
        ytro_bobroe = true;
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

#pragma comment(lib, "Comctl32.lib")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    CreateAndRedirectConsole(); // это для консоли и вывода в консоль

    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icc);

    float c1[3] = { 0, 0, 0 };
    float u1[3] = { 5, 0, 0 };
    float v1[3] = { 0, 5, 0 };
    float c2[3] = { 1, 0, 0 };
    float u2[3] = { 0, 3, 0 };
    float v2[3] = { 0, 0, 4 };
    float norm = std::sqrt(n[0] * n[0] + n[1] * n[1]);

    //TLE processing begin
    std::string name = "NOAA 19";
    std::string line1 = "1 33591U 09005A   24062.45833333  .00000061  00000-0  88872-5 0  9993";
    std::string line2 = "2 33591  99.1926 184.7784 0013268 304.8974  55.0816 14.12402469820552";
    libsgp4::Tle tle(name, line1, line2);
    libsgp4::SGP4 sgp4(tle);
    float startTime = 0.0;
    float endTime = 100.0;
    float timeStep = 0.1;
    int i;
    int r_len = (endTime - startTime) / timeStep;
    float** r = new float* [r_len];
    for (double time = startTime; time <= endTime; time += timeStep) {
        libsgp4::Eci eci = sgp4.FindPosition(time);
        i = (time - startTime) / timeStep;
        r[i] = new float[3];
        r[i][0] = eci.Position().x;
        r[i][1] = eci.Position().y;
        r[i][2] = eci.Position().z;
    }
    pc1.set_r3(r, i + 1);
    pc1.projectCloud(R_e, 600, 300, n);
    //TLE processing end

    POINTS* earth = new POINTS[360];
    for (int i = 0; i < 360; i++) {
        earth[i].x = R_e * std::sin((float)i / 180 * pi) + x_c;
        earth[i].y = R_e * std::cos((float)i / 180 * pi) + y_c;
    }

    hInst = hInstance;
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SputnikTracker";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Ошибка регистрации класса окна!", L"Ошибка", MB_ICONERROR);
        return 1;
    }
    HWND hWnd = CreateWindow(
        L"SputnikTracker", L"SPUTNIK Tracker",
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_VISIBLE | WS_CLIPSIBLINGS,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL, hInstance, NULL
    );
    if (!hWnd) {
        MessageBox(NULL, L"Ошибка создания окна!", L"Ошибка", MB_ICONERROR);
        return 1;
    }
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (ytro_bobroe == true) {
            ytro_bobroe = false;
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            RECT rc;
            GetClientRect(hWnd, &rc);
            HDC hdcMem = CreateCompatibleDC(hdc);
            HBITMAP hbm = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
            HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, hbm);
            FillRect(hdcMem, &rc, (HBRUSH)(COLOR_WINDOW + 1));
            RECT mainArea = rc;
            mainArea.bottom -= CONTROL_HEIGHT;
            {
                int savedDC = SaveDC(hdcMem);
                HRGN hrgn = CreateRectRgnIndirect(&mainArea);
                SelectClipRgn(hdcMem, hrgn);
                DeleteObject(hrgn);
                HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
                HPEN hOldPen = (HPEN)SelectObject(hdcMem, hPen);
                MoveToEx(hdcMem, (int)pc1.points[0].x, (int)pc1.points[0].y, NULL);
                for (size_t i = 1; i < pc1.get_length(); i++) {
                    if (pc1.mask[i])
                        LineTo(hdcMem, (int)pc1.points[i].x, (int)pc1.points[i].y);
                    else
                        MoveToEx(hdcMem, (int)pc1.points[i].x, (int)pc1.points[i].y, NULL);
                }
                hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
                SelectObject(hdcMem, hPen);
                MoveToEx(hdcMem, earth[0].x, earth[0].y, NULL);
                for (size_t i = 1; i < 360; i++) {
                    LineTo(hdcMem, earth[i].x, earth[i].y);
                }
                LineTo(hdcMem, earth[0].x, earth[0].y);

                SelectObject(hdcMem, hOldPen);
                DeleteObject(hPen);
                RestoreDC(hdcMem, savedDC);
            }

            BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdcMem, 0, 0, SRCCOPY);

            SelectObject(hdcMem, hOldBmp);
            DeleteObject(hbm);
            DeleteDC(hdcMem);

            EndPaint(hWnd, &ps);
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif */





































/*#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include <windows.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#include "sgp4/SGP4.h"
#include "sgp4/Tle.h"
#include "sgp4/Util.h"
#include "sgp4/Vector.h"
#include "sgp4/Globals.h"

#define pi 3.14159265f
#define NUM_POINTS 100

// Глобальные переменные
std::vector<POINT> points1;
std::vector<POINT> points2;
float n[3] = { 1.0f, 0.0f, -1.0f };
bool isDragging = false;
bool ytro_bobroe = false;
POINT lastMousePos;

class PointsCloud {
public:
    PointsCloud() {
        N = 0;
        r3 = nullptr;
    }
    PointsCloud(int N_) {
        N = N_;
        r3 = new float* [N];
        for (int i = 0; i < N; i++) {
            r3[i][0] = 0;
            r3[i][1] = 0;
            r3[i][2] = 0;
        }
    }
    void projectCloud(float* n) {
        const float norm = std::sqrt(n[0] * n[0] + n[1] * n[1]); //нормировочный коэффициент для вектора базиса локальной системы координат, лежащего в плоскости xy
        float r2[2]; //элемент r3 в локальных координатах плоскости проектирования
        
        points.resize(N);
        for (int i = 0; i < N; i++) {
            r2[0] = 0;
            r2[1] = 0;
            float t = 2 * pi * i / (N - 1); */ //параметр уравнения эллипса
            /*float a1[3]; //a1=c-(c*n)*n
            float a2[3]; //a2=u-(u*n)*n
            float a3[3]; //a3=v-(v*n)*n
            for(int j = 0; j < 3; j++) {
                a1[j] = c[j] - (c[0]*n[0] + c[1]*n[1] + c[2]*n[2]) * n[j];
                a2[j] = u[j] - (u[0]*n[0] + u[1]*n[1] + u[2]*n[2]) * n[j];
                a3[j] = v[j] - (v[0]*n[0] + v[1]*n[1] + v[2]*n[2]) * n[j];
                r3[i][j] = a1[j] + a2[j] * std::cos(t) + a3[j] * std::sin(t);
            }*/ /*
            if (n[0] != 0 || n[1] != 0) {
                r2[0] = (n[1] * r3[i][0] - n[0] * r3[i][1]) / norm;
                r2[1] = r3[i][0] * n[0] * n[2] / std::sqrt(n[0] * n[0] + n[1] * n[1]) + r3[i][1] * n[1] * n[2] / std::sqrt(n[0] * n[0] + n[1] * n[1]) - r3[i][2] * std::sqrt(n[0] * n[0] + n[1] * n[1]);
            }
            else {
                r2[0] = r3[i][0];
                r2[1] = r3[i][1] * -1;
            }
            */
            /*r3[i][0] += 10;
            r3[i][0] *= 20;
            r3[i][1] += 10;
            r3[i][1] *= 20;
            r3[i][2] += 10;
            r3[i][2] *= 20;*/ /*
            r2[0] *= 0.04;
            r2[0] += 600;
            r2[1] *= 0.04;
            r2[1] += 600;
            //points.push_back(std::pair(r2[0], r2[1]));
            //points.emplace_back(r2[0], r2[1]); //может быть тут тоже ЛОНГи должны быть
            points[i].x = (LONG)r2[0];
            points[i].y = (LONG)r2[1];
        }
    }
    void set_r3(float**& r3_, int N_) {
        r3 = r3_;
        N = N_;
    }
    int get_length() {
        return N;
    }
    std::vector<POINT> points;
private:
    float** r3;
    int N;
};
PointsCloud pc1 = PointsCloud();

// Функция проекции эллипса
void ProjectEllipse(float* c, float* u, float* v, float* n, int N, float norm, std::vector<POINT>& points) {
    points.clear();
    points.resize(N);
    float** r3 = new float* [N];
    float** r2 = new float* [N];
    for (int i = 0; i < N; i++) {
        r3[i] = new float[3];
        r2[i] = new float[2];
    }
    for (int i = 0; i < N; i++) {
        float t = 2 * pi * i / (N - 1);
        float a1[3]; //a1=c-(c*n)*n
        float a2[3]; //a2=u-(u*n)*n
        float a3[3]; //a3=v-(v*n)*n
        for (int j = 0; j < 3; j++) {
            a1[j] = c[j] - (c[0] * n[0] + c[1] * n[1] + c[2] * n[2]) * n[j];
            a2[j] = u[j] - (u[0] * n[0] + u[1] * n[1] + u[2] * n[2]) * n[j];
            a3[j] = v[j] - (v[0] * n[0] + v[1] * n[1] + v[2] * n[2]) * n[j];
            r3[i][j] = a1[j] + a2[j] * std::cos(t) + a3[j] * std::sin(t);
        }
        if (n[0] != 0 || n[1] != 0) {
            r2[i][0] = (n[1] * r3[i][0] - n[0] * r3[i][1]) / norm;
            r2[i][1] = r3[i][0] * n[0] * n[2] / std::sqrt(n[0] * n[0] + n[1] * n[1]) + r3[i][1] * n[1] * n[2] / std::sqrt(n[0] * n[0] + n[1] * n[1]) - r3[i][2] * std::sqrt(n[0] * n[0] + n[1] * n[1]);
        }
        else {
            r2[i][0] = r3[i][0];
            r2[i][1] = r3[i][1] * -1;
        }

        r2[i][0] += 15;
        r2[i][0] *= 50;
        r2[i][1] += 10;
        r2[i][1] *= 50;
        //lines[i].position = sf::Vector2f(r2[i][0], r2[i][1]);
        points[i].x = (LONG)r2[i][0];
        points[i].y = (LONG)r2[i][1];
    }
    for (int i = 0; i < N; i++) {
        delete[] r3[i];
        delete[] r2[i];
    }
    delete[] r3;
    delete[] r2;
}
void ResizeWindowTo90Percent(HWND hWnd) {
    RECT workArea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
    int width = (workArea.right - workArea.left) * 0.9; // - вместо 90% можно сделать любое другое кол-во процентов
    int height = (workArea.bottom - workArea.top) * 0.9;
    int x = workArea.left + (workArea.right - workArea.left - width) / 2;
    int y = workArea.top + (workArea.bottom - workArea.top - height) / 2;
    SetWindowPos(
        hWnd,
        NULL,
        x, y,
        width, height,
        SWP_NOZORDER | SWP_FRAMECHANGED
    );
}
// ААААААААААААААААААДВОКАТ ОБРАБОТЧИК
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        ResizeWindowTo90Percent(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_ENTERSIZEMOVE:
        isDragging = true; // Блокируем пересчёт при перемещении окна
        break;
    case WM_LBUTTONDOWN:
        isDragging = true;
        lastMousePos.x = LOWORD(lParam);
        lastMousePos.y = HIWORD(lParam);
        SetCapture(hWnd);
        break;
    case WM_MOUSEMOVE: {
        if (isDragging) {
            POINT currentMousePos;
            currentMousePos.x = LOWORD(lParam);
            currentMousePos.y = HIWORD(lParam);

            // Вычисляем изменение углов
            float dx = lastMousePos.x - currentMousePos.x;
            float dy = lastMousePos.y - currentMousePos.y;

            // Обновляем нормальный вектор
            float theta = acosf(n[2]);
            float phi = atan2f(n[1], n[0]);

            theta -= 0.01f * dy;
            phi += 0.01f * dx;

            n[0] = sinf(theta) * cosf(phi);
            n[1] = sinf(theta) * sinf(phi);
            n[2] = cosf(theta);

            // Нормализуем вектор
            float norm = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
            n[0] /= norm;
            n[1] /= norm;
            n[2] /= norm;

            // Пересчитываем проекции
            float c1[3] = { 0, 0, 0 };
            float u1[3] = { 5, 0, 0 };
            float v1[3] = { 0, 5, 0 };
            float c2[3] = { 1, 0, 0 };
            float u2[3] = { 0, 3, 0 };
            float v2[3] = { 0, 0, 4 };
            float norm2D = sqrtf(n[0] * n[0] + n[1] * n[1]);

            ProjectEllipse(c1, u1, v1, n, NUM_POINTS, norm2D, points1);
            ProjectEllipse(c2, u2, v2, n, NUM_POINTS, norm2D, points2);

            //TLE processing begin
            pc1.projectCloud(n);
            //TLE processing end

            // Перерисовываем окно
            InvalidateRect(hWnd, NULL, TRUE);

            lastMousePos = currentMousePos;
        }
        break;
    }

    case WM_LBUTTONUP:
        isDragging = false;
        ReleaseCapture();
        break;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            PostQuitMessage(0);
        }
        break;
    case WM_ERASEBKGND:
        return 1;
    case WM_PAINT: {
        ytro_bobroe = true;
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Точка входа
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Инициализация данных
    float c1[3] = { 0, 0, 0 };
    float u1[3] = { 5, 0, 0 };
    float v1[3] = { 0, 5, 0 };
    float c2[3] = { 1, 0, 0 };
    float u2[3] = { 0, 3, 0 };
    float v2[3] = { 0, 0, 4 };
    float norm = sqrtf(n[0] * n[0] + n[1] * n[1]);

    //TLE processing begin
    std::string name = "NOAA 19";
    std::string line1 = "1 33591U 09005A   24062.45833333  .00000061  00000-0  88872-5 0  9993";
    std::string line2 = "2 33591  99.1926 184.7784 0013268 304.8974  55.0816 14.12402469820552";
    libsgp4::Tle tle(name, line1, line2);
    libsgp4::SGP4 sgp4(tle);
    double startTime = 0.0;
    double endTime = 100.0;
    double timeStep = 1.0;
    int i;
    int r_len = (endTime - startTime) / timeStep;
    float** r = new float* [r_len];
    for (double time = startTime; time <= endTime; time += timeStep) {
        libsgp4::Eci eci = sgp4.FindPosition(time);
        std::cout << eci.Position() << std::endl;
        i = (time - startTime) / timeStep;
        r[i] = new float[3];
        r[i][0] = eci.Position().x;
        r[i][1] = eci.Position().y;
        r[i][2] = eci.Position().z;
    }
    pc1.set_r3(r, i + 1);
    pc1.projectCloud(n);

    //TLE processing end

    ProjectEllipse(c1, u1, v1, n, NUM_POINTS, norm, points1);
    ProjectEllipse(c2, u2, v2, n, NUM_POINTS, norm, points2);
    // Регистрация класса окна
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SputnikTracker";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Ошибка регистрации класса окна!", L"Ошибка", MB_ICONERROR);
        return 1;
    }
    HWND hWnd = CreateWindow(
        L"SputnikTracker", L"SPUTNIK Tracker",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL, hInstance, NULL
    );
    if (!hWnd) {
        MessageBox(NULL, L"Ошибка создания окна!", L"Ошибка", MB_ICONERROR);
        return 1;
    }
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (ytro_bobroe == true) {
            ytro_bobroe = false;
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            HDC hdc_pamiat = CreateCompatibleDC(hdc); // Это мы создаём "контекст устройства в памяти"
            RECT rc;
            GetClientRect(hWnd, &rc); // создаёт прямоугольник, где можно рисовать
            HBITMAP hbm_rastr = CreateCompatibleBitmap(hdc, rc.right, rc.bottom); //добавляем растровое изоб
            SelectObject(hdc_pamiat, hbm_rastr);

            // Отрисовка в буфер
            FillRect(hdc_pamiat, &rc, (HBRUSH)(COLOR_WINDOW + 1));

            HPEN hRedPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
            HPEN hOldPen = (HPEN)SelectObject(hdc_pamiat, hRedPen);

            if (!points1.empty() and !points2.empty()) {
                MoveToEx(hdc_pamiat, points1[0].x, points1[0].y, NULL);
                for (size_t i = 1; i < points1.size(); i++) {
                    LineTo(hdc_pamiat, points1[i].x, points1[i].y);
                }
                LineTo(hdc_pamiat, points1[0].x, points1[0].y);

                MoveToEx(hdc_pamiat, points2[0].x, points2[0].y, NULL);
                for (size_t i = 1; i < points2.size(); i++) {
                    LineTo(hdc_pamiat, points2[i].x, points2[i].y);
                }
                LineTo(hdc_pamiat, points2[0].x, points2[0].y);
            }
            BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdc_pamiat, 0, 0, SRCCOPY); //это мы из буфера копируем на экран

            DeleteObject(hbm_rastr); //Функция DeleteObject удаляет логическое перо, кисть, шрифт, 
            //растровое изображение, область или палитру, освобождая все системные ресурсы, связанные с объектом.
            //После удаления объекта указанный дескриптор больше не действует. - из Microsoft

            DeleteDC(hdc_pamiat); // Функция DeleteDC удаляет указанный контекст устройства (DC).
            DeleteObject(hRedPen); // ручку удаялем

            EndPaint(hWnd, &ps);
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif */
/*#include <windows.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
#define pi 3.14159265f
#define NUM_POINTS 100

std::vector<POINT> points1;
std::vector <POINT> points2;
float n[3] = { 1.0f, 0.0f, -1.0f };
bool isDragging = false;
bool ggg = false;
bool ytro_dobroe = false;
POINT lastMousePos;

void ResizeWindowTo90Percent(HWND hWnd) { // - РАЗМЕР окна, прямоугольника (пока нет перерисовки эллипсов, это начальное окно)
    RECT workArea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
    int width = (workArea.right - workArea.left) * 0.9; // - вместо 90% можно сделать любое другое кол-во процентов
    int height = (workArea.bottom - workArea.top) * 0.9;
    int x = workArea.left + (workArea.right - workArea.left - width) / 2;
    int y = workArea.top + (workArea.bottom - workArea.top - height) / 2;
    SetWindowPos(
        hWnd,
        NULL,
        x, y,
        width, height,
        SWP_NOZORDER | SWP_FRAMECHANGED
    );
}

void ProjectEllipse(float* c, float* u, float* v, float* n, int N, float norm, std::vector<POINT>& points) {
    points.clear();
    points.resize(N);
    float** r3 = new float* [N];
    float** r2 = new float* [N];
    for (int i = 0; i < N; i++) {
        r3[i] = new float[3];
        r2[i] = new float[2];
    }
    for (int i = 0; i < N; i++) {
        float t = 2 * pi * i / (N - 1);
        float a1[3]; //a1=c-(c*n)*n
        float a2[3]; //a2=u-(u*n)*n
        float a3[3]; //a3=v-(v*n)*n
        for (int j = 0; j < 3; j++) {
            a1[j] = c[j] - (c[0] * n[0] + c[1] * n[1] + c[2] * n[2]) * n[j];
            a2[j] = u[j] - (u[0] * n[0] + u[1] * n[1] + u[2] * n[2]) * n[j];
            a3[j] = v[j] - (v[0] * n[0] + v[1] * n[1] + v[2] * n[2]) * n[j];
            r3[i][j] = a1[j] + a2[j] * std::cos(t) + a3[j] * std::sin(t);
        }
        if (n[0] != 0 || n[1] != 0) {
            r2[i][0] = (n[1] * r3[i][0] - n[0] * r3[i][1]) / norm;
            r2[i][1] = r3[i][0] * n[0] * n[2] / std::sqrt(n[0] * n[0] + n[1] * n[1]) + r3[i][1] * n[1] * n[2] / std::sqrt(n[0] * n[0] + n[1] * n[1]) - r3[i][2] * std::sqrt(n[0] * n[0] + n[1] * n[1]);
        }
        else {
            r2[i][0] = r3[i][0];
            r2[i][1] = r3[i][1] * -1;
        }

        r2[i][0] += 12;
        r2[i][0] *= 50;
        r2[i][1] += 7;
        r2[i][1] *= 50;
        points[i].x = (LONG)r2[i][0];
        points[i].y = (LONG)r2[i][1];
    }
    for (int i = 0; i < N; i++) {
        delete[] r3[i];
        delete[] r2[i];
    }
    delete[] r3;
    delete[] r2;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        ResizeWindowTo90Percent(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_ENTERSIZEMOVE:
        isDragging = true; // Блокируем пересчёт при перемещении окна
        break;
    case WM_LBUTTONDOWN:
        isDragging = true;
        lastMousePos.x = LOWORD(lParam);
        lastMousePos.y = HIWORD(lParam);
        SetCapture(hWnd);
        break;
    case WM_MOUSEMOVE:
        if (isDragging) {
            POINT currentMousePos;
            currentMousePos.x = LOWORD(lParam);
            currentMousePos.y = HIWORD(lParam);
            float dx = lastMousePos.x - currentMousePos.x;
            float dy = lastMousePos.y - currentMousePos.y;
            float theta = acosf(n[2]);
            float phi = atan2f(n[1], n[0]);
            theta -= 0.01f * dy;
            phi += 0.01f * dx;
            n[0] = sinf(theta) * cosf(phi);
            n[1] = sinf(theta) * sinf(phi);
            n[2] = cosf(theta);
            float norm = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
            n[0] /= norm;
            n[1] /= norm;
            n[2] /= norm;
            float c1[3] = { 0, 0, 0 };
            float u1[3] = { 5, 0, 0 };
            float v1[3] = { 0, 5, 0 };
            float c2[3] = { 1, 0, 0 };
            float u2[3] = { 0, 3, 0 };
            float v2[3] = { 0, 0, 4 };
            float norm2D = sqrtf(n[0] * n[0] + n[1] * n[1]);

            ProjectEllipse(c1, u1, v1, n, NUM_POINTS, norm2D, points1);
            ProjectEllipse(c2, u2, v2, n, NUM_POINTS, norm2D, points2);

            InvalidateRect(hWnd, NULL, TRUE); // po syti pererisovka okna
            lastMousePos = currentMousePos;
        }
        break;

    case WM_LBUTTONUP:
        isDragging = false;
        ReleaseCapture();
        break;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            PostQuitMessage(0);
        }
        break;
    case WM_ERASEBKGND:
        return 1;
    case WM_PAINT: {
        ggg = true;
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    //ofstream file("Proverka.txt", ios::out);
    float c1[3] = { 0, 0, 0 };
    float u1[3] = { 5, 0, 0 };
    float v1[3] = { 0, 5, 0 };
    float c2[3] = { 1, 0, 0 };
    float u2[3] = { 0, 3, 0 };
    float v2[3] = { 0, 0, 4 };
    float norm = sqrtf(n[0] * n[0] + n[1] * n[1]);

    ProjectEllipse(c1, u1, v1, n, NUM_POINTS, norm, points1);
    ProjectEllipse(c2, u2, v2, n, NUM_POINTS, norm, points2);

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SputnikTracker";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Ошибка регистрации класса окна!", L"Ошибка", MB_ICONERROR);
        return 1;
    }

    HWND hWnd = CreateWindow(
        L"SputnikTracker", L"SPUTNIK Tracker",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL, hInstance, NULL
    );

    if (!hWnd) {
        MessageBox(NULL, L"Ошибка создания окна!", L"Ошибка", MB_ICONERROR);
        return 1;
    }
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (ggg == true) {
            ggg = false;
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            HDC hdc_pamiat = CreateCompatibleDC(hdc); // Это мы создаём "контекст устройства в памяти"
            RECT rc;
            GetClientRect(hWnd, &rc); // создаёт прямоугольник, где можно рисовать
            HBITMAP hbm_rastr = CreateCompatibleBitmap(hdc, rc.right, rc.bottom); //добавляем растровое изоб
            SelectObject(hdc_pamiat, hbm_rastr);

            // Отрисовка в буфер
            FillRect(hdc_pamiat, &rc, (HBRUSH)(COLOR_WINDOW + 1));

            HPEN hRedPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
            HPEN hOldPen = (HPEN)SelectObject(hdc_pamiat, hRedPen);

            if (!points1.empty() and !points2.empty()) {
                MoveToEx(hdc_pamiat, points1[0].x, points1[0].y, NULL);
                for (size_t i = 1; i < points1.size(); i++) {
                    LineTo(hdc_pamiat, points1[i].x, points1[i].y);
                }
                LineTo(hdc_pamiat, points1[0].x, points1[0].y);

                MoveToEx(hdc_pamiat, points2[0].x, points2[0].y, NULL);
                for (size_t i = 1; i < points2.size(); i++) {
                    LineTo(hdc_pamiat, points2[i].x, points2[i].y);
                }
                LineTo(hdc_pamiat, points2[0].x, points2[0].y);
            }

            BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdc_pamiat, 0, 0, SRCCOPY);

            DeleteObject(hbm_rastr); //Функция DeleteObject удаляет логическое перо, кисть, шрифт, 
            //растровое изображение, область или палитру, освобождая все системные ресурсы, связанные с объектом.
            //После удаления объекта указанный дескриптор больше не действует.

            DeleteDC(hdc_pamiat); // Функция DeleteDC удаляет указанный контекст устройства (DC).
            DeleteObject(hRedPen); // ручку удаялем

            EndPaint(hWnd, &ps);
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}*/


/*#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include <windows.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

#include "sgp4/SGP4.h"
#include "sgp4/Tle.h"
#include "sgp4/Util.h"
#include "sgp4/Vector.h"
#include "sgp4/Globals.h"

#define pi 3.14159265f
#define NUM_POINTS 100

// Глобальные переменные
std::vector<POINT> points1;
std::vector<POINT> points2;
float n[3] = { 1.0f, 0.0f, -1.0f };
bool isDragging = false;
bool ytro_bobroe = false;
POINT lastMousePos;

class PointsCloud {
public:
    PointsCloud() {
        N = 0;
        r3 = nullptr;
    }
    PointsCloud(int N_) {
        N = N_;
        r3 = new float* [N];
        for (int i = 0; i < N; i++) {
            r3[i][0] = 0;
            r3[i][1] = 0;
            r3[i][2] = 0;
        }
    }
    void projectCloud(float* n) {
        const float norm = std::sqrt(n[0] * n[0] + n[1] * n[1]); //нормировочный коэффициент для вектора базиса локальной системы координат, лежащего в плоскости xy
        float r2[2]; //элемент r3 в локальных координатах плоскости проектирования
        points.clear();
        for (int i = 0; i < N; i++) {
            r2[0] = 0;
            r2[1] = 0;
            float t = 2 * pi * i / (N - 1); //параметр уравнения эллипса
            /*float a1[3]; //a1=c-(c*n)*n
            float a2[3]; //a2=u-(u*n)*n
            float a3[3]; //a3=v-(v*n)*n
            for(int j = 0; j < 3; j++) {
                a1[j] = c[j] - (c[0]*n[0] + c[1]*n[1] + c[2]*n[2]) * n[j];
                a2[j] = u[j] - (u[0]*n[0] + u[1]*n[1] + u[2]*n[2]) * n[j];
                a3[j] = v[j] - (v[0]*n[0] + v[1]*n[1] + v[2]*n[2]) * n[j];
                r3[i][j] = a1[j] + a2[j] * std::cos(t) + a3[j] * std::sin(t);
            }*//*
            if (n[0] != 0 || n[1] != 0) {
                r2[0] = (n[1] * r3[i][0] - n[0] * r3[i][1]) / norm;
                r2[1] = r3[i][0] * n[0] * n[2] / std::sqrt(n[0] * n[0] + n[1] * n[1]) + r3[i][1] * n[1] * n[2] / std::sqrt(n[0] * n[0] + n[1] * n[1]) - r3[i][2] * std::sqrt(n[0] * n[0] + n[1] * n[1]);
            }
            else {
                r2[0] = r3[i][0];
                r2[1] = r3[i][1] * -1;
            }*/
            /*r3[i][0] += 10;
            r3[i][0] *= 20;
            r3[i][1] += 10;
            r3[i][1] *= 20;
            r3[i][2] += 10;
            r3[i][2] *= 20;*//*
            r2[0] *= 0.04;
            r2[0] += 600;
            r2[1] *= 0.04;
            r2[1] += 600;
            //points.push_back(std::pair(r2[0], r2[1]));
            //points.emplace_back(r2[0], r2[1]); //может быть тут тоже ЛОНГи должны быть
            points.emplace_back((LONG)r2[0], (LONG)r2[1]);
        }
    }
    void set_r3(float**& r3_, int N_) {
        r3 = r3_;
        N = N_;
    }
    int get_length() {
        return N;
    }
    std::vector<POINT> points;
private:
    float** r3;
    int N;
};
PointsCloud pc1 = PointsCloud();

// Функция проекции эллипса
void ProjectEllipse(float* c, float* u, float* v, float* n, int N, float norm, std::vector<POINT>& points) {
    points.clear();
    points.resize(N);
    float** r3 = new float* [N];
    float** r2 = new float* [N];
    for (int i = 0; i < N; i++) {
        r3[i] = new float[3];
        r2[i] = new float[2];
    }
    for (int i = 0; i < N; i++) {
        float t = 2 * pi * i / (N - 1);
        float a1[3]; //a1=c-(c*n)*n
        float a2[3]; //a2=u-(u*n)*n
        float a3[3]; //a3=v-(v*n)*n
        for (int j = 0; j < 3; j++) {
            a1[j] = c[j] - (c[0] * n[0] + c[1] * n[1] + c[2] * n[2]) * n[j];
            a2[j] = u[j] - (u[0] * n[0] + u[1] * n[1] + u[2] * n[2]) * n[j];
            a3[j] = v[j] - (v[0] * n[0] + v[1] * n[1] + v[2] * n[2]) * n[j];
            r3[i][j] = a1[j] + a2[j] * std::cos(t) + a3[j] * std::sin(t);
        }
        if (n[0] != 0 || n[1] != 0) {
            r2[i][0] = (n[1] * r3[i][0] - n[0] * r3[i][1]) / norm;
            r2[i][1] = r3[i][0] * n[0] * n[2] / std::sqrt(n[0] * n[0] + n[1] * n[1]) + r3[i][1] * n[1] * n[2] / std::sqrt(n[0] * n[0] + n[1] * n[1]) - r3[i][2] * std::sqrt(n[0] * n[0] + n[1] * n[1]);
        }
        else {
            r2[i][0] = r3[i][0];
            r2[i][1] = r3[i][1] * -1;
        }

        r2[i][0] += 15;
        r2[i][0] *= 50;
        r2[i][1] += 10;
        r2[i][1] *= 50;
        //lines[i].position = sf::Vector2f(r2[i][0], r2[i][1]);
        points[i].x = (LONG)r2[i][0];
        points[i].y = (LONG)r2[i][1];
    }
    for (int i = 0; i < N; i++) {
        delete[] r3[i];
        delete[] r2[i];
    }
    delete[] r3;
    delete[] r2;
}
void ResizeWindowTo90Percent(HWND hWnd) {
    RECT workArea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
    int width = (workArea.right - workArea.left) * 0.9; // - вместо 90% можно сделать любое другое кол-во процентов
    int height = (workArea.bottom - workArea.top) * 0.9;
    int x = workArea.left + (workArea.right - workArea.left - width) / 2;
    int y = workArea.top + (workArea.bottom - workArea.top - height) / 2;
    SetWindowPos(
        hWnd,
        NULL,
        x, y,
        width, height,
        SWP_NOZORDER | SWP_FRAMECHANGED
    );
}
// ААААААААААААААААААДВОКАТ ОБРАБОТЧИК
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        ResizeWindowTo90Percent(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_ENTERSIZEMOVE:
        isDragging = true; // Блокируем пересчёт при перемещении окна
        break;
    case WM_LBUTTONDOWN:
        isDragging = true;
        lastMousePos.x = LOWORD(lParam);
        lastMousePos.y = HIWORD(lParam);
        SetCapture(hWnd);
        break;
    case WM_MOUSEMOVE: {
        if (isDragging) {
            POINT currentMousePos;
            currentMousePos.x = LOWORD(lParam);
            currentMousePos.y = HIWORD(lParam);

            // Вычисляем изменение углов
            float dx = lastMousePos.x - currentMousePos.x;
            float dy = lastMousePos.y - currentMousePos.y;

            // Обновляем нормальный вектор
            float theta = acosf(n[2]);
            float phi = atan2f(n[1], n[0]);

            theta -= 0.01f * dy;
            phi += 0.01f * dx;

            n[0] = sinf(theta) * cosf(phi);
            n[1] = sinf(theta) * sinf(phi);
            n[2] = cosf(theta);

            // Нормализуем вектор
            float norm = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
            n[0] /= norm;
            n[1] /= norm;
            n[2] /= norm;

            // Пересчитываем проекции
            float c1[3] = { 0, 0, 0 };
            float u1[3] = { 5, 0, 0 };
            float v1[3] = { 0, 5, 0 };
            float c2[3] = { 1, 0, 0 };
            float u2[3] = { 0, 3, 0 };
            float v2[3] = { 0, 0, 4 };
            float norm2D = sqrtf(n[0] * n[0] + n[1] * n[1]);

            ProjectEllipse(c1, u1, v1, n, NUM_POINTS, norm2D, points1);
            ProjectEllipse(c2, u2, v2, n, NUM_POINTS, norm2D, points2);

            //TLE processing begin
            pc1.projectCloud(n);
            //TLE processing end

            // Перерисовываем окно
            InvalidateRect(hWnd, NULL, TRUE);

            lastMousePos = currentMousePos;
        }
        break;
    }

    case WM_LBUTTONUP:
        isDragging = false;
        ReleaseCapture();
        break;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            PostQuitMessage(0);
        }
        break;
    case WM_ERASEBKGND:
        return 1;
    case WM_PAINT: {
        ytro_bobroe = true;
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Точка входа
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Инициализация данных
    float c1[3] = { 0, 0, 0 };
    float u1[3] = { 5, 0, 0 };
    float v1[3] = { 0, 5, 0 };
    float c2[3] = { 1, 0, 0 };
    float u2[3] = { 0, 3, 0 };
    float v2[3] = { 0, 0, 4 };
    float norm = sqrtf(n[0] * n[0] + n[1] * n[1]);

    //TLE processing begin
    std::string name = "NOAA 19";
    std::string line1 = "1 33591U 09005A   24062.45833333  .00000061  00000-0  88872-5 0  9993";
    std::string line2 = "2 33591  99.1926 184.7784 0013268 304.8974  55.0816 14.12402469820552";
    libsgp4::Tle tle(name, line1, line2);
    libsgp4::SGP4 sgp4(tle);
    double startTime = 0.0;
    double endTime = 100.0;
    double timeStep = 1.0;
    int i;
    int r_len = (endTime - startTime) / timeStep;
    float** r = new float* [r_len];
    for (double time = startTime; time <= endTime; time += timeStep) {
        libsgp4::Eci eci = sgp4.FindPosition(time);
        std::cout << eci.Position() << std::endl;
        i = (time - startTime) / timeStep;
        r[i] = new float[3];
        r[i][0] = eci.Position().x;
        r[i][1] = eci.Position().y;
        r[i][2] = eci.Position().z;
    }
    pc1.set_r3(r, i + 1);
    pc1.projectCloud(n);

    //TLE processing end

    ProjectEllipse(c1, u1, v1, n, NUM_POINTS, norm, points1);
    ProjectEllipse(c2, u2, v2, n, NUM_POINTS, norm, points2);
    // Регистрация класса окна
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SputnikTracker";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Ошибка регистрации класса окна!", L"Ошибка", MB_ICONERROR);
        return 1;
    }
    HWND hWnd = CreateWindow(
        L"SputnikTracker", L"SPUTNIK Tracker",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL, hInstance, NULL
    );
    if (!hWnd) {
        MessageBox(NULL, L"Ошибка создания окна!", L"Ошибка", MB_ICONERROR);
        return 1;
    }
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (ytro_bobroe == true) {
            ytro_bobroe = false;
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            HDC hdc_pamiat = CreateCompatibleDC(hdc); // Это мы создаём "контекст устройства в памяти"
            RECT rc;
            GetClientRect(hWnd, &rc); // создаёт прямоугольник, где можно рисовать
            HBITMAP hbm_rastr = CreateCompatibleBitmap(hdc, rc.right, rc.bottom); //добавляем растровое изоб
            SelectObject(hdc_pamiat, hbm_rastr);

            // Отрисовка в буфер
            FillRect(hdc_pamiat, &rc, (HBRUSH)(COLOR_WINDOW + 1));

            HPEN hRedPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
            HPEN hOldPen = (HPEN)SelectObject(hdc_pamiat, hRedPen);

            if (!points1.empty() and !points2.empty()) {
                MoveToEx(hdc_pamiat, points1[0].x, points1[0].y, NULL);
                for (size_t i = 1; i < points1.size(); i++) {
                    LineTo(hdc_pamiat, points1[i].x, points1[i].y);
                }
                LineTo(hdc_pamiat, points1[0].x, points1[0].y);

                MoveToEx(hdc_pamiat, points2[0].x, points2[0].y, NULL);
                for (size_t i = 1; i < points2.size(); i++) {
                    LineTo(hdc_pamiat, points2[i].x, points2[i].y);
                }
                LineTo(hdc_pamiat, points2[0].x, points2[0].y);
            }
            BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdc_pamiat, 0, 0, SRCCOPY); //это мы из буфера копируем на экран

            DeleteObject(hbm_rastr); //Функция DeleteObject удаляет логическое перо, кисть, шрифт, 
            //растровое изображение, область или палитру, освобождая все системные ресурсы, связанные с объектом.
            //После удаления объекта указанный дескриптор больше не действует. - из Microsoft

            DeleteDC(hdc_pamiat); // Функция DeleteDC удаляет указанный контекст устройства (DC).
            DeleteObject(hRedPen); // ручку удаялем

            EndPaint(hWnd, &ps);
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif*/

/*
#include <windows.h>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
#define pi 3.14159265f
#define NUM_POINTS 100

std::vector<POINT> points1;
std::vector <POINT> points2;
float n[3] = { 1.0f, 0.0f, -1.0f };
bool isDragging = false;
bool ggg = false;
bool ytro_dobroe = false;
POINT lastMousePos;

void ResizeWindowTo90Percent(HWND hWnd) {
    RECT workArea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
    int width = (workArea.right - workArea.left) * 0.9; // - вместо 90% можно сделать любое другое кол-во процентов
    int height = (workArea.bottom - workArea.top) * 0.9;
    int x = workArea.left + (workArea.right - workArea.left - width) / 2;
    int y = workArea.top + (workArea.bottom - workArea.top - height) / 2;
    SetWindowPos(
        hWnd,
        NULL,           
        x, y,          
        width, height,
        SWP_NOZORDER | SWP_FRAMECHANGED
    );
}

void ProjectEllipse(float* c, float* u, float* v, float* n, int N, float norm, std::vector<POINT>& points) {
    points.clear();
    points.resize(N);
    float** r3 = new float* [N];
    float** r2 = new float* [N];
    for (int i = 0; i < N; i++) {
        r3[i] = new float[3];
        r2[i] = new float[2];
    }
    for (int i = 0; i < N; i++) {
        float t = 2 * pi * i / (N - 1);
        float a1[3]; //a1=c-(c*n)*n
        float a2[3]; //a2=u-(u*n)*n
        float a3[3]; //a3=v-(v*n)*n
        for (int j = 0; j < 3; j++) {
            a1[j] = c[j] - (c[0] * n[0] + c[1] * n[1] + c[2] * n[2]) * n[j];
            a2[j] = u[j] - (u[0] * n[0] + u[1] * n[1] + u[2] * n[2]) * n[j];
            a3[j] = v[j] - (v[0] * n[0] + v[1] * n[1] + v[2] * n[2]) * n[j];
            r3[i][j] = a1[j] + a2[j] * std::cos(t) + a3[j] * std::sin(t);
        }
        if (n[0] != 0 || n[1] != 0) {
            r2[i][0] = (n[1] * r3[i][0] - n[0] * r3[i][1]) / norm;
            r2[i][1] = r3[i][0] * n[0] * n[2] / std::sqrt(n[0] * n[0] + n[1] * n[1]) + r3[i][1] * n[1] * n[2] / std::sqrt(n[0] * n[0] + n[1] * n[1]) - r3[i][2] * std::sqrt(n[0] * n[0] + n[1] * n[1]);
        }
        else {
            r2[i][0] = r3[i][0];
            r2[i][1] = r3[i][1] * -1;
        }

        r2[i][0] += 12;
        r2[i][0] *= 50;
        r2[i][1] += 7;
        r2[i][1] *= 50;
        points[i].x = (LONG)r2[i][0];
        points[i].y = (LONG)r2[i][1];
    }
    for (int i = 0; i < N; i++) {
        delete[] r3[i];
        delete[] r2[i];
    }
    delete[] r3;
    delete[] r2;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        ResizeWindowTo90Percent(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_ENTERSIZEMOVE:
        isDragging = true; // Блокируем пересчёт при перемещении окна
        break;
    case WM_LBUTTONDOWN:
        isDragging = true;
        lastMousePos.x = LOWORD(lParam);
        lastMousePos.y = HIWORD(lParam);
        SetCapture(hWnd);
        break;
    case WM_MOUSEMOVE:
        if (isDragging) {
            POINT currentMousePos;
            currentMousePos.x = LOWORD(lParam);
            currentMousePos.y = HIWORD(lParam);
            float dx = lastMousePos.x - currentMousePos.x;
            float dy = lastMousePos.y - currentMousePos.y;
            float theta = acosf(n[2]);
            float phi = atan2f(n[1], n[0]);
            theta -= 0.01f * dy;
            phi += 0.01f * dx;
            n[0] = sinf(theta) * cosf(phi);
            n[1] = sinf(theta) * sinf(phi);
            n[2] = cosf(theta);
            float norm = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
            n[0] /= norm;
            n[1] /= norm;
            n[2] /= norm;
            float c1[3] = { 0, 0, 0 };
            float u1[3] = { 5, 0, 0 };
            float v1[3] = { 0, 5, 0 };
            float c2[3] = { 1, 0, 0 };
            float u2[3] = { 0, 3, 0 };
            float v2[3] = { 0, 0, 4 };
            float norm2D = sqrtf(n[0] * n[0] + n[1] * n[1]);

            ProjectEllipse(c1, u1, v1, n, NUM_POINTS, norm2D, points1);
            ProjectEllipse(c2, u2, v2, n, NUM_POINTS, norm2D, points2);

            InvalidateRect(hWnd, NULL, TRUE); // po syti pererisovka okna
            lastMousePos = currentMousePos;
        }
        break;

    case WM_LBUTTONUP:
        isDragging = false;
        ReleaseCapture();
        break;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            PostQuitMessage(0);
        }
        break;
    case WM_ERASEBKGND:
        return 1;
    case WM_PAINT: {
        ggg = true;
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    //ofstream file("Proverka.txt", ios::out);
    float c1[3] = { 0, 0, 0 };
    float u1[3] = { 5, 0, 0 };
    float v1[3] = { 0, 5, 0 };
    float c2[3] = { 1, 0, 0 };
    float u2[3] = { 0, 3, 0 };
    float v2[3] = { 0, 0, 4 };
    float norm = sqrtf(n[0] * n[0] + n[1] * n[1]);

    ProjectEllipse(c1, u1, v1, n, NUM_POINTS, norm, points1);
    ProjectEllipse(c2, u2, v2, n, NUM_POINTS, norm, points2);

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SputnikTracker";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Ошибка регистрации класса окна!", L"Ошибка", MB_ICONERROR);
        return 1;
    }

    HWND hWnd = CreateWindow(
        L"SputnikTracker", L"SPUTNIK Tracker",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL, hInstance, NULL
    );

    if (!hWnd) {
        MessageBox(NULL, L"Ошибка создания окна!", L"Ошибка", MB_ICONERROR);
        return 1;
    }
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (ggg == true) {
            ggg = false;
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            HDC hdc_pamiat = CreateCompatibleDC(hdc); // Это мы создаём "контекст устройства в памяти"
            RECT rc;
            GetClientRect(hWnd, &rc); // создаёт прямоугольник, где можно рисовать
            HBITMAP hbm_rastr = CreateCompatibleBitmap(hdc, rc.right, rc.bottom); //добавляем растровое изоб
            SelectObject(hdc_pamiat, hbm_rastr);

            // Отрисовка в буфер
            FillRect(hdc_pamiat, &rc, (HBRUSH)(COLOR_WINDOW + 1));

            HPEN hRedPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
            HPEN hOldPen = (HPEN)SelectObject(hdc_pamiat, hRedPen);

            if (!points1.empty() and !points2.empty()) {
                MoveToEx(hdc_pamiat, points1[0].x, points1[0].y, NULL);
                for (size_t i = 1; i < points1.size(); i++) {
                    LineTo(hdc_pamiat, points1[i].x, points1[i].y);
                }
                LineTo(hdc_pamiat, points1[0].x, points1[0].y);

                MoveToEx(hdc_pamiat, points2[0].x, points2[0].y, NULL);
                for (size_t i = 1; i < points2.size(); i++) {
                    LineTo(hdc_pamiat, points2[i].x, points2[i].y);
                }
                LineTo(hdc_pamiat, points2[0].x, points2[0].y);
            }

            BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdc_pamiat, 0, 0, SRCCOPY);

            DeleteObject(hbm_rastr); //Функция DeleteObject удаляет логическое перо, кисть, шрифт, 
            //растровое изображение, область или палитру, освобождая все системные ресурсы, связанные с объектом.
            //После удаления объекта указанный дескриптор больше не действует.

            DeleteDC(hdc_pamiat); // Функция DeleteDC удаляет указанный контекст устройства (DC).
            DeleteObject(hRedPen); // ручку удаялем

            EndPaint(hWnd, &ps);
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
*/








/*#include <windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <cmath>

#define pi 3.14159265
#define IDC_MYBUTTON 100 
#define IDC_EDITBOX 101
std::vector<std::pair<float, float>> points1; // Массив точек для отрисовки
std::vector<std::pair<float, float>> points2;

float n[3] = { 1.0f, 0.0f, -1.0f };
bool isDragging = false;
POINT lastMousePos;
// Глобальные переменные для хранения данных

HBRUSH hRedBrush;

void projectEllipse(float* c, float* u, float* v, float* n, const int N, float norm, std::vector<std::pair<float, float>>& points) {
    float** r3 = new float* [N];
    float** r2 = new float* [N];
    for (int i = 0; i < N; i++) {
        r3[i] = new float[3];
        r2[i] = new float[2];
    }
    for (int i = 0; i < N; i++) {
        float t = 2 * pi * i / (N - 1);
        float a1[3]; //a1=c-(c*n)*n
        float a2[3]; //a2=u-(u*n)*n
        float a3[3]; //a3=v-(v*n)*n
        for (int j = 0; j < 3; j++) {
            a1[j] = c[j] - (c[0] * n[0] + c[1] * n[1] + c[2] * n[2]) * n[j];
            a2[j] = u[j] - (u[0] * n[0] + u[1] * n[1] + u[2] * n[2]) * n[j];
            a3[j] = v[j] - (v[0] * n[0] + v[1] * n[1] + v[2] * n[2]) * n[j];
            r3[i][j] = a1[j] + a2[j] * std::cos(t) + a3[j] * std::sin(t);
        }
        if (n[0] != 0 || n[1] != 0) {
            r2[i][0] = (n[1] * r3[i][0] - n[0] * r3[i][1]) / norm;
            r2[i][1] = r3[i][0] * n[0] * n[2] / std::sqrt(n[0] * n[0] + n[1] * n[1]) + r3[i][1] * n[1] * n[2] / std::sqrt(n[0] * n[0] + n[1] * n[1]) - r3[i][2] * std::sqrt(n[0] * n[0] + n[1] * n[1]);
        }
        else {
            r2[i][0] = r3[i][0];
            r2[i][1] = r3[i][1] * -1;
        }


        r2[i][0] += 15;
        r2[i][0] *= 50;
        r2[i][1] += 10;
        r2[i][1] *= 50;
        points.push_back({ (r2[i][0]),(r2[i][1]) });
        //std::vector<POINT>.push_back({r2[i][0],r2[i][1]});
        //points->push_back({ static_cast<LONG>(r2[i][0]), static_cast<LONG>(r2[i][1]) });
       // points[i].position = sf::Vector2f(r2[i][0], r2[i][1]);
    }
    for (int i = 0; i < N; i++) {
        delete[] r3[i];
        delete[] r2[i];
    }
    delete[] r3;
    delete[] r2;
}
// , std::vector<std::pair<float, float>>& points1, std::vector<std::pair<float, float>>& points2
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));  // Красное перо, толщина 2
    //HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    float c1[3] = { 0, 0, 0 };
    float u1[3] = { 5, 0, 0 };
    float v1[3] = { 0, 5, 0 };
    float c2[3] = { 1, 0, 0 };
    float u2[3] = { 0, 3, 0 };
    float v2[3] = { 0, 0, 4 };
    float n[3] = { 1, 0, -1 };
    float norm = std::sqrt(n[0] * n[0] + n[1] * n[1]);
    const int N = 100;

    projectEllipse(c1, u1, v1, n, N, norm, points1);
    projectEllipse(c2, u2, v2, n, N, norm, points2);
    hRedBrush = CreateSolidBrush(RGB(255, 150, 2));

    WNDCLASS wc = { 0 };
    wc.style = CS_HREDRAW | CS_VREDRAW; // Добавляем перерисовку при изменении размера
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SputnikTracker";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Ошибка регистрации класса окна!", L"Ошибка", MB_ICONERROR);
        return 1;
    }

    // Создание окна
    HWND hWnd = CreateWindow(
        L"SputnikTracker", L"SPUTNIK Tracker",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL, hInstance, NULL
    );

    if (!hWnd) {
        MessageBox(NULL, L"Ошибка создания окна!", L"Ошибка", MB_ICONERROR);
        return 1;
    }

    // Полноэкранный режим
    ShowWindow(hWnd, SW_MAXIMIZE);

    // Цикл сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;*/
    /*
    WNDCLASS wc = { 0 };
    wc.style = CS_HREDRAW | CS_VREDRAW; // Добавляем перерисовку при изменении размера
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MyWindowClass";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Стандартный фон окна
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    

    HWND hWnd = CreateWindow(
        L"MyWindowClass", L"Окно с графиком",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        900, 300,
        NULL, NULL, hInstance, NULL
    );

    if (!hWnd) {
        MessageBox(NULL, L"Ошибка создания окна!", L"Ошибка", MB_OK);
        return 0;
    }

    // Создаём элементы управления
    CreateWindow(
        L"BUTTON", L"Обновить точки",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 10, 150, 30,
        hWnd, (HMENU)IDC_MYBUTTON, hInstance, NULL
    );

    CreateWindow(
        L"EDIT", L"50",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_NUMBER,
        170, 10, 50, 30,
        hWnd, (HMENU)IDC_EDITBOX, hInstance, NULL
    );

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DeleteObject(hRedBrush);
    return (int)msg.wParam;*//*
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Очищаем экран (аналог window.clear())
        RECT rect;
        GetClientRect(hWnd, &rect);
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

        // Рисуем lines1 (красным)
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

        if (!points1.empty()) {
            MoveToEx(hdc, (int)points1[0].first, (int)points1[0].second, NULL);
            for (size_t i = 1; i < points1.size(); ++i) {
                LineTo(hdc, (int)points1[i].first, (int)points1[i].second);
                MoveToEx(hdc, (int)points1[i].first, (int)points1[i].second, NULL);
            }
        }

        // Рисуем lines2 (синим)
        DeleteObject(hPen);
        hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
        SelectObject(hdc, hPen);

        if (!points2.empty()) {
            MoveToEx(hdc, (int)points2[0].first, (int)points2[0].second, NULL);
            for (size_t i = 1; i < points2.size(); ++i) {
                LineTo(hdc, (int)points2[i].first, (int)points2[i].second);
                MoveToEx(hdc, (int)points2[i].first, (int)points2[i].second, NULL);
            }
        }

        // Восстанавливаем старое перо и освобождаем ресурсы
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);

        EndPaint(hWnd, &ps);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;  // Добавлен return 0 для всех case, кроме default
}

*/























/*#include <windows.h>
#include <vector>
#include <cmath>
#include <string>

#define PI 3.14159265f
#define NUM_POINTS 100

// Глобальные переменные
std::vector<POINT> points1;
std::vector<POINT> points2;
float n[3] = { 1.0f, 0.0f, -1.0f };
bool isDragging = false;
POINT lastMousePos;

// Функция проекции эллипса
void ProjectEllipse(float* c, float* u, float* v, float* n, int N, float norm, std::vector<POINT>& points) {
    points.clear();
    points.resize(N);

    for (int i = 0; i < N; i++) {
        float t = 2 * PI * i / (N - 1);

        // Вычисляем базисные векторы
        float a1[3], a2[3], a3[3];
        float cn = c[0] * n[0] + c[1] * n[1] + c[2] * n[2];
        float un = u[0] * n[0] + u[1] * n[1] + u[2] * n[2];
        float vn = v[0] * n[0] + v[1] * n[1] + v[2] * n[2];

        for (int j = 0; j < 3; j++) {
            a1[j] = c[j] - cn * n[j];
            a2[j] = u[j] - un * n[j];
            a3[j] = v[j] - vn * n[j];
        }

        // Вычисляем точку на эллипсе
        float r3[3];
        for (int j = 0; j < 3; j++) {
            r3[j] = a1[j] + a2[j] * cosf(t) + a3[j] * sinf(t);
        }

        // Проецируем на 2D плоскость
        float x, y;
        if (n[0] != 0 || n[1] != 0) {
            x = (n[1] * r3[0] - n[0] * r3[1]) / norm;
            y = (r3[0] * n[0] * n[2] + r3[1] * n[1] * n[2]) / norm - r3[2] * norm;
        }
        else {
            x = r3[0];
            y = -r3[1];
        }

        // Масштабируем и центрируем
        x = (x + 15) * 50;
        y = (y + 10) * 50;

        points[i].x = (LONG)x;
        points[i].y = (LONG)y;
    }
}

// Функция обработки сообщений
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Очищаем экран
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        FillRect(hdc, &clientRect, (HBRUSH)(COLOR_WINDOW + 1));

        // Рисуем первый эллипс (красный)
        HPEN hRedPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hRedPen);

        if (!points1.empty()) {
            MoveToEx(hdc, points1[0].x, points1[0].y, NULL);
            for (size_t i = 1; i < points1.size(); i++) {
                LineTo(hdc, points1[i].x, points1[i].y);
            }
            // Замыкаем фигуру
            LineTo(hdc, points1[0].x, points1[0].y);
        }

        // Рисуем второй эллипс (синий)
        HPEN hBluePen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
        SelectObject(hdc, hBluePen);

        if (!points2.empty()) {
            MoveToEx(hdc, points2[0].x, points2[0].y, NULL);
            for (size_t i = 1; i < points2.size(); i++) {
                LineTo(hdc, points2[i].x, points2[i].y);
            }
            // Замыкаем фигуру
            LineTo(hdc, points2[0].x, points2[0].y);
        }

        // Восстанавливаем оригинальное перо
        SelectObject(hdc, hOldPen);
        DeleteObject(hRedPen);
        DeleteObject(hBluePen);

        EndPaint(hWnd, &ps);
        break;
    }

    case WM_LBUTTONDOWN: {
        isDragging = true;
        lastMousePos.x = LOWORD(lParam);
        lastMousePos.y = HIWORD(lParam);
        SetCapture(hWnd);
        break;
    }

    case WM_MOUSEMOVE: {
        if (isDragging) {
            POINT currentMousePos;
            currentMousePos.x = LOWORD(lParam);
            currentMousePos.y = HIWORD(lParam);

            // Вычисляем изменение углов
            float dx = lastMousePos.x - currentMousePos.x;
            float dy = lastMousePos.y - currentMousePos.y;

            // Обновляем нормальный вектор
            float theta = acosf(n[2]);
            float phi = atan2f(n[1], n[0]);

            theta -= 0.01f * dy;
            phi += 0.01f * dx;

            n[0] = sinf(theta) * cosf(phi);
            n[1] = sinf(theta) * sinf(phi);
            n[2] = cosf(theta);

            // Нормализуем вектор
            float norm = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
            n[0] /= norm;
            n[1] /= norm;
            n[2] /= norm;

            // Пересчитываем проекции
            float c1[3] = { 0, 0, 0 };
            float u1[3] = { 5, 0, 0 };
            float v1[3] = { 0, 5, 0 };
            float c2[3] = { 1, 0, 0 };
            float u2[3] = { 0, 3, 0 };
            float v2[3] = { 0, 0, 4 };
            float norm2D = sqrtf(n[0] * n[0] + n[1] * n[1]);

            ProjectEllipse(c1, u1, v1, n, NUM_POINTS, norm2D, points1);
            ProjectEllipse(c2, u2, v2, n, NUM_POINTS, norm2D, points2);

            // Перерисовываем окно
            InvalidateRect(hWnd, NULL, TRUE);

            lastMousePos = currentMousePos;
        }
        break;
    }

    case WM_LBUTTONUP: {
        isDragging = false;
        ReleaseCapture();
        break;
    }

    case WM_KEYDOWN: {
        if (wParam == VK_ESCAPE) {
            PostQuitMessage(0);
        }
        break;
    }

    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Точка входа
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Инициализация данных
    float c1[3] = { 0, 0, 0 };
    float u1[3] = { 5, 0, 0 };
    float v1[3] = { 0, 5, 0 };
    float c2[3] = { 1, 0, 0 };
    float u2[3] = { 0, 3, 0 };
    float v2[3] = { 0, 0, 4 };
    float norm = sqrtf(n[0] * n[0] + n[1] * n[1]);

    ProjectEllipse(c1, u1, v1, n, NUM_POINTS, norm, points1);
    ProjectEllipse(c2, u2, v2, n, NUM_POINTS, norm, points2);

    // Регистрация класса окна
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SputnikTracker";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Ошибка регистрации класса окна!", L"Ошибка", MB_ICONERROR);
        return 1;
    }

    // Создание окна
    HWND hWnd = CreateWindow(
        L"SputnikTracker", L"SPUTNIK Tracker",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL, hInstance, NULL
    );

    if (!hWnd) {
        MessageBox(NULL, L"Ошибка создания окна!", L"Ошибка", MB_ICONERROR);
        return 1;
    }

    // Полноэкранный режим
    ShowWindow(hWnd, SW_MAXIMIZE);

    // Цикл сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}*/
/*LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Очищаем экран (аналог window.clear())
        RECT rect;
        GetClientRect(hWnd, &rect);
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

        // Рисуем lines1 (красным)
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

        MoveToEx(hdc, (int)points1[0].first, (int)points1[0].second, NULL);
         for (size_t i = 1; i < points1.size(); ++i) {
            LineTo(hdc, (int)points1[i].first, (int)points1[i].second);
            MoveToEx(hdc, (int)points1[i].first, (int)points1[i].second, NULL);
        }

        DeleteObject(hPen);
        hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
        SelectObject(hdc, hPen);

        MoveToEx(hdc, (int)points2[0].first, (int)points2[0].second, NULL);
        for (size_t i = 1; i < points2.size(); ++i) {
            LineTo(hdc, (int)points2[i].first, (int)points2[i].second);
            MoveToEx(hdc, (int)points2[i].first, (int)points2[i].second, NULL);
        }

        // Восстанавливаем старое перо и освобождаем ресурсы
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);

        EndPaint(hWnd, &ps);
        break;
    }
        

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
        
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    
    return 0;
}*/

    /*
        case WM_COMMAND:
            if (LOWORD(wParam) == IDC_MYBUTTON) {
                // Обновляем массив точек
                wchar_t buffer[256];
                GetDlgItemText(hWnd, IDC_EDITBOX, buffer, 256);
                int count = _wtoi(buffer);
                if (count <= 0) count = 50;

                points.clear();
                for (int i = 0; i < count; i++) {
                    points.push_back({ rand() % 800, rand() % 200 });
                }

                // Перерисовываем окно
                InvalidateRect(hWnd, NULL, TRUE);
            }
            break;

        case WM_TIMER:
            // Анимация - двигаем точки
            for (auto& pt : points) {
                pt.x = (pt.x + 5) % 800;
                pt.y = (pt.y + rand() % 5 - 2) % 200;
            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;}*/
// 7. Функция обработки сообщений 
/*
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0); // Завершаем приложение при закрытии
            return 0;
        
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam); // Стандартная обработка
    }
}
*/

/*#include <windows.h>

// Идентификатор кнопки
#define IDC_MYBUTTON 100

// Прототип функции обработки сообщений
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        // Отрисовка содержимого окна
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        TextOut(hdc, 10, 10, L"Hello, World!", 13);
        EndPaint(hWnd, &ps);
        return 0;
    }
    case WM_CLOSE:
        DestroyWindow(hWnd); // Закрыть окно
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0); // Завершить приложение
        return 0;
    default:
        // Передать необработанные сообщения стандартной процедуре
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 1. Регистрация класса окна
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MyWindowClass";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    // 2. Создание главного окна
    HWND hWnd = CreateWindow(
        L"MyWindowClass", L"Окно с кнопкой",
        WS_OVERLAPPEDWINDOW,
        100, 100, 300, 200,
        NULL, NULL, hInstance, NULL
    );

    // 3. Создание кнопки
    CreateWindow(
        L"BUTTON", L"Нажми меня",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        100, 50, 100, 30,
        hWnd, (HMENU)IDC_MYBUTTON, hInstance, NULL
    );

    // 4. Показ окна
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // 5. Цикл сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}*/

// 6. Обработчик сообщений (с вашей WndProc + обработка
/*
#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
*/
/*LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    //std::cout << "Don't die";

    switch (message) {
    case WM_CREATE:
        ResizeWindowTo90Percent(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_ENTERSIZEMOVE:
        isDragging = true; // Блокируем пересчёт при перемещении окна
        break;
    case WM_LBUTTONDOWN:
        isDragging = true;
        lastMousePos.x = LOWORD(lParam);
        lastMousePos.y = HIWORD(lParam);
        SetCapture(hWnd);
        break;
    case WM_MOUSEMOVE:
        if (isDragging) {
            POINT currentMousePos;
            currentMousePos.x = LOWORD(lParam);
            currentMousePos.y = HIWORD(lParam);
            float dx = lastMousePos.x - currentMousePos.x;
            float dy = lastMousePos.y - currentMousePos.y;
            float theta = acosf(n[2]);
            float phi = atan2f(n[1], n[0]);
            theta -= 0.01f * dy;
            phi += 0.01f * dx;
            n[0] = sinf(theta) * cosf(phi);
            n[1] = sinf(theta) * sinf(phi);
            n[2] = cosf(theta);
            float norm = sqrtf(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
            n[0] /= norm;
            n[1] /= norm;
            n[2] /= norm;
            float c1[3] = { 0, 0, 0 };
            float u1[3] = { 5, 0, 0 };
            float v1[3] = { 0, 5, 0 };
            float c2[3] = { 1, 0, 0 };
            float u2[3] = { 0, 3, 0 };
            float v2[3] = { 0, 0, 4 };
            float norm2D = sqrtf(n[0] * n[0] + n[1] * n[1]);

            ProjectEllipse(c1, u1, v1, n, NUM_POINTS, norm2D, points1);
            ProjectEllipse(c2, u2, v2, n, NUM_POINTS, norm2D, points2);

            InvalidateRect(hWnd, NULL, TRUE); // po syti pererisovka okna
            lastMousePos = currentMousePos;
        }
        break;

    case WM_LBUTTONUP:
        isDragging = false;
        ReleaseCapture();
        break;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            PostQuitMessage(0);
        }
        break;
    case WM_PAINT:
        ggg = true;
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}*/