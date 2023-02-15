
add_custom_target(typhon_core_lib
    COMMAND $<TARGET_FILE:tyc>
    DEPENDS tyc
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
