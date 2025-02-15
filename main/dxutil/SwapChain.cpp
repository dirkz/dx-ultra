#include "SwapChain.h"

namespace dxultra
{

SwapChain::SwapChain(IDXGIFactory4 *pFactory, ComPtr<ID3D12Device4> device,
                     ID3D12CommandQueue *pCommandQueue, HWND hwnd, UINT width, UINT height)
    : m_device{device},
      m_descriptorHeap{device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, NumFrames, false}
{
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = NumFrames;
    swapChainDesc.Width = width;
    swapChainDesc.Height = height;
    swapChainDesc.Format = SwapChainFormat;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swapChain;
    ThrowIfFailed(pFactory->CreateSwapChainForHwnd(pCommandQueue, hwnd, &swapChainDesc, nullptr,
                                                   nullptr, swapChain.GetAddressOf()));

    ThrowIfFailed(swapChain.As(&m_swapChain));

    ThrowIfFailed(pFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));

    CreateRenderTargets();
}

void SwapChain::CreateRenderTargets()
{
    for (UINT i = 0; i < m_renderTargets.size(); ++i)
    {
        ThrowIfFailed(
            m_swapChain->GetBuffer(i, IID_PPV_ARGS(m_renderTargets[i].GetAddressOf())));
        m_device->CreateRenderTargetView(m_renderTargets[i].Get(), nullptr,
                                         m_descriptorHeap.HandleCPU(i));
    }
}

void SwapChain::Present(UINT syncInterval, UINT flags)
{
    ThrowIfFailed(m_swapChain->Present(syncInterval, flags));
}

void SwapChain::Resize(UINT width, UINT height)
{
    // The render targets have to be released before the swap chain gets resized.
    for (UINT i = 0; i < m_renderTargets.size(); ++i)
    {
        m_renderTargets[i].Reset();
    }
    
    ThrowIfFailed(m_swapChain->ResizeBuffers(NumFrames, width, height, SwapChainFormat, 0));
    CreateRenderTargets();
}

UINT SwapChain::GetCurrentBackBufferIndex()
{
    return m_swapChain->GetCurrentBackBufferIndex();
}

ID3D12Resource *SwapChain::CurrentRenderTarget()
{
    return m_renderTargets[m_swapChain->GetCurrentBackBufferIndex()].Get();
}

CD3DX12_CPU_DESCRIPTOR_HANDLE SwapChain::CurrentRenderTargetHandle()
{
    return m_descriptorHeap.HandleCPU(m_swapChain->GetCurrentBackBufferIndex());
}

} // namespace dxultra