/**
  * Terminology:
  * "Physical point" -- a point located in 3-dimensional virtual space, which coordinates are stored in km units
  * "Graphical point" -- a point located in 2-dimensional space which corresponds to graphical window. GP coordinates are stored in px units (variables of integer-like type)
  */
#pragma once

#include <string>
#include "supplementary2.h"
#include <fstream>
#include <iostream>
#include <filesystem>

/// WinAPI widget IDs:
#define IDC_MYBUTTON 101            /// 'Create satellite' button
#define IDC_Name_satellite 102      /// 'Name' text field
#define IDC_EDITBOX1 103            /// TLE line 1 text field
#define IDC_EDITBOX2 104            /// TLE line 2 text field
#define IDC_TIMESTEP 105            /// 'Timestep' text field
#define IDC_COLOR1 106              /// Red color component counter
#define IDC_COLOR2 107              /// Green color component counter
#define IDC_COLOR3 108              /// Blue color component counter
#define IDC_SEARCH_BUTTON 110       /// 'Import TLE from database by name'
#define ID_PANEL 200                /// The bottom panel itself

/// WinAPI widgets as objects:
extern HINSTANCE hInst;
extern HWND hPanel;                 /// The bottom panel itself
extern HWND knopka1;                /// 'Create satellite' button
extern HWND hEdit3;                 /// 'Name' text field
extern HWND hEdit1;                 /// TLE line 1 text field
extern HWND hEdit2;                 /// TLE line 2 text field
extern HWND hEditColor1;            /// Red color component counter
extern HWND hEditColor2;            /// Green color component counter
extern HWND hEditColor3;            /// Blue color component counter
extern HWND hUpDown1;               /// Arrows bound to the red color counter
extern HWND hUpDown2;               /// Arrows bound to the green color counter
extern HWND hUpDown3;               /// Arrows bound to the blue color counter
extern HWND hButton;                /// 'Import TLE from database by name'
extern float timeStep;              /// Secondary buffer for the following text field
extern HWND StepStep;               /// 'Timestep' text field (look 'Simulation.h' for further explanation)

/// Text labels meant to describe input fields:
extern HWND hTextLine1;             /// To 'hEdit1'
extern HWND hTextLine2;             /// To 'hEdit2'
extern HWND hTextTimeStep;          /// To 'StepStep'
extern HWND hTextColor;             /// To 'hEditColor1..3'
extern const int CONTROL_HEIGHT;    /// Bottom panel height

extern Simulation sim;              /// Graphics space contents

// и ничего вы мне не сделаете
extern std::string name;            /// Secondary buffer for 'hEdit3'
extern std::string line1;           /// Secondary buffer for 'hEdit1'
extern std::string line2;           /// Secondary buffer for 'hEdit2'
extern float n[3];                  /// Vector normal to the monitor plane
extern bool isDragging;             /// Whether left mouse button is pressed
bool gotTLE = false;                /// Whether TLE is stored in buffers
extern POINT lastMousePos;
const float dk_zoom = 0.00005;      /// Change of 'k_zoom' bound to mousewheel being scrolled on 1 unit
int k1 = 875; // x-позици€ первого
int k2 = 80; // ширина €чейки
int k3 = 30; // высота €чейки комбо-бокс дл€ цвета
int color_R=255, color_G=0, color_B=0;

namespace fs = std::filesystem;

