#pragma once

#include "stdafx.h"

#include "WindowCallback.h"

namespace dxultra
{

struct Window
{
    Window(WindowCallback *callback) : m_callback{callback} {};

    int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd);

  private:
    WindowCallback *m_callback;
};

} // namespace dxultra
