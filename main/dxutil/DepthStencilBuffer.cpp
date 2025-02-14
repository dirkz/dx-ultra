#include "DepthStencilBuffer.h"

namespace dxultra
{

DepthStencilBuffer::DepthStencilBuffer(ComPtr<ID3D12Device4> device, UINT width, UINT height)
    : m_device{device}, m_descriptorHeap{device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false}
{
    D3D12_RESOURCE_DESC desc{};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    desc.Alignment = 0;
    desc.Width = width;
    desc.Height = height;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DepthStencilFormat;
    desc.SampleDesc = {1, 0};
    desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE clearValue{DepthStencilFormat, 1.f, 0};

    D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES{D3D12_HEAP_TYPE_DEFAULT};

    ThrowIfFailed(device->CreateCommittedResource(
        &heapProperties, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COMMON, &clearValue,
        IID_PPV_ARGS(m_depthStencilBuffer.GetAddressOf())));
}

CD3DX12_CPU_DESCRIPTOR_HANDLE DepthStencilBuffer::DescriptorHandle()
{
    return m_descriptorHeap.HandleCPU(0);
}

} // namespace dxultra