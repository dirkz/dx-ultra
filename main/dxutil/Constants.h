#pragma once

namespace dxultra
{

constexpr UINT NumFrames = 2;
constexpr DXGI_FORMAT SwapChainFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

/// <summary>
/// The depth stencil format.
/// <code>DXGI_FORMAT_D24_UNORM_S8_UINT</code> is a 24bit float
/// between 0.0 and 1.0, while the stencil component is an 8bit unsigned integer.
/// </summary>
constexpr DXGI_FORMAT DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

}
