#pragma once

#include "stdafx.h"

#include "Fence.h"

namespace dxultra
{

struct DefaultBufferUploader
{

    DefaultBufferUploader(ID3D12Device4 *pDevice, ID3D12CommandQueue *pCommandQueue,
                          ID3D12GraphicsCommandList *pCommandList);

  private:
    ID3D12Device4 *m_device;
    ID3D12CommandQueue *m_commandQueue;
    ID3D12GraphicsCommandList *m_commandList;
    ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    Fence m_fence;
};

} // namespace dxultra
