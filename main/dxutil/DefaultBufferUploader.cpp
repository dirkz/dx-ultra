#include "DefaultBufferUploader.h"

namespace dxultra
{

DefaultBufferUploader::DefaultBufferUploader(ID3D12Device4 *pDevice,
                                             ID3D12CommandQueue *pCommandQueue,
                                             ID3D12GraphicsCommandList *pCommandList)
    : m_device{pDevice}, m_commandQueue{pCommandQueue}, m_commandList{pCommandList},
      m_fence{pDevice}
{
    ThrowIfFailed(pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                  IID_PPV_ARGS(m_commandAllocator.GetAddressOf())));
}

} // namespace dxultra