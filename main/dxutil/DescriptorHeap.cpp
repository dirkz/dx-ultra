#include "DescriptorHeap.h"

namespace dxultra
{

DescriptorHeap::DescriptorHeap(ID3D12Device *pDevice, D3D12_DESCRIPTOR_HEAP_TYPE heapType,
                               UINT numDescriptors, bool isShaderVisible)
{
    if (isShaderVisible)
    {
        switch (heapType)
        {
        case D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV:
        case D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER:
            break; // all good
        default:
            throw std::runtime_error{"only CBV, SRV, UAV and sampler heaps can be shader visible"};
        }

        D3D12_DESCRIPTOR_HEAP_FLAGS flags = isShaderVisible
                                                ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
                                                : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        D3D12_DESCRIPTOR_HEAP_DESC desc{};
        desc.Flags = flags;
        desc.Type = heapType;
        desc.NumDescriptors = numDescriptors;

        ThrowIfFailed(
            pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(m_descriptorHeap.GetAddressOf())));

        m_descriptorHandleIncrementSize = pDevice->GetDescriptorHandleIncrementSize(heapType);

        m_hCPU = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();

        if (isShaderVisible)
        {
            m_hGPU = m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
        }
        else
        {
            m_hGPU = CD3DX12_GPU_DESCRIPTOR_HANDLE{};
        }
    }
}

CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::HandleCPU(UINT index)
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE{m_hCPU, m_descriptorHandleIncrementSize, index};
}

CD3DX12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::HandleGPU(UINT index)
{
    return CD3DX12_GPU_DESCRIPTOR_HANDLE{m_hGPU, m_descriptorHandleIncrementSize, index};
}

} // namespace dxultra