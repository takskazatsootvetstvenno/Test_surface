cmake_minimum_required(VERSION 3.12)

set(CORE_PROJECT_NAME ViewerCore)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CORE_INCLUDES
	includes/Application.h
	includes/BufferLayout.h
	includes/Event.h
	includes/IndexBuffer.h
	includes/LogManager.h
	includes/Mesh.h
	includes/ResourceManager.h
	includes/ShaderProgram.h
	includes/UniformBuffer.h
	includes/VertexArray.h
	includes/VertexBuffer.h
	includes/Window.h
	includes/Surface.h
	bindings/imgui_impl_glfw.h
    bindings/imgui_impl_opengl3.h
	bindings/imgui_impl_opengl3_loader.h
)

set(CORE_SOURCES
	sources/Application.cpp
	sources/BufferLayout.cpp
	sources/IndexBuffer.cpp
	sources/LogManager.cpp
	sources/main.cpp
	sources/Mesh.cpp
	sources/ResourceManager.cpp
	sources/ShaderProgram.cpp
	sources/UniformBuffer.cpp
	sources/VertexArray.cpp
	sources/VertexBuffer.cpp
	sources/Window.cpp
	sources/Surface.cpp
	bindings/imgui_impl_glfw.cpp
    	bindings/imgui_impl_opengl3.cpp
)
add_executable(${CORE_PROJECT_NAME}
	${CORE_INCLUDES}
	${CORE_SOURCES}
)
target_compile_features(${CORE_PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(${CORE_PROJECT_NAME} PUBLIC includes)
target_include_directories(${CORE_PROJECT_NAME} PRIVATE src)
target_include_directories(${CORE_PROJECT_NAME} PRIVATE bindings)

target_link_libraries(${CORE_PROJECT_NAME} ${CONAN_LIBS})

add_custom_command(TARGET ${CORE_PROJECT_NAME}
    POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/data/Shaders/Red.frag ${PROJECT_BINARY_DIR}/ViewerCore/Shaders/Red.frag
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/data/Shaders/Red.vert ${PROJECT_BINARY_DIR}/ViewerCore/Shaders/Red.vert
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/data/Shaders/Axes.vert ${PROJECT_BINARY_DIR}/ViewerCore/Shaders/Axes.vert
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/data/Shaders/Axes.frag ${PROJECT_BINARY_DIR}/ViewerCore/Shaders/Axes.frag
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/data/Shaders/Compute.comp ${PROJECT_BINARY_DIR}/ViewerCore/Shaders/Compute.comp
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/data/Shaders/Init.comp ${PROJECT_BINARY_DIR}/ViewerCore/Shaders/Init.comp
)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
	add_compile_definitions(CMAKE_BUILD_TYPE_RELEASE)
	message("Release version" )
else()
	add_compile_definitions(CMAKE_BUILD_TYPE_DEBUG)
	message("Debug version" )
endif()

install(TARGETS ${CORE_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/install)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/data/Shaders DESTINATION ${CMAKE_INSTALL_PREFIX}/install)