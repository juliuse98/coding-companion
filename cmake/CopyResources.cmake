# CopyResources.cmake

# Define source and destination directories
set(RESOURCES_SOURCE_DIR ${CMAKE_SOURCE_DIR}/resources)
set(SHADERS_SOURCE_DIR ${RESOURCES_SOURCE_DIR}/shaders)
set(TEXTURES_SOURCE_DIR ${RESOURCES_SOURCE_DIR}/textures)
set(ICONS_SOURCE_DIR ${RESOURCES_SOURCE_DIR}/icons)
set(LOGGER_CONFIG_SOURCE_FILE ${CMAKE_SOURCE_DIR}/easyloggingpp.conf)  # Path to easyloggingpp.conf
set(CC_CONFIG_SOURCE_FILE ${CMAKE_SOURCE_DIR}/cc-config.json)	# Path to cc-config.json

set(RESOURCES_DEST_DIR ${CMAKE_BINARY_DIR}/resources)
set(SHADERS_DEST_DIR ${RESOURCES_DEST_DIR}/shaders)
set(TEXTURES_DEST_DIR ${RESOURCES_DEST_DIR}/textures)
set(ICONS_DEST_DIR ${RESOURCES_DEST_DIR}/icons)
set(LOGGER_CONFIG_DEST_FILE ${CMAKE_BINARY_DIR}/easyloggingpp.conf)  # Destination for easyloggingpp.conf
set(CC_CONFIG_DEST_FILE ${CMAKE_BINARY_DIR}/cc-config.json)      # Destination for cc-config.json

# Helper function to copy resource directories
function(copy_resource_dir SOURCE_DIR DEST_DIR COMMENT_MESSAGE)
    add_custom_command(
        OUTPUT ${DEST_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${DEST_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_DIR} ${DEST_DIR}
        DEPENDS ${SOURCE_DIR}  # You may add specific files for DEPENDS if needed
        COMMENT ${COMMENT_MESSAGE}
    )
endfunction()

# Helper function to copy a single file
function(copy_resource_file SOURCE_FILE DEST_FILE COMMENT_MESSAGE)
    add_custom_command(
        OUTPUT ${DEST_FILE}
        COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_FILE} ${DEST_FILE}
        DEPENDS ${SOURCE_FILE}
        COMMENT ${COMMENT_MESSAGE}
    )
endfunction()

# Copy shaders, textures, and icons
copy_resource_dir(${SHADERS_SOURCE_DIR} ${SHADERS_DEST_DIR} "Copying shader files to the build directory...")
copy_resource_dir(${TEXTURES_SOURCE_DIR} ${TEXTURES_DEST_DIR} "Copying texture files to the build directory...")
copy_resource_dir(${ICONS_SOURCE_DIR} ${ICONS_DEST_DIR} "Copying icon files to the build directory...")
copy_resource_dir(${RESOURCES_SOURCE_DIR}/companions ${RESOURCES_DEST_DIR}/companions "Copying icon files to the build directory...")

# Copy easyloggingpp.conf and cc-config.json file
copy_resource_file(${LOGGER_CONFIG_SOURCE_FILE} ${LOGGER_CONFIG_DEST_FILE} "Copying easyloggingpp.conf to the build directory...")
copy_resource_file(${CC_CONFIG_SOURCE_FILE} ${CC_CONFIG_DEST_FILE} "Copying cc-config.json to the build directory...")

# Custom target to trigger the copying of resources
add_custom_target(copy_resources ALL
    DEPENDS ${SHADERS_DEST_DIR} ${TEXTURES_DEST_DIR} ${ICONS_DEST_DIR} ${LOGGER_CONFIG_DEST_FILE} ${CC_CONFIG_DEST_FILE} ${RESOURCES_DEST_DIR}/companions
)
