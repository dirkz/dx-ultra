#pragma once

#include "stdafx.h"

#include "Fence.h"

namespace dxultra
{

struct DefaultBufferUploader
{

    DefaultBufferUploader(ComPtr<ID3D12Device4> device, ComPtr<ID3D12CommandQueue> commandQueue,
                          ComPtr<ID3D12GraphicsCommandList> commandList);

    /// <summary>
    /// Schedules the given data to be uploaded to the GPU.
    /// </summary>
    /// <param name="pData"></param>
    /// <param name="size"></param>
    /// <returns>The GPU buffer that will have been uploaded after <code>Execute</code> has been
    /// called.</returns>
    ComPtr<ID3D12Resource1> Upload(const void *pData, UINT64 size);

    /// <summary>
    /// Executes the scheduled uploads on the GPU and waits for it.
    /// </summary>
    void Execute();

  private:
    ComPtr<ID3D12Device4> m_device;
    ComPtr<ID3D12CommandQueue> m_commandQueue;
    ComPtr<ID3D12GraphicsCommandList> m_commandList;
    ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    Fence m_fence;
    std::vector<ComPtr<ID3D12Resource1>> m_uploadBuffers;
};

} // namespace dxultra
