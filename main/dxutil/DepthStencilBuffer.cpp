#include "DepthStencilBuffer.h"

namespace dxultra
{

DepthStencilBuffer::DepthStencilBuffer(ComPtr<ID3D12Device4> device)
    : m_device{device}, m_descriptorHeap{device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false}
{
}

CD3DX12_CPU_DESCRIPTOR_HANDLE DepthStencilBuffer::DescriptorHandle()
{
    return m_descriptorHeap.HandleCPU(0);
}

} // namespace dxultra