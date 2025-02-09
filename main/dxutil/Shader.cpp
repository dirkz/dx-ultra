#include "Shader.h"

namespace dxultra
{

Shader::Shader(std::string filename)
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
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();
}

} // namespace dxultra