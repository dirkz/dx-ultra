add_library(windows OBJECT
	Window.h
	Window.cpp
	WindowCallback.h
	"Strings.h"
	"Strings.cpp"
	Timer.h
	Timer.cpp)

target_include_directories(windows PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(windows PRIVATE common)
