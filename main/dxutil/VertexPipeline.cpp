#include "VertexPipeline.h"

#include "Vertex.h"

namespace dxultra
{

constexpr UINT SemanticIndex = 0;
constexpr UINT InputSlot = 0;
constexpr UINT InstanceDataStepRate = 0;

VertexPipeline::VertexPipeline(ID3D12Device4 *pDevice, const std::wstring &vertexShaderName,
                               const std::wstring &pixelShaderName)
    : m_vertexShader{vertexShaderName}, m_pixelShader{pixelShaderName},
      m_inputElementDescriptions{
          D3D12_INPUT_ELEMENT_DESC{"POSITION", SemanticIndex, DXGI_FORMAT_R32G32B32_FLOAT,
                                   InputSlot, offsetof(Vertex, Position),
                                   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
                                   InstanceDataStepRate},
          D3D12_INPUT_ELEMENT_DESC{"COLOR", SemanticIndex, DXGI_FORMAT_R32G32B32_FLOAT, InputSlot,
                                   offsetof(Vertex, Color),
                                   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
                                   InstanceDataStepRate}}
{
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    rootSignatureDesc.Init(0, nullptr, 0, nullptr,
                           D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;
    HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1,
                                             &signature, &error);

    if (FAILED(hr))
    {
        const char *ptrChar = static_cast<const char *>(error->GetBufferPointer());
        std::string msg{ptrChar, error->GetBufferSize()};
        OutputDebugStringA(msg.c_str());
        OutputDebugStringA("\n");
    }
    ThrowIfFailed(hr);

    ThrowIfFailed(pDevice->CreateRootSignature(0, signature->GetBufferPointer(),
                                               signature->GetBufferSize(),
                                               IID_PPV_ARGS(&m_rootSignature)));
}

D3D12_INPUT_LAYOUT_DESC VertexPipeline::InputLayoutDescription()
{
    return {m_inputElementDescriptions.data(),
            static_cast<UINT>(m_inputElementDescriptions.size())};
}

CD3DX12_SHADER_BYTECODE VertexPipeline::VertexShaderByteCode()
{
    return m_vertexShader.ByteCode();
}

CD3DX12_SHADER_BYTECODE VertexPipeline::PixelShaderByteCode()
{
    return m_pixelShader.ByteCode();
}

ID3D12RootSignature *VertexPipeline::RootSignature()
{
    return m_rootSignature.Get();
}

} // namespace dxultra