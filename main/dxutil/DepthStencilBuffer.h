#pragma once

#include "stdafx.h"

#include "DescriptorHeap.h"

namespace dxultra
{

struct DepthStencilBuffer
{
    DepthStencilBuffer(ComPtr<ID3D12Device4> device);

    CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorHandle();

  private:
    ComPtr<ID3D12Device4> m_device;
    DescriptorHeap m_descriptorHeap;
};

} // namespace dxultra
