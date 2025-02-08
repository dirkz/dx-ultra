#include "DXUltra.h"

#include "Constants.h"

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

    ThrowIfFailed(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0,
                                    IID_PPV_ARGS(m_device.GetAddressOf())));

    D3D12_COMMAND_QUEUE_DESC queueDesc{};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    ThrowIfFailed(
        m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(m_commandQueue.GetAddressOf())));

    m_swapChain.reset(new SwapChain{factory.Get(), m_device, m_commandQueue.Get(), NumFrames, hwnd,
                                    width, height});
}

void DXUltra::OnUpdate()
{
}

void DXUltra::OnRender()
{
}

} // namespace dxultra