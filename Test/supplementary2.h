/**
  * Terminology:
  * "Physical point" -- a point located in 3-dimensional virtual space, which coordinates are stored in km units
  * "Graphical point" -- a point located in 2-dimensional space which corresponds to graphical window. GP coordinates are stored in px units (variables of integer-like type)
  */
#pragma once

#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

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

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif
