# CMakeList.txt : CMake project for dx-ultra, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.29)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED True)

project ("dxultra" CXX)

add_compile_definitions(_UNICODE UNICODE)

cmake_path(SET DIRECTX_HEADER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/DirectX-Headers/include/directx)

add_subdirectory(common)

# Add source to this project's executable.
add_executable (${CMAKE_PROJECT_NAME} WIN32 "main.cpp")

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE common)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE d3d12.lib dxgi.lib D3DCompiler.lib)

