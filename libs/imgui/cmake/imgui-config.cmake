set(IMGUI_ROOT ${CMAKE_CURRENT_LIST_DIR}/../)
add_library(IMGUI STATIC
        ${IMGUI_ROOT}/imgui.cpp
        ${IMGUI_ROOT}/imgui_demo.cpp
        ${IMGUI_ROOT}/imgui_draw.cpp
        ${IMGUI_ROOT}/imgui_widgets.cpp
        ${IMGUI_ROOT}/imgui_tables.cpp
        ${IMGUI_ROOT}/imgui_impl_opengl3.cpp
        ${IMGUI_ROOT}/imgui_stdlib.cpp
        ${IMGUI_ROOT}/imgui_impl_sdl.cpp
        ${IMGUI_ROOT}/imgui_impl_sdlrenderer.cpp)
target_link_libraries(IMGUI ${SDL2_LIBRARIES})
set(IMGUI_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/..)
message("Found TARGET IMGUI at ${CMAKE_CURRENT_LIST_DIR}")

