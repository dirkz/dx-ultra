#include "SwapChain.h"

namespace dxultra
{

SwapChain::SwapChain(IDXGIFactory1 *pFactory, ID3D12CommandQueue *pCommandQueue, UINT frameCount,
                     HWND hwnd, UINT width, UINT height)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = frameCount;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = SwapChainFormat;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;

    ComPtr<IDXGISwapChain> swapChain;
    ThrowIfFailed(
        pFactory->CreateSwapChain(pCommandQueue, &swapChainDesc, swapChain.GetAddressOf()));

    ThrowIfFailed(swapChain.As(&m_swapChain));

    ThrowIfFailed(pFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));
}

UINT SwapChain::GetCurrentBackBufferIndex()
{
    return m_swapChain->GetCurrentBackBufferIndex();
}

} // namespace dxultra