std::string find_file_by_prefix(const std::string& directory_path, char prefix_char) {
    std::string found_file;
    for (const auto& entry : fs::directory_iterator(directory_path)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            if (!filename.empty() && (toupper(filename[0]) == toupper(prefix_char))) {
                if (!found_file.empty()) {
                    throw std::runtime_error("Found more than one file with mentioned prefix!");
                }
                found_file = filename;
            }
        }
    }
    /*
    if (found_file.empty()) {
        throw std::runtime_error("‘айл с указанным префиксом не найден!");
    }
    */
    return found_file;
}
LRESULT CALLBACK PanelProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    switch (message)
    {
    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED)
        {
            if (LOWORD(wParam) == IDC_SEARCH_BUTTON) {
                wchar_t buffer[256];
                GetDlgItemTextW(hWnd, IDC_Name_satellite, buffer, 256);
                std::string text1 = WideToUTF8(buffer);
                char str = text1[0];

                std::string search_directory = "H:\\Documents\\MIPT\\C++\\WinAPI\\Base_satellites\\";
                std::string files=find_file_by_prefix(search_directory, str);
                files = search_directory + files;
                std::cout << files << std::endl;
                //std::ifstream file("C:/Users/User/OneDrive/Desktop/Base_satellite/F.txt");
                std::ifstream file(files);
                if (file.good())
                {
                    string sLine;
                    getline(file, sLine);
                    int t = sLine.length();
                    while (sLine[t-1] == ' ') {
                        t -= 1;
                    }
                    sLine = sLine.substr(0, t);
                    while (sLine != text1) {
                        getline(file, sLine);
                        t = sLine.length();
                        while (sLine[t - 1] == ' ') {
                            t -= 1;
                        }
                        sLine = sLine.substr(0, t);
                    }
                    std::cout << sLine << std::endl; //ЌашЄл с таким названием
                    //string nextLine1, nextLine2;
                    getline(file, line1);
                    getline(file, line2);
                    gotTLE = true;
                    /*if (getline(file, nextLine1)) {
                        t = nextLine1.length();
                        while (t > 0 && nextLine1[t - 1] == ' ') t--;
                        nextLine1 = nextLine1.substr(0, t);

                        std::cout << "—ледующа€ строка 1: " << nextLine1 << std::endl;

                        if (getline(file, nextLine2)) {
                            t = nextLine2.length();
                            while (t > 0 && nextLine2[t - 1] == ' ') t--;
                            nextLine2 = nextLine2.substr(0, t);

                            std::cout << "—ледующа€ строка 2: " << nextLine2 << std::endl;
                        }
                    }*/
                }
                return TRUE;
            }
            if (LOWORD(wParam) == IDC_MYBUTTON)
            {
                wchar_t buffer1[256];
                GetDlgItemTextW(hWnd, IDC_EDITBOX1, buffer1, 256);
                std::string text1 = WideToUTF8(buffer1);
                wchar_t buffer2[256];
                GetDlgItemTextW(hWnd, IDC_EDITBOX2, buffer2, 256);
                std::string text2 = WideToUTF8(buffer2);
                wchar_t buffer3[256];
                GetDlgItemTextW(hWnd, IDC_Name_satellite, buffer3, 256);
                std::string name_satellite = WideToUTF8(buffer3);

                wchar_t buffer4[256]; // TimeStep
                GetDlgItemTextW(hWnd, IDC_TIMESTEP, buffer4, 256);
                name = name_satellite;
                if(!gotTLE) {
                    line1 = text1;
                    line2 = text2;
                }
                gotTLE = true;
                //line1 = "1 19596U 88095A   25110.87941604 -.00000352  00000-0  00000-0 0  9999";
                //line2 = "2 19596  12.0770 332.9726 0005784 179.7204 140.0282  1.00319827117417";

                GetDlgItemTextW(hWnd, IDC_COLOR1, buffer1, 256);
                GetDlgItemTextW(hWnd, IDC_COLOR2, buffer2, 256);
                GetDlgItemTextW(hWnd, IDC_COLOR3, buffer3, 256);
                color_R = atoi((WideToUTF8(buffer1)).c_str());
                color_G = atoi((WideToUTF8(buffer2)).c_str());
                color_B = atoi((WideToUTF8(buffer3)).c_str());
                if (color_R > 255) color_R = 255; if (color_R < 0) color_R = 0;
                if (color_G > 255) color_G = 255; if (color_G < 0) color_G = 0;
                if (color_B > 255) color_B = 255;if (color_B < 0) color_B = 0;

                // The clear
                SetDlgItemTextW(hWnd, IDC_EDITBOX1, L"");
                SetDlgItemTextW(hWnd, IDC_EDITBOX2, L"");
                SetDlgItemTextW(hWnd, IDC_COLOR1, L"");
                SetDlgItemTextW(hWnd, IDC_COLOR2, L"");
                SetDlgItemTextW(hWnd, IDC_COLOR3, L"");
                SetDlgItemTextW(hWnd, IDC_Name_satellite, L"");
                SetDlgItemTextW(hWnd, IDC_TIMESTEP, L"");

                float startTime = 0.0;
                float endTime = 100.0;
                timeStep = atof((WideToUTF8(buffer4)).c_str());

                sim.add_orbit(name, line1, line2, timeStep, color_R, color_G, color_B);
                sim.updateGraph3D(n);
                //InvalidateRect(hWnd, NULL, FALSE);
                gotTLE = false;

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
        hTextLine1 = CreateWindowW(L"STATIC", L"ENTER TLE Line 1",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            12, 20,
            200, 15,
            hPanel,
            NULL, hInst, NULL);
        hTextLine2 = CreateWindowW(L"STATIC", L"ENTER TLE Line 2",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            12, 55,
            200, 15,
            hPanel,
            NULL, hInst, NULL);
        hTextTimeStep = CreateWindowW(L"STATIC", L"STEP",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            1000, 15,
            50, 15,
            hPanel,
            NULL, hInst, NULL);
        hTextColor = CreateWindowW(L"STATIC", L"COLOR",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            820, 55,
            50, 15,
            hPanel,
            NULL, hInst, NULL);

        SetWindowSubclass(hPanel, PanelProc, 0, 0);
        knopka1 = CreateWindow(
            L"BUTTON", L"Create the satellite",
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
        hButton = CreateWindow(L"BUTTON", L"Search",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            800, 15, k2, k3, hPanel, (HMENU)IDC_SEARCH_BUTTON, NULL, NULL);

        hEditColor1 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
            k1, 50, k2, k3, hPanel, (HMENU)IDC_COLOR1, NULL, NULL);
        /*hEditColor1 = CreateWindow(
            L"EDIT", L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            k1, 50, k2, k3,
            hPanel, (HMENU)IDC_COLOR1, hInst, NULL
        );*/
        hUpDown1 = CreateWindow(
            UPDOWN_CLASS, NULL,
            WS_VISIBLE | WS_CHILD | UDS_ALIGNRIGHT | UDS_SETBUDDYINT | UDS_ARROWKEYS,
            0, 0, 0, 0, hPanel, NULL, hInst, NULL
        );
        SendMessage(hUpDown1, UDM_SETBUDDY, (WPARAM)hEditColor1, 0);
        SendMessage(hUpDown1, UDM_SETRANGE, 0, MAKELPARAM(255, 0));
        hEditColor2 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
            k1 + k2 + 5, 50, k2, k3,
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
        hEditColor3 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
            k1 + k2 + 5, 50, k2, k3,
            hPanel, (HMENU)IDC_COLOR3, hInst, NULL
        );
        hUpDown3 = CreateWindow(
            UPDOWN_CLASS, NULL,
            WS_VISIBLE | WS_CHILD | UDS_ALIGNRIGHT | UDS_SETBUDDYINT | UDS_ARROWKEYS,
            0, 0, 0, 0, hPanel, NULL, hInst, NULL
        );
        SendMessage(hUpDown3, UDM_SETBUDDY, (WPARAM)hEditColor3, 0);
        SendMessage(hUpDown3, UDM_SETRANGE, 0, MAKELPARAM(255, 0));


        // TLE
        hEdit3 = CreateWindow(
            L"EDIT", L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            490, 15, 300, 30,
            hPanel, (HMENU)IDC_Name_satellite, hInst, NULL
        );
        hEdit1 = CreateWindow(
            L"EDIT", L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            180, 15, 300, 30,
            hPanel, (HMENU)IDC_EDITBOX1, hInst, NULL
        );
        hEdit2 = CreateWindow(
            L"EDIT", L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
            180, 50, 300, 30,
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
    case WM_MOUSEWHEEL:
        sim.scroll(dk_zoom * GET_WHEEL_DELTA_WPARAM(wParam), n);
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_MOUSEMOVE: {
        POINT currentMousePos;
        currentMousePos.x = LOWORD(lParam);
        currentMousePos.y = HIWORD(lParam);
        if (isDragging) {
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
            lastMousePos = currentMousePos;
        }
        sim.highlight(currentMousePos);
        InvalidateRect(hWnd, NULL, FALSE);
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
