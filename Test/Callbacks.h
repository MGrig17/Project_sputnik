/**
  * Terminology:
  * "Physical point" -- a point located in 3-dimensional virtual space, which coordinates are stored in km units
  * "Graphical point" -- a point located in 2-dimensional space which corresponds to graphical window. GP coordinates are stored in px units (variables of integer-like type)
  */
#pragma once

#include <string>
#include "supplementary2.h"

#define IDC_MYBUTTON 101 // которая Создать спутник
#define IDC_Name_satellite 102
#define IDC_EDITBOX1 103
#define IDC_EDITBOX2 104
#define IDC_TIMESTEP 105
#define IDC_COLOR1 106
#define IDC_COLOR2 107
#define IDC_COLOR3 108
#define ID_PANEL 200

extern HINSTANCE hInst;
extern HWND hPanel;

extern HWND knopka1; // кнопка "Создать спутник"
extern HWND hEdit3; // для ввода имени спутника
extern HWND hEdit1; // тле1
extern HWND hEdit2;// тле2
extern HWND hEditColor1; // выбор цвета
extern HWND hEditColor2;
extern HWND hEditColor3;
extern HWND hUpDown1; //это для стрелочек
extern HWND hUpDown2;
extern HWND hUpDown3;
extern float timeStep;
extern HWND StepStep;
//Текст на экране
extern HWND hTextLine1;
extern HWND hTextLine2;
extern HWND hTextTimeStep;
extern HWND hTextColor;
extern const int CONTROL_HEIGHT; // высота дочернего окна
extern Simulation sim;

// и ничего вы мне не сделаете
extern std::string name;
extern std::string line1;
extern std::string line2;
extern float n[3];
extern bool isDragging;
extern POINT lastMousePos;
const float dk_zoom = 0.00005; //изменение масштабного коэффициента при прокрутке колёсика мыши на 1 у. е.
int k1 = 875; // x-позиция первого
int k2 = 80; // ширина ячейки
int k3 = 30; // высота ячейки комбо-бокс для цвета
int color_R=255, color_G=0, color_B=0;

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
                //line1 = text1;
                //line2 = text2;
                line1 = "1 19596U 88095A   25110.87941604 -.00000352  00000-0  00000-0 0  9999";
                line2 = "2 19596  12.0770 332.9726 0005784 179.7204 140.0282  1.00319827117417";
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
                timeStep = atof((WideToUTF8(buffer4)).c_str()); //было 0.1

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
            phi -= 0.01f * dx;
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
