#pragma once

#include "stdafx.h"

#include "DescriptorHeap.h"

namespace dxultra
{

struct DepthStencilBuffer
{
    DepthStencilBuffer(ComPtr<ID3D12Device4> device, UINT width, UINT height);

    /// <summary>
    /// Inserts a resource barrier into the given graphics command list for
    /// a transition from common to depth-write.
    /// </summary>
    /// <param name="pCommandList">The graphics command list. Must be ready for commands</param>
    void Transition(ID3D12GraphicsCommandList *pCommandList);

    CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorHandle();

  private:
    ComPtr<ID3D12Device4> m_device;
    DescriptorHeap m_descriptorHeap;
    ComPtr<ID3D12Resource> m_resource;
};

} // namespace dxultra
