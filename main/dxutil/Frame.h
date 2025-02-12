#pragma once

#include "stdafx.h"

#include "Fence.h"

namespace dxultra
{

struct Frame
{
    Frame(ID3D12Device4 *pDevice);

    void Start(ID3D12GraphicsCommandList *pCommandList);
    void Finish(ID3D12CommandQueue *pCommandQueue);

  private:
    UINT m_index;
    ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    Fence m_fence;
};

} // namespace dxultra
