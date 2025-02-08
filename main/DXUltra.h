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

  private:
    ComPtr<ID3D12Device> m_device;
    ComPtr<ID3D12CommandQueue> m_commandQueue;
};

} // namespace dxultra
