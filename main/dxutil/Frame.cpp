#include "Frame.h"

namespace dxultra
{

Frame::Frame(ID3D12Device4 *pDevice, UINT index) : m_index{index}, m_fence{pDevice}
{
    ThrowIfFailed(pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                  IID_PPV_ARGS(m_commandAllocator.GetAddressOf())));
}

void Frame::Wait()
{
    m_fence.Wait();
}

void Frame::PopulateCommandList(ID3D12CommandQueue *pCommandQueue,
                                ID3D12GraphicsCommandList *pCommandList,
                                ID3D12Resource *pRenderTarget,
                                CD3DX12_CPU_DESCRIPTOR_HANDLE renderTargetHandle)
{
    ThrowIfFailed(pCommandList->Reset(m_commandAllocator.Get(), nullptr));

    auto transitionPresentToRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
        pRenderTarget, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    pCommandList->ResourceBarrier(1, &transitionPresentToRenderTarget);

    const float clearColor[] = {0.0f, 0.2f, 0.4f, 1.0f};
    pCommandList->ClearRenderTargetView(renderTargetHandle, clearColor, 0, nullptr);

    auto transitionRenderTargetToPresent = CD3DX12_RESOURCE_BARRIER::Transition(
        pRenderTarget, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    pCommandList->ResourceBarrier(1, &transitionRenderTargetToPresent);

    ThrowIfFailed(pCommandList->Close());
}

void Frame::Signal(ID3D12CommandQueue *pCommandQueue)
{
    m_fence.Signal(pCommandQueue);
}

} // namespace dxultra