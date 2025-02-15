#pragma once

#include "stdafx.h"

#include "Constants.h"

namespace dxultra
{

struct SwapChainDepthStencil
{

    SwapChainDepthStencil(IDXGIFactory4 *pFactory, ComPtr<ID3D12Device> device,
                          ComPtr<ID3D12CommandQueue> commandQueue, HWND hwnd, UINT width,
                          UINT height);

    void Resize(ID3D12GraphicsCommandList *pCommandList, UINT width, UINT height);

  private:
    ComPtr<IDXGISwapChain3> m_swapChain;
    ComPtr<ID3D12CommandAllocator> m_commandAllocator;
};

} // namespace dxultra
