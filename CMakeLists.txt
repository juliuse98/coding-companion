cmake_minimum_required(VERSION 3.13)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

include(CopyResources)
include(CreateCmpCmdsLink)

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
	src/ccobject.cpp
	src/window.cpp
	src/sprite_renderer.cpp

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
	include/ccobject.h
	include/window.h
	include/sprite_renderer.h
	include/render_batch.h

	# EasyLogingpp
	deps/easyloggingpp/easylogging++.h
	deps/easyloggingpp/easylogging++.cc
)

include_directories(include/ deps/ deps/glfw/include deps/windows/glew/include deps/glm/ deps/easyloggingpp/ deps/nano-signal-slot/  deps/stb/)

if(WIN32)
    set(APP_ICON_RESOURCE "${CMAKE_SOURCE_DIR}/resources/icons/executable_icon.rc")
    add_executable(codingCompanion ${SOURCE_FILES} ${APP_ICON_RESOURCE})
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
    	set_target_properties(codingCompanion PROPERTIES
    	LINK_FLAGS "-Xlinker /SUBSYSTEM:WINDOWS -Xlinker /ENTRY:mainCRTStartup"
    	)
    	set_target_properties(codingCompanion PROPERTIES 
    	WIN31_EXECUTABLE ON
    	)
	endif()
else()
    add_executable(codingCompanion ${SOURCE_FILES})
endif()

add_dependencies(codingCompanion copy_resources)
add_dependencies(codingCompanion create_compile_commands_link)

add_compile_definitions(GLEW_STATIC)

if(UNIX)
	message("Configuring for UNIX")

	set(CMAKE_C_COMPILER gcc-9)
	set(CMAKE_CXX_COMPILER g++-9)

	set(GLFW_UNIX_DEPENDENCIES dl pthread)

	find_file(OpenGL REQUIRED)

	target_link_libraries(codingCompanion
		GL
		dl
		${GLFW_UNIX_DEPENDENCIES}
		${CMAKE_CURRENT_SOURCE_DIR}/deps/glew/libGLEW.a
		${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw/libglfw3.a
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
		)
endif()

