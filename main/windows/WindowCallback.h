#pragma once

#include "stdafx.h"

namespace dxultra
{

struct WindowCallback
{
    virtual void OnInit(HWND hwnd, UINT width, UINT height) = 0;
    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;
    virtual void OnResize(UINT width, UINT height) = 0;
    virtual void OnDestroy() = 0;
};

} // namespace dxultra
