#pragma once

#include "stdafx.h"

namespace dxultra
{

constexpr DXGI_FORMAT SwapChainFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

struct SwapChain
{
    SwapChain(IDXGIFactory1 *pFactory, ID3D12CommandQueue *pCommandQueue, UINT frameCount,
              HWND hwnd, UINT width, UINT height);

    UINT GetCurrentBackBufferIndex();

  private:
    ComPtr<IDXGISwapChain3> m_swapChain;
};

} // namespace dxultra
