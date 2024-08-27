cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

project(Coding-Companion)
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES 
	src/main.cpp
	src/environment.cpp
	src/sprite.cpp
	src/texture.cpp
	src/texture_loader.cpp
	src/shader.cpp
	src/renderer.cpp
	src/index_buffer.cpp
	src/vertex_buffer.cpp
	src/vertex_array.cpp
	src/vertex_buffer_layout.cpp

	include/environment.h
	include/sprite.h
	include/texture.h
	include/texture_loader.h
	include/shader.h
	include/renderer.h
	include/index_buffer.h
	include/vertex_buffer.h
	include/vertex_array.h
	include/vertex.h
	include/transform.h
	include/vertex_buffer_layout.h
)

include_directories(include/ deps/ deps/glfw/include deps/windows/glew/include deps/glm/ deps/stb/ ${BOOST_INCLUDE_DIRS})

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(codingCompanion ${SOURCE_FILES})

add_compile_definitions(GLEW_STATIC)

if(UNIX)
	message("Configuring for UNIX")

	set(GLFW_UNIX_DEPENDENCIES dl pthread)

	find_file(OpenGL REQUIRED)

	target_link_libraries(codingCompanion
		GL
		dl
		${CMAKE_CURRENT_SOURCE_DIR}/deps/glew/libGLEW.a
		${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw/libglfw3.a
		${BOOST_LIBRARIES}
		X11
		Xext
		Xfixes
		)

elseif(WIN32)
	message("Configuring for WIN32")

	find_package(OpenGL REQUIRED)
	target_link_libraries(codingCompanion
		opengl32.lib
		C:/local/glfw/lib-vc2022/glfw3.lib
		${CMAKE_CURRENT_SOURCE_DIR}/deps/windows/glew/lib/Release/x64/glew32s.lib
		${BOOST_LIBRARIES}
		)
endif()
