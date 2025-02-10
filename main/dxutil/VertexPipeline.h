#pragma once

#include "stdafx.h"

#include "Shader.h"

namespace dxultra
{

struct VertexPipeline
{
    VertexPipeline(const std::wstring &vertexShaderName, const std::wstring &pixelShaderName);

    D3D12_INPUT_LAYOUT_DESC InputLayoutDescription();

    CD3DX12_SHADER_BYTECODE VertexShaderByteCode();
    CD3DX12_SHADER_BYTECODE PixelShaderByteCode();

  private:
    Shader m_vertexShader;
    Shader m_pixelShader;
};

} // namespace dxultra
