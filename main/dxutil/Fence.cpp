#include "Fence.h"

namespace dxultra
{

constexpr UINT64 InitialValue = 0;

Fence::Fence(ID3D12Device4 *pDevice) : m_value{InitialValue}
{
    ThrowIfFailed(pDevice->CreateFence(InitialValue, D3D12_FENCE_FLAG_NONE,
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

} // namespace dxultra