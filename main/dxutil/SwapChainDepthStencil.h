#pragma once

#include "stdafx.h"

#include "Constants.h"
#include "Fence.h"

namespace dxultra
{

struct SwapChainDepthStencil
{
    SwapChainDepthStencil(IDXGIFactory4 *pFactory, ComPtr<ID3D12Device4> device,
                          ComPtr<ID3D12CommandQueue> commandQueue, HWND hwnd, UINT width,
                          UINT height);

    void Resize(ID3D12GraphicsCommandList *pCommandList, UINT width, UINT height);

    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilDescriptorHandle();
    CD3DX12_CPU_DESCRIPTOR_HANDLE SwapChainCurrentRenderTargetHandle();

    UINT GetCurrentBackBufferIndex();
    ID3D12Resource *CurrentRenderTarget();
    void Present(UINT syncInterval, UINT flags);

  private:
    ComPtr<ID3D12Device4> m_device;
    ComPtr<ID3D12CommandQueue> m_commandQueue;
    ComPtr<IDXGISwapChain3> m_swapChain;

    Fence m_fence;

    ComPtr<ID3D12CommandAllocator> m_commandAllocator;

    std::array<ComPtr<ID3D12Resource>, NumFrames> m_renderTargets;
    ComPtr<ID3D12DescriptorHeap> m_descriptorHeapRenderTargets;
    UINT m_descriptorHeapRenderTargetsIncrementSize;

    ComPtr<ID3D12Resource> m_depthStencilBuffer;
    ComPtr<ID3D12DescriptorHeap> m_descriptorHeapDepthStencil;
};

} // namespace dxultra
