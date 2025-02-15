#include "SwapChainDepthStencil.h"

namespace dxultra
{

SwapChainDepthStencil::SwapChainDepthStencil(IDXGIFactory4 *pFactory, ComPtr<ID3D12Device4> device,
                                             ComPtr<ID3D12CommandQueue> commandQueue, HWND hwnd,
                                             UINT width, UINT height)
    : m_device{device}, m_commandQueue{commandQueue}, m_fence{device.Get()}
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
    ThrowIfFailed(pFactory->CreateSwapChainForHwnd(m_commandQueue.Get(), hwnd, &swapChainDesc,
                                                   nullptr, nullptr, swapChain.GetAddressOf()));

    ThrowIfFailed(swapChain.As(&m_swapChain));

    ThrowIfFailed(pFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));

    ThrowIfFailed(m_device->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_commandAllocator.GetAddressOf())));

    // render target descriptor heap

    D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
    descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    descriptorHeapDesc.NumDescriptors = NumFrames;

    ThrowIfFailed(m_device->CreateDescriptorHeap(
        &descriptorHeapDesc, IID_PPV_ARGS(m_descriptorHeapRenderTargets.GetAddressOf())));

    m_descriptorHeapRenderTargetsIncrementSize =
        m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // depth stencil descriptor heap

    descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    descriptorHeapDesc.NumDescriptors = 1;

    ThrowIfFailed(m_device->CreateDescriptorHeap(
        &descriptorHeapDesc, IID_PPV_ARGS(m_descriptorHeapDepthStencil.GetAddressOf())));
}

void SwapChainDepthStencil::Resize(ID3D12GraphicsCommandList *pCommandList, UINT width, UINT height)
{
    // Anything that depends on the swap chain must be finished
    // before we manipulate it.
    m_fence.SignalAndWait(m_commandQueue.Get());

    // swap chain

    // Have to release any previous render targets _before_ the actual resize,
    // so a later ReleaseAndGetAddressOf on re-creation is not an option.
    for (auto i = 0; i < m_renderTargets.size(); ++i)
    {
        m_renderTargets[i].Reset();
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
        &clearValue, IID_PPV_ARGS(m_depthStencilBuffer.ReleaseAndGetAddressOf())));

    m_device->CreateDepthStencilView(
        m_depthStencilBuffer.Get(), nullptr,
        m_descriptorHeapDepthStencil->GetCPUDescriptorHandleForHeapStart());

    // transition the depth stencil buffer

    ThrowIfFailed(m_commandAllocator->Reset());
    ThrowIfFailed(pCommandList->Reset(m_commandAllocator.Get(), nullptr));

    auto transitionFromCommonToDepthWrite = CD3DX12_RESOURCE_BARRIER::Transition(
        m_depthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
    pCommandList->ResourceBarrier(1, &transitionFromCommonToDepthWrite);

    ThrowIfFailed(pCommandList->Close());

    ID3D12CommandList *ppCommandLists[] = {pCommandList};
    m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Wait for the resize to be finished.
    m_fence.SignalAndWait(m_commandQueue.Get());
}

D3D12_CPU_DESCRIPTOR_HANDLE SwapChainDepthStencil::DepthStencilDescriptorHandle()
{
    return m_descriptorHeapDepthStencil->GetCPUDescriptorHandleForHeapStart();
}

CD3DX12_CPU_DESCRIPTOR_HANDLE SwapChainDepthStencil::SwapChainCurrentRenderTargetHandle()
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(
        m_descriptorHeapRenderTargets->GetCPUDescriptorHandleForHeapStart(),
        m_swapChain->GetCurrentBackBufferIndex(), m_descriptorHeapRenderTargetsIncrementSize);
}

UINT SwapChainDepthStencil::GetCurrentBackBufferIndex()
{
    return m_swapChain->GetCurrentBackBufferIndex();
}

ID3D12Resource *SwapChainDepthStencil::CurrentRenderTarget()
{
    return m_renderTargets[m_swapChain->GetCurrentBackBufferIndex()].Get();
}

void SwapChainDepthStencil::Present(UINT syncInterval, UINT flags)
{
    m_swapChain->Present(syncInterval, flags);
}

} // namespace dxultra