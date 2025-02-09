#pragma once

#include "stdafx.h"

#include "Fence.h"
#include "Frame.h"
#include "SwapChain.h"
#include "WindowCallback.h"

namespace dxultra
{

struct DXUltra : WindowCallback
{
    void OnInit(HWND hwnd, UINT width, UINT height) override;
    void OnUpdate() override;
    void OnRender() override;
    void OnDestroy() override;

  private:
    void CreatePipeline();

    ComPtr<ID3D12Device4> m_device;
    ComPtr<ID3D12CommandQueue> m_commandQueue;
    std::unique_ptr<SwapChain> m_swapChain;

    // This command list gets reused by individual frames,
    // who bring their own allocators.
    ComPtr<ID3D12GraphicsCommandList> m_commandList;

    // For "globally" waiting on the command queue, e.g. "outside" of any frame,
    // when the window is about to be destroyed.
    std::unique_ptr<Fence> m_fence;

    std::array<std::unique_ptr<Frame>, NumFrames> m_frames;
};

} // namespace dxultra
