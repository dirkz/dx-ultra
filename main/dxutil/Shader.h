#pragma once

#include "stdafx.h"

namespace dxultra
{

struct Shader
{
    Shader(std::string filename);

    ID3DBlob *Blob();

  private:
    ComPtr<ID3DBlob> m_blob;
};

} // namespace dxultra
