#pragma once

#include "stdafx.h"

#include "Fence.h"

namespace dxultra
{

struct DefaultBufferUploader
{

    DefaultBufferUploader(ComPtr<ID3D12Device4> device, ComPtr<ID3D12CommandQueue> commandQueue,
                          ComPtr<ID3D12GraphicsCommandList> commandList);

    ComPtr<ID3D12Resource1> Upload(const void *pData, UINT64 size);

  private:
    ComPtr<ID3D12Device4> m_device;
    ComPtr<ID3D12CommandQueue> m_commandQueue;
    ComPtr<ID3D12GraphicsCommandList> m_commandList;
    ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    Fence m_fence;
    std::vector<ComPtr<ID3D12Resource1>> m_uploadBuffers;
};

} // namespace dxultra
