add_library(dxutil OBJECT
	Constants.h
	DescriptorHeap.h
	DescriptorHeap.cpp
	Fence.h
	Fence.cpp
	Frame.h
	Frame.cpp
	Shader.h
	Shader.cpp
	DefaultBufferUploader.h
	DefaultBufferUploader.cpp
	Vertex.h
	Vertex.cpp
	VertexPipeline.h
	VertexPipeline.cpp
	SwapChainDepthStencil.h
	SwapChainDepthStencil.cpp)

target_include_directories(dxutil PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(dxutil PRIVATE common)