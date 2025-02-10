#pragma once

#include "stdafx.h"

namespace dxultra
{

struct Shader
{
    Shader(std::wstring filename);

    ID3DBlob *Blob();
    CD3DX12_SHADER_BYTECODE ByteCode();

  private:
    ComPtr<ID3DBlob> m_blob;
};

} // namespace dxultra
