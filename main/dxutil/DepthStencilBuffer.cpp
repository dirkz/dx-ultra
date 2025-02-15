#include "DepthStencilBuffer.h"

namespace dxultra
{

DepthStencilBuffer::DepthStencilBuffer(ComPtr<ID3D12Device4> device, UINT width, UINT height)
    : m_device{device}, m_descriptorHeap{device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false}
{
    CD3DX12_RESOURCE_DESC depthStencilDesc = CD3DX12_RESOURCE_DESC::Tex2D(
        DepthStencilFormat, width, height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

    D3D12_CLEAR_VALUE clearValue{DepthStencilFormat, 1.f, 0};

    D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES{D3D12_HEAP_TYPE_DEFAULT};

    ThrowIfFailed(device->CreateCommittedResource(
        &heapProperties, D3D12_HEAP_FLAG_NONE, &depthStencilDesc, D3D12_RESOURCE_STATE_COMMON,
        &clearValue, IID_PPV_ARGS(m_resource.GetAddressOf())));

    m_device->CreateDepthStencilView(m_resource.Get(), nullptr, m_descriptorHeap.HandleCPU(0));
}

void DepthStencilBuffer::Transition(ID3D12GraphicsCommandList *pCommandList)
{
    auto transitionFromCommonToDepthWrite = CD3DX12_RESOURCE_BARRIER::Transition(
        m_resource.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
    pCommandList->ResourceBarrier(1, &transitionFromCommonToDepthWrite);
}

CD3DX12_CPU_DESCRIPTOR_HANDLE DepthStencilBuffer::DescriptorHandle()
{
    return m_descriptorHeap.HandleCPU(0);
}

} // namespace dxultra