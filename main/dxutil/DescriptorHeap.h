#pragma once

#include "stdafx.h"

namespace dxultra
{

struct DescriptorHeap
{
    DescriptorHeap(ID3D12Device *pDevice, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors,
                   bool isShaderVisible);

    CD3DX12_CPU_DESCRIPTOR_HANDLE HandleCPU(UINT index);
    CD3DX12_GPU_DESCRIPTOR_HANDLE HandleGPU(UINT index);

  private:
    ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
    UINT m_descriptorHandleIncrementSize;
    CD3DX12_CPU_DESCRIPTOR_HANDLE m_hCPU;
    CD3DX12_GPU_DESCRIPTOR_HANDLE m_hGPU;
};

} // namespace dxultra
