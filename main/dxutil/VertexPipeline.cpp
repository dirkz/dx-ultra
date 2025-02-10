#include "VertexPipeline.h"

#include "Vertex.h"

namespace dxultra
{

VertexPipeline::VertexPipeline(const std::string &vertexShaderName,
                               const std::string &pixelShaderName)
    : m_vertexShader{vertexShaderName}, m_pixelShader{pixelShaderName}
{
}

D3D12_INPUT_LAYOUT_DESC VertexPipeline::InputLayoutDescription()
{
    constexpr UINT SemanticIndex = 0;
    constexpr UINT InputSlot = 0;
    constexpr UINT InstanceDataStepRate = 0;

    constexpr D3D12_INPUT_ELEMENT_DESC inputDescriptions[] = {
        {"POSITION", SemanticIndex, DXGI_FORMAT_R32G32B32_FLOAT, InputSlot,
         offsetof(Vertex, Position), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
         InstanceDataStepRate},
        {"COLOR", SemanticIndex, DXGI_FORMAT_R32G32B32_FLOAT, InputSlot, offsetof(Vertex, Color),
         D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, InstanceDataStepRate}};

    return {inputDescriptions, _countof(inputDescriptions)};
}

CD3DX12_SHADER_BYTECODE VertexPipeline::VertexShaderByteCode()
{
    return m_vertexShader.ByteCode();
}

CD3DX12_SHADER_BYTECODE VertexPipeline::PixelShaderByteCode()
{
    return m_pixelShader.ByteCode();
}

} // namespace dxultra