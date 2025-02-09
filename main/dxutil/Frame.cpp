#include "Frame.h"

namespace dxultra
{

Frame::Frame(ID3D12Device4 *pDevice, UINT index) : m_index{index}
{
    ThrowIfFailed(pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                  IID_PPV_ARGS(m_commandAllocator.GetAddressOf())));
}

} // namespace dxultra