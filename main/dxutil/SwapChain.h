#pragma once

#include "stdafx.h"

#include "DescriptorHeap.h"

namespace dxultra
{

constexpr DXGI_FORMAT SwapChainFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

struct SwapChain
{
    SwapChain(IDXGIFactory4 *pFactory, ID3D12Device *pDevice, ID3D12CommandQueue *pCommandQueue,
              UINT frameCount, HWND hwnd, UINT width, UINT height);

    UINT GetCurrentBackBufferIndex();

  private:
    ComPtr<IDXGISwapChain3> m_swapChain;
    DescriptorHeap m_descriptorHeap;
};

} // namespace dxultra
