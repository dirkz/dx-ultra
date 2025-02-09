#include "Fence.h"

namespace dxultra
{

constexpr UINT64 InitialFenceValue = 0;

Fence::Fence(ID3D12Device4 *pDevice) : m_value{InitialFenceValue}
{
    ThrowIfFailed(pDevice->CreateFence(InitialFenceValue, D3D12_FENCE_FLAG_NONE,
                                       IID_PPV_ARGS(m_fence.GetAddressOf())));
    m_event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (!m_event)
    {
        CheckLastError();
    }
}

Fence::~Fence()
{
    CloseHandle(m_event);
}

void Fence::Signal(ID3D12CommandQueue *pCommandQueue)
{
    ++m_value;
    ThrowIfFailed(pCommandQueue->Signal(m_fence.Get(), m_value));
}

void Fence::Wait()
{
    if (m_value == InitialFenceValue)
    {
        return;
    }

    if (m_fence->GetCompletedValue() < m_value)
    {
        ThrowIfFailed(m_fence->SetEventOnCompletion(m_value, m_event));
        WaitForSingleObject(m_event, INFINITE);
    }
}

void Fence::SignalAndWait(ID3D12CommandQueue *pCommandQueue)
{
    Signal(pCommandQueue);
    Wait();
}

} // namespace dxultra