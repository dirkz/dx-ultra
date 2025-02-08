#pragma once

#include "stdafx.h"

namespace dxultra
{

struct Fence
{
    Fence(ID3D12Device4 *pDevice);
    ~Fence();

  private:
    UINT64 m_value;
    ComPtr<ID3D12Fence> m_fence;
    HANDLE m_event;
};

} // namespace dxultra
