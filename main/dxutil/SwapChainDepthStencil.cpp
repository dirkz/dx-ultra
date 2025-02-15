#include "SwapChainDepthStencil.h"

namespace dxultra
{

SwapChainDepthStencil::SwapChainDepthStencil(IDXGIFactory4 *pFactory, ComPtr<ID3D12Device> device,
                                             ComPtr<ID3D12CommandQueue> commandQueue, HWND hwnd,
                                             UINT width, UINT height)
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
    ThrowIfFailed(pFactory->CreateSwapChainForHwnd(commandQueue.Get(), hwnd, &swapChainDesc,
                                                   nullptr, nullptr, swapChain.GetAddressOf()));

    ThrowIfFailed(swapChain.As(&m_swapChain));

    ThrowIfFailed(pFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));

    ThrowIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                 IID_PPV_ARGS(m_commandAllocator.GetAddressOf())));
}

void SwapChainDepthStencil::Resize(ID3D12GraphicsCommandList *pCommandList, UINT width, UINT height)
{
}

} // namespace dxultra