#pragma once

#include "stdafx.h"

#include "DescriptorHeap.h"

namespace dxultra
{

/// <summary>
/// The depth stencil format.
/// <code>DXGI_FORMAT_D24_UNORM_S8_UINT</code> is a 24bit float
/// between 0.0 and 1.0, while the stencil component is an 8bit unsigned integer.
/// </summary>
constexpr DXGI_FORMAT DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

struct DepthStencilBuffer
{
    DepthStencilBuffer(ComPtr<ID3D12Device4> device);

    CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorHandle();

  private:
    ComPtr<ID3D12Device4> m_device;
    DescriptorHeap m_descriptorHeap;
};

} // namespace dxultra
