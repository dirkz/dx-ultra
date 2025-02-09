#pragma once

#include "stdafx.h"

namespace dxultra
{

struct Frame
{
    Frame(ID3D12Device4 *pDevice, UINT index);

  private:
    UINT m_index;
    ComPtr<ID3D12CommandAllocator> m_commandAllocator;
};

} // namespace dxultra
