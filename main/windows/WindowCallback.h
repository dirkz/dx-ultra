#pragma once

#include "stdafx.h"

namespace dxultra
{

constexpr UINT MinimumWindowsWidth = 300;
constexpr UINT MinimumWindowsHeight = 300;

struct WindowCallback
{
    virtual void OnInit(HWND hwnd, UINT width, UINT height) = 0;
    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;
    virtual void OnResize(UINT width, UINT height) = 0;
    virtual void OnDestroy() = 0;
};

} // namespace dxultra
