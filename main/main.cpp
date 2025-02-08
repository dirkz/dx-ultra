#include "stdafx.h"

#include "DXUltra.h"
#include "Window.h"

static int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                           _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
    OutputDebugString(L"Hello, world!\n");

    {
        using namespace dxultra;
        DXUltra dxultra{};
        Window window{&dxultra};
        window.WinMain(hInstance, hPrevInstance, pCmdLine, nCmdShow);
    }
}