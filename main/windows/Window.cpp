#include "stdafx.h"

#include "Window.h"

namespace dxultra
{

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE)
    {
        LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        WindowCallback *pCallback =
            reinterpret_cast<WindowCallback *>(pCreateStruct->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCallback));
        return 0;
    }

    WindowCallback *pCallback =
        reinterpret_cast<WindowCallback *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    if (pCallback)
    {
        switch (uMsg)
        {
        case WM_DESTROY:
            pCallback->OnDestroy();
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
            pCallback->OnUpdate();
            pCallback->OnRender();
            return 0;

        case WM_SIZE: {
            WORD width = LOWORD(lParam);
            WORD height = HIWORD(lParam);
            pCallback->OnResize(width, height);
        }
            return 0;

        case WM_GETMINMAXINFO: {
            LPMINMAXINFO pMinMaxInfo = (LPMINMAXINFO)lParam;
            pMinMaxInfo->ptMinTrackSize = MinimumWindowsDimensions;
        }
            return 0;

        case WM_ACTIVATE: {
            WORD action = LOWORD(wParam);
            std::wstring msg = L"*** WM_ACTIVATE with action " + std::to_wstring(action) + L"\n";
            OutputDebugString(msg.c_str());
            bool isBeingActivated = action != WA_INACTIVE;
            pCallback->Activate(isBeingActivated);
        }
            return 0;
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Window::Window(WindowCallback *callback) : m_callback{callback}
{
}

int Window::Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd)
{
    int screenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

    int screenWidthQuarter = screenWidth / 4;
    int screenHeightHalve = screenHeight / 2;

    int left = screenWidthQuarter;
    int right = 3 * screenWidthQuarter;
    int top = 0;
    int width = right - left;
    int height = screenHeightHalve;

    const wchar_t CLASS_NAME[] = L"DX-Ultra";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"DX-Ultra", WS_OVERLAPPEDWINDOW, left, top, width,
                               height, nullptr, nullptr, hInstance, m_callback);

    if (hwnd == NULL)
    {
        return 0;
    }

    m_callback->OnInit(hwnd, width, height);

    ShowWindow(hwnd, nShowCmd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<char>(msg.wParam);
}

} // namespace dxultra
