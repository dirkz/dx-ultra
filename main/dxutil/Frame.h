#pragma once

#include "stdafx.h"

#include "Fence.h"

namespace dxultra
{

struct Frame
{
    Frame(ID3D12Device4 *pDevice, UINT index);

    /// <summary>
    /// Wait for all previous commands from this frame to finish executing
    /// </summary>
    void Wait();

    /// <summary>
    /// Fill the given command list with render commands for this frame
    /// </summary>
    /// <param name="pCommandQueue">The command queue should only be used for
    /// synchronization</param> <param name="pCommandList">The graphics command list to fill with
    /// commands</param> <param name="pRenderTarget">The current render target from the swap
    /// chain</param> <param name="renderTargetHandle">The handle to the current render target from
    /// the swap chain</param>
    void PopulateCommandList(ID3D12CommandQueue *pCommandQueue,
                             ID3D12GraphicsCommandList *pCommandList, ID3D12Resource *pRenderTarget,
                             CD3DX12_CPU_DESCRIPTOR_HANDLE renderTargetHandle);

    /// <summary>
    /// Signal the end of the execution of this frame on the given command queue
    /// </summary>
    /// <param name="pCommandQueue">The command queue to signal finishing of work on</param>
    void Signal(ID3D12CommandQueue *pCommandQueue);

  private:
    UINT m_index;
    ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    Fence m_fence;
};

} // namespace dxultra
