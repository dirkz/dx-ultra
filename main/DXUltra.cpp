#include "DXUltra.h"

#include "Constants.h"
#include "DefaultBufferUploader.h"
#include "Vertex.h"
#include "VertexPipeline.h"

namespace dxultra
{

DXUltra::DXUltra()
    : m_viewport{0.f, 0.f, 0.f, 0.f}, m_scissorRect{0, 0, 0, 0}, m_vertexBufferView{},
      m_indexBufferView{}
{
}

void DXUltra::OnInit(HWND hwnd, UINT width, UINT height)
{
    m_viewport.Height = static_cast<FLOAT>(height);
    m_viewport.Width = static_cast<FLOAT>(width);

    m_scissorRect.bottom = height;
    m_scissorRect.right = width;

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

    // EnumAdapters1 returns the primary adapter first, that is,
    // the one in use right now. Sounds like good enough.
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

    m_depthStencilBuffer.reset(new DepthStencilBuffer{m_device, width, height});

    ThrowIfFailed(m_device->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
                                               D3D12_COMMAND_LIST_FLAG_NONE,
                                               IID_PPV_ARGS(m_commandList.GetAddressOf())));

    ThrowIfFailed(m_device->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_commandAllocator.GetAddressOf())));

    m_fence.reset(new Fence{m_device.Get()});

    for (UINT i = 0; i < NumFrames; i++)
    {
        m_frames[i].reset(new Frame{m_device.Get()});
    }

    CreatePipeline();
    UploadDataAndTransitionDepthStencilBuffer();
}

void DXUltra::OnUpdate()
{
}

void DXUltra::OnRender()
{
    Frame *pFrame = m_frames[m_swapChain->GetCurrentBackBufferIndex()].get();

    pFrame->Start(m_commandList.Get(), m_pipelineState.Get());

    m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
    m_commandList->RSSetViewports(1, &m_viewport);
    m_commandList->RSSetScissorRects(1, &m_scissorRect);

    CD3DX12_CPU_DESCRIPTOR_HANDLE renderTargetHandle = m_swapChain->CurrentRenderTargetHandle();

    auto transitionPresentToRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
        m_swapChain->CurrentRenderTarget(), D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET);
    m_commandList->ResourceBarrier(1, &transitionPresentToRenderTarget);

    m_commandList->OMSetRenderTargets(1, &renderTargetHandle, FALSE, nullptr);

    const float clearColor[] = {0.0f, 0.2f, 0.4f, 1.0f};
    m_commandList->ClearRenderTargetView(renderTargetHandle, clearColor, 0, nullptr);

    m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_commandList->IASetIndexBuffer(&m_indexBufferView);
    m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
    m_commandList->DrawIndexedInstanced(3, 1, 0, 0, 0);

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

void DXUltra::OnResize(UINT width, UINT height)
{
    // Anything that depends on the swap chain must be finished
    // before we manipulate it.
    m_fence->SignalAndWait(m_commandQueue.Get());

    m_swapChain->Resize(width, height);
    m_depthStencilBuffer.reset(new DepthStencilBuffer{m_device, width, height});

    ThrowIfFailed(m_commandAllocator->Reset());
    ThrowIfFailed(m_commandList->Reset(m_commandAllocator.Get(), nullptr));

    m_depthStencilBuffer->Transition(m_commandList.Get());

    ThrowIfFailed(m_commandList->Close());

    ID3D12CommandList *ppCommandLists[] = {m_commandList.Get()};
    m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Wait for the resize to be finished.
    m_fence->SignalAndWait(m_commandQueue.Get());
}

void DXUltra::OnDestroy()
{
    m_fence->SignalAndWait(m_commandQueue.Get());
}

void DXUltra::CreatePipeline()
{
    VertexPipeline pipeline{m_device.Get(), L"basic_triangle.hlsl_VS.cso",
                            L"basic_triangle.hlsl_PS.cso"};

    m_rootSignature = pipeline.RootSignature();

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = pipeline.InputLayoutDescription();
    psoDesc.pRootSignature = m_rootSignature.Get();
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

void DXUltra::UploadDataAndTransitionDepthStencilBuffer()
{
    Vertex vertices[]{{-0.5, -0.5, 0, Colors::OrangeRed},
                      {0, 0.5, 0, Colors::DarkSeaGreen},
                      {0.5, -0.5, 0, Colors::AliceBlue}};
    UINT16 indices[]{0, 1, 2};

    DefaultBufferUploader uploader{m_device.Get(), m_commandQueue.Get(), m_commandList.Get()};

    m_vertexBuffer = uploader.Upload(vertices, sizeof(vertices));
    m_indexBuffer = uploader.Upload(indices, sizeof(indices));

    // Piggy-back on the command list the uploader prepared for
    // recording for transitioning the depth stencil buffer.
    m_depthStencilBuffer->Transition(m_commandList.Get());

    uploader.Execute();

    m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
    m_vertexBufferView.StrideInBytes = sizeof(Vertex);
    m_vertexBufferView.SizeInBytes = sizeof(vertices);

    m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
    m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
    m_indexBufferView.SizeInBytes = sizeof(indices);
}

} // namespace dxultra