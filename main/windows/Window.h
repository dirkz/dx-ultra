#pragma once

#include "stdafx.h"

#include "WindowCallback.h"

namespace dxultra
{

struct Window
{
    Window(WindowCallback *callback);

    int Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd);

  private:
    WindowCallback *m_callback;
};

} // namespace dxultra
