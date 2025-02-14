#pragma once

#include "stdafx.h"

#include "Constants.h"
#include "DescriptorHeap.h"

namespace dxultra
{

constexpr DXGI_FORMAT SwapChainFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
constexpr DXGI_FORMAT DepthStencilFormat = DXGI_FORMAT_D32_FLOAT;

struct SwapChain
{
    SwapChain(IDXGIFactory4 *pFactory, ComPtr<ID3D12Device4> device,
              ID3D12CommandQueue *pCommandQueue, HWND hwnd, UINT width, UINT height);

    void CreateRenderTargets();
    void Present(UINT syncInterval, UINT flags);

    UINT GetCurrentBackBufferIndex();
    ID3D12Resource *CurrentRenderTarget();
    CD3DX12_CPU_DESCRIPTOR_HANDLE CurrentRenderTargetHandle();

  private:
    UINT m_numFrames;
    ComPtr<ID3D12Device4> m_device;
    ComPtr<IDXGISwapChain3> m_swapChain;
    DescriptorHeap m_descriptorHeap;
    std::array<ComPtr<ID3D12Resource>, NumFrames> m_renderTargets;
};

} // namespace dxultra
