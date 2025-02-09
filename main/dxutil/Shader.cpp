#include "Shader.h"

namespace dxultra
{

Shader::Shader(std::string filename)
{
    DWORD capacity = MAX_PATH;
    std::wstring strFilename{};
    strFilename.reserve(capacity);

    DWORD size = GetModuleFileName(NULL, strFilename.data(), capacity);
    while (size == capacity && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
    {
        capacity *= 2;
        strFilename.reserve(capacity);
    }
}

} // namespace dxultra