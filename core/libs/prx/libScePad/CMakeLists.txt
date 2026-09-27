add_library(libScePad SHARED EXCLUDE_FROM_ALL
    ${CMAKE_CURRENT_SOURCE_DIR}/Export.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PadState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/InputConfig.cpp
)
target_include_directories(libScePad PRIVATE
    ${LIBS_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/3rdparty/SDL2/include
)
target_link_libraries(libScePad PRIVATE SDL2-static)
set_target_properties(libScePad PROPERTIES CXX_EXTENSIONS OFF)
configure_windows_unwind(libScePad)
