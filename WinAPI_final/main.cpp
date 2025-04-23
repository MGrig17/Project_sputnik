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



#include <stdio.h> // это мне для консольки
#include <io.h>
#include <fcntl.h>
#include <commctrl.h>

#include "Simulation.h"
#include "Callbacks.h"

#define NUM_POINTS 100
#define R_earth 6400 //радиус Земли в км
#define x_c 600
#define y_c 300



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
float timeStep = -1;
HWND StepStep;
//Текст на экране
HWND hTextLine1;
HWND hTextLine2;
HWND hTextLine3;
HWND hTextTimeStep;
HWND hTextColor;
HWND hButton;
const int CONTROL_HEIGHT = 100; // высота дочернего окна

// и ничего вы мне не сделаете
std::string name;
std::string line1;
std::string line2;
libsgp4::DateTime timeStart;
libsgp4::DateTime timeEnd;
float n[3] = { 1.0f, 0.0f, -1.0f };
bool isDragging = false;
bool ytro_bobroe = false;
POINT lastMousePos;
//координаты центра окна
float k_zoom = 0.04; //масштабный коэффициент, число пикселей на км

LONG R_e = R_earth * k_zoom; //радиус Земли в пикселях

/*std::string WideToUTF8_1(const wchar_t* wideStr) {
    int size = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, nullptr, 0, nullptr, nullptr);
    std::string utf8Str(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, &utf8Str[0], size, nullptr, nullptr);
    return utf8Str;
}*/




Simulation sim(k_zoom);



#pragma comment(lib, "Comctl32.lib")
// Точка входа
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    CreateAndRedirectConsole(); // это для консоли и вывода в консоль

    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icc);

    int yearStart;
    std::string monthStart;
    int dshiftStart;
    int dayStart;
    int yearEnd;
    std::string monthEnd;
    int dshiftEnd;
    int dayEnd;

    std::cout << "Awaiting for input: year the simulation begins" << std::endl;
    std::cin >> yearStart;
    std::cout << "Awaiting for input: month the simulation begins" << std::endl;
    std::cin >> monthStart;
    dshiftStart = 1;
    if(monthStart == "February")
        dshiftStart = 2;
    if(monthStart == "March")
        dshiftStart = 3;
    if(monthStart == "April")
        dshiftStart = 4;
    if(monthStart == "May")
        dshiftStart = 5;
    if(monthStart == "June")
        dshiftStart = 6;
    if(monthStart == "July")
        dshiftStart = 7;
    if(monthStart == "August")
        dshiftStart = 8;
    if(monthStart == "September")
        dshiftStart = 9;
    if(monthStart == "October")
        dshiftStart = 10;
    if(monthStart == "November")
        dshiftStart = 11;
    if(monthStart == "December")
        dshiftStart = 12;
    if(dshiftStart == 0)
        std::cout << "Month name is not recognized; set value to default 'January'" << std::endl;
    std::cout << "Awaiting for input: day the simulation begins" << std::endl;
    std::cin >> dayStart;
    timeStart = libsgp4::DateTime(yearStart, dshiftStart, dayStart);
    std::cout << "Awaiting for input: year the simulation ends" << std::endl;
    std::cin >> yearEnd;
    std::cout << "Awaiting for input: month the simulation ends" << std::endl;
    std::cin >> monthEnd;
    dshiftEnd = 1;
    if(monthEnd == "February")
        dshiftEnd = 2;
    if(monthEnd == "March")
        dshiftEnd = 3;
    if(monthEnd == "April")
        dshiftEnd = 4;
    if(monthEnd == "May")
        dshiftEnd = 5;
    if(monthEnd == "June")
        dshiftEnd = 6;
    if(monthEnd == "July")
        dshiftEnd = 7;
    if(monthEnd == "August")
        dshiftEnd = 8;
    if(monthEnd == "September")
        dshiftEnd = 9;
    if(monthEnd == "October")
        dshiftEnd = 10;
    if(monthEnd == "November")
        dshiftEnd = 11;
    if(monthEnd == "December")
        dshiftEnd = 12;
    if(dshiftEnd == 0)
        std::cout << "Month name is not recognized; set value to default 'January'" << std::endl;
    std::cout << "Awaiting for input: day the simulation ends" << std::endl;
    std::cin >> dayEnd;
    timeEnd = libsgp4::DateTime(yearEnd, dshiftEnd, dayEnd);

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
        //MessageBox(NULL, L"Ошибка регистрации класса окна!", L"Ошибка", MB_ICONERROR);
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
        //MessageBox(NULL, L"Ошибка создания окна!", L"Ошибка", MB_ICONERROR);
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
