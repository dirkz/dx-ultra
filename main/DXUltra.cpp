#include "DXUltra.h"

#include "Constants.h"
#include "DefaultBufferUploader.h"
#include "Vertex.h"
#include "VertexPipeline.h"

namespace dxultra
{

void DXUltra::OnInit(HWND hwnd, UINT width, UINT height)
{
#if defined(_DEBUG)
    // Enable the D3D12 debug layer.
    {

        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();
        }
    }
#endif

    ComPtr<IDXGIFactory4> factory;
    ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));

    // EnumAdapters1 returns the primary adapter first, I assume
    // this is the primary device, for now.
    ComPtr<IDXGIAdapter1> adapter;
    ThrowIfFailed(factory->EnumAdapters1(0, adapter.GetAddressOf()));

    ComPtr<ID3D12Device> device;
    ThrowIfFailed(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0,
                                    IID_PPV_ARGS(device.GetAddressOf())));

    ThrowIfFailed(device.As(&m_device));

    D3D12_COMMAND_QUEUE_DESC queueDesc{};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    ThrowIfFailed(
        m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(m_commandQueue.GetAddressOf())));

    m_swapChain.reset(
        new SwapChain{factory.Get(), m_device, m_commandQueue.Get(), hwnd, width, height});

    ThrowIfFailed(m_device->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
                                               D3D12_COMMAND_LIST_FLAG_NONE,
                                               IID_PPV_ARGS(m_commandList.GetAddressOf())));

    m_fence.reset(new Fence{m_device.Get()});

    for (UINT i = 0; i < NumFrames; i++)
    {
        m_frames[i].reset(new Frame{m_device.Get()});
    }

    CreatePipeline();
    UploadData();
}

void DXUltra::OnUpdate()
{
}

void DXUltra::OnRender()
{
    Frame *pFrame = m_frames[m_swapChain->GetCurrentBackBufferIndex()].get();

    pFrame->Start(m_commandList.Get());

    CD3DX12_CPU_DESCRIPTOR_HANDLE renderTargetHandle = m_swapChain->CurrentRenderTargetHandle();

    auto transitionPresentToRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
        m_swapChain->CurrentRenderTarget(), D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET);
    m_commandList->ResourceBarrier(1, &transitionPresentToRenderTarget);

    const float clearColor[] = {0.0f, 0.2f, 0.4f, 1.0f};
    m_commandList->ClearRenderTargetView(renderTargetHandle, clearColor, 0, nullptr);

    auto transitionRenderTargetToPresent = CD3DX12_RESOURCE_BARRIER::Transition(
        m_swapChain->CurrentRenderTarget(), D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT);
    m_commandList->ResourceBarrier(1, &transitionRenderTargetToPresent);

    ThrowIfFailed(m_commandList->Close());

    ID3D12CommandList *ppCommandLists[] = {m_commandList.Get()};
    m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    m_swapChain->Present(1, 0);

    pFrame->Finish(m_commandQueue.Get());
}

void DXUltra::OnDestroy()
{
    m_fence->SignalAndWait(m_commandQueue.Get());
}

void DXUltra::CreatePipeline()
{
    VertexPipeline pipeline{m_device.Get(), L"basic_triangle.hlsl_VS.cso",
                            L"basic_triangle.hlsl_PS.cso"};

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = pipeline.InputLayoutDescription();
    psoDesc.pRootSignature = pipeline.RootSignature();
    psoDesc.VS = pipeline.VertexShaderByteCode();
    psoDesc.PS = pipeline.PixelShaderByteCode();
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState.DepthEnable = FALSE;
    psoDesc.DepthStencilState.StencilEnable = FALSE;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = SwapChainFormat;
    psoDesc.SampleDesc.Count = 1;

    ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
}

void DXUltra::UploadData()
{
    Vertex vertices[]{{-0.5, -0.5, 0, Colors::OrangeRed},
                      {0, 0.5, 0, Colors::DarkSeaGreen},
                      {0.5, -0.5, 0, Colors::AliceBlue}};
    UINT16 indices[]{0, 1, 2};

    DefaultBufferUploader uploader{m_device.Get(), m_commandQueue.Get(), m_commandList.Get()};

    m_vertexBuffer = uploader.Upload(vertices, sizeof(vertices));
    m_indexBuffer = uploader.Upload(indices, sizeof(indices));

    uploader.Execute();

    m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
    m_vertexBufferView.StrideInBytes = sizeof(Vertex);
    m_vertexBufferView.SizeInBytes = sizeof(vertices);

    m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
    m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
    m_indexBufferView.SizeInBytes = sizeof(indices);
}

} // namespace dxultra