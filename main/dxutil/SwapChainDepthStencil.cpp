#include "SwapChainDepthStencil.h"

namespace dxultra
{

SwapChainDepthStencil::SwapChainDepthStencil(IDXGIFactory4 *pFactory, ComPtr<ID3D12Device> device,
                                             ComPtr<ID3D12CommandQueue> commandQueue, HWND hwnd,
                                             UINT width, UINT height)
    : m_device{device}
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

    // render target descriptor heap

    D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
    descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    descriptorHeapDesc.NumDescriptors = NumFrames;

    ThrowIfFailed(device->CreateDescriptorHeap(
        &descriptorHeapDesc, IID_PPV_ARGS(m_descriptorHeapRenderTargets.GetAddressOf())));

    m_descriptorHeapRenderTargetsIncrementSize =
        device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // depth stencil descriptor heap

    descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    descriptorHeapDesc.NumDescriptors = 1;

    ThrowIfFailed(device->CreateDescriptorHeap(
        &descriptorHeapDesc, IID_PPV_ARGS(m_descriptorHeapDepthStencil.GetAddressOf())));
}

void SwapChainDepthStencil::Resize(ID3D12GraphicsCommandList *pCommandList, UINT width, UINT height)
{
    // swap chain

    // Have to release any previous render targets before the actual resize.
    for (ComPtr<ID3D12Resource> renderTargets : m_renderTargets)
    {
        renderTargets.Reset();
    }

    ThrowIfFailed(m_swapChain->ResizeBuffers(NumFrames, width, height, SwapChainFormat, 0));

    D3D12_CPU_DESCRIPTOR_HANDLE rtHandle =
        m_descriptorHeapRenderTargets->GetCPUDescriptorHandleForHeapStart();

    for (auto i = 0; i < NumFrames; ++i)
    {
        ThrowIfFailed(m_swapChain->GetBuffer(i, IID_PPV_ARGS(m_renderTargets[i].GetAddressOf())));
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtv =
            CD3DX12_CPU_DESCRIPTOR_HANDLE{rtHandle, i, m_descriptorHeapRenderTargetsIncrementSize};
        m_device->CreateRenderTargetView(m_renderTargets[i].Get(), nullptr, rtv);
    }

    // depth stencil

    CD3DX12_RESOURCE_DESC depthStencilDesc = CD3DX12_RESOURCE_DESC::Tex2D(
        DepthStencilFormat, width, height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

    D3D12_CLEAR_VALUE clearValue{DepthStencilFormat, 1.f, 0};

    D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES{D3D12_HEAP_TYPE_DEFAULT};

    ThrowIfFailed(m_device->CreateCommittedResource(
        &heapProperties, D3D12_HEAP_FLAG_NONE, &depthStencilDesc, D3D12_RESOURCE_STATE_COMMON,
        &clearValue, IID_PPV_ARGS(m_depthStencilBuffer.GetAddressOf())));

    m_device->CreateDepthStencilView(
        m_depthStencilBuffer.Get(), nullptr,
        m_descriptorHeapDepthStencil->GetCPUDescriptorHandleForHeapStart());
}

} // namespace dxultra