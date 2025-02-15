#pragma once

#include "stdafx.h"

#include "Constants.h"
#include "Fence.h"
#include "Frame.h"
#include "SwapChainDepthStencil.h"
#include "WindowCallback.h"

namespace dxultra
{

struct DXUltra : WindowCallback
{
    DXUltra();

    void OnInit(HWND hwnd, UINT width, UINT height) override;
    void OnUpdate() override;
    void OnRender() override;
    void OnResize(UINT width, UINT height) override;
    void OnDestroy() override;

  private:
    void CreatePipeline();
    void UploadData();

    CD3DX12_VIEWPORT m_viewport;
    CD3DX12_RECT m_scissorRect;

    ComPtr<ID3D12Device4> m_device;
    ComPtr<ID3D12CommandQueue> m_commandQueue;
    std::unique_ptr<SwapChainDepthStencil> m_swapChain;

    // This command list gets reused by individual frames,
    // who bring their own allocators.
    ComPtr<ID3D12GraphicsCommandList> m_commandList;

    // For "globally" waiting on the command queue, e.g. "outside" of any frame,
    // when the window is about to be destroyed, or when resizing.
    std::unique_ptr<Fence> m_fence;

    std::array<std::unique_ptr<Frame>, NumFrames> m_frames;

    ComPtr<ID3D12PipelineState> m_pipelineState;
    ComPtr<ID3D12RootSignature> m_rootSignature;

    ComPtr<ID3D12Resource1> m_vertexBuffer;
    ComPtr<ID3D12Resource1> m_indexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
};

} // namespace dxultra
