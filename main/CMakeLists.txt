add_executable(${CMAKE_PROJECT_NAME} WIN32
	main.cpp
	DXUltra.h
	DXUltra.cpp)

add_subdirectory(common)
add_subdirectory(windows)
add_subdirectory(dxutil)
add_subdirectory(shaders)

add_dependencies(${CMAKE_PROJECT_NAME} shaders)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE common)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE windows)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE dxutil)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE d3d12.lib dxgi.lib dxguid.lib D3DCompiler.lib)

