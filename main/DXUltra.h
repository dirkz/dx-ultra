#pragma once

#include "stdafx.h"

#include "WindowCallback.h"

namespace dxultra
{

struct DXUltra : WindowCallback
{
    void OnInit(HWND hwnd, UINT width, UINT height) override;
    void OnUpdate() override;
    void OnRender() override;
};

} // namespace dxultra
