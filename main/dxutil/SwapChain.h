#pragma once

#include "stdafx.h"

#include "DescriptorHeap.h"

namespace dxultra
{

constexpr DXGI_FORMAT SwapChainFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

struct SwapChain
{
    SwapChain(IDXGIFactory4 *pFactory, ComPtr<ID3D12Device> device,
              ID3D12CommandQueue *pCommandQueue, UINT frameCount, HWND hwnd, UINT width,
              UINT height);

    void CreateRenderTargets();

    UINT GetCurrentBackBufferIndex();

  private:
    UINT m_numFrames;
    ComPtr<ID3D12Device> m_device;
    ComPtr<IDXGISwapChain3> m_swapChain;
    DescriptorHeap m_descriptorHeap;
    std::vector<ComPtr<ID3D12Resource>> m_renderTargets;
};

} // namespace dxultra
