#pragma once

#include "stdafx.h"

#include "Fence.h"

namespace dxultra
{

struct Frame
{
    Frame(ID3D12Device4 *pDevice, UINT index);

    void Render(ID3D12CommandQueue *pCommandQueue, ID3D12GraphicsCommandList *pCommandList,
                ID3D12Resource *pRenderTarget, CD3DX12_CPU_DESCRIPTOR_HANDLE renderTargetHandle);

  private:
    UINT m_index;
    ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    Fence m_fence;
};

} // namespace dxultra
