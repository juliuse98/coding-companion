# CopyResources.cmake

set(RESOURCES_SOURCE_DIR ${CMAKE_SOURCE_DIR}/resources)
set(SHADERS_SOURCE_DIR ${RESOURCES_SOURCE_DIR}/shaders)
set(TEXTURES_SOURCE_DIR ${RESOURCES_SOURCE_DIR}/textures)

set(SHADERS_DEST_DIR ${CMAKE_BINARY_DIR}/resources/shaders)
set(TEXTURES_DEST_DIR ${CMAKE_BINARY_DIR}/resources/textures)

add_custom_command(
    OUTPUT ${SHADERS_DEST_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADERS_SOURCE_DIR} ${SHADERS_DEST_DIR}
    DEPENDS ${SHADERS_SOURCE_DIR}
    COMMENT "Copying shader files to the build directory..."
)

add_custom_command(
    OUTPUT ${TEXTURES_DEST_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${TEXTURES_SOURCE_DIR} ${TEXTURES_DEST_DIR}
    DEPENDS ${TEXTURES_SOURCE_DIR}
    COMMENT "Copying texture files to the build directory..."
)

add_custom_target(copy_resources ALL
    DEPENDS ${SHADERS_DEST_DIR} ${TEXTURES_DEST_DIR}
)
