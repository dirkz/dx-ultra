#include "Frame.h"

namespace dxultra
{

Frame::Frame(ID3D12Device4 *pDevice) : m_fence{pDevice}
{
    ThrowIfFailed(pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                  IID_PPV_ARGS(m_commandAllocator.GetAddressOf())));
}

void Frame::Start(ID3D12GraphicsCommandList *pCommandList, ID3D12PipelineState *pPipelineState)
{
    m_fence.Wait();

    ThrowIfFailed(m_commandAllocator->Reset());
    ThrowIfFailed(pCommandList->Reset(m_commandAllocator.Get(), pPipelineState));
}

void Frame::Finish(ID3D12CommandQueue *pCommandQueue)
{
    m_fence.Signal(pCommandQueue);
}

} // namespace dxultra