#include "Shader.h"

namespace dxultra
{

Shader::Shader(std::wstring filename)
{
    DWORD capacity = MAX_PATH;
    std::wstring strModulePath{};
    strModulePath.reserve(capacity);

    DWORD size = GetModuleFileName(NULL, strModulePath.data(), capacity);
    while (size == capacity && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
    {
        capacity *= 2;
        strModulePath.reserve(capacity);
        size = GetModuleFileName(NULL, strModulePath.data(), capacity);
    }

    std::filesystem::path modulePath{strModulePath.c_str()};
    std::filesystem::path filenamePath{filename};
    std::filesystem::path filepath = modulePath.remove_filename() / "shaders" / filenamePath;

    std::ifstream file{filepath, std::ios::ate | std::ios::binary};

    if (!file.is_open())
    {
        throw std::runtime_error{"can't open shader file"};
    }

    size_t fileSize = file.tellg();

    ThrowIfFailed(D3DCreateBlob(fileSize, m_blob.GetAddressOf()));

    file.seekg(0);
    file.read(static_cast<char *>(m_blob->GetBufferPointer()), fileSize);

    file.close();
}

ID3DBlob *Shader::Blob()
{
    return m_blob.Get();
}

CD3DX12_SHADER_BYTECODE Shader::ByteCode()
{
    return CD3DX12_SHADER_BYTECODE{m_blob.Get()};
}

} // namespace dxultra