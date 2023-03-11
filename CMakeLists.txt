cmake_minimum_required(VERSION 3.24)
project(Logic_Sim)

set(CMAKE_CXX_STANDARD 17)

# Set window specific packages
if(WIN32)
    set(SDL2_DIR libs/SDL2-2.26.3/cmake)
    set(SDL2_TTF_DIR libs/SDL2_ttf-2.20.2/cmake)
endif()
set(PUGIXML_DIR libs/pugixml/cmake)
set(IMGUI_DIR libs/imgui/cmake)

# Packages
find_package(SDL2 REQUIRED)
find_package(SDL2_TTF REQUIRED)
find_package(PUGIXML REQUIRED)
find_package(IMGUI REQUIRED)

# Includes
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${PUGIXML_INCLUDE_DIRS})
include_directories(${IMGUI_INCLUDE_DIRS})

# Source files
set(SIM_LOGIC_FILES SimLogic/IONode.cpp SimLogic/Chip.cpp SimLogic/Wire.cpp SimLogic/BasicChips/ChipAND.cpp SimLogic/BasicChips/ChipOR.cpp SimLogic/BasicChips/ChipNOT.cpp SimLogic/ProgrammableChip.cpp SimLogic/Button.cpp)
set(SIM_SERVICE_FILES Services/RandomService.cpp Services/FileService.cpp Services/FileService.h)
set(SIM_MANAGER_FILES SimManagers/SimManager.cpp SimManagers/RenderManager.cpp SimManagers/MouseCollisionManager.cpp SimManagers/SimControlManager.cpp SimManagers/ChipFactory.cpp)

# Create executable
add_executable(${PROJECT_NAME}
        ${SIM_LOGIC_FILES}
        ${SIM_SERVICE_FILES}
        ${SIM_MANAGER_FILES}
        main.cpp)

# Copying res folder to build dir
add_custom_target(
        copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/res
        ${CMAKE_CURRENT_BINARY_DIR}/res
        COMMENT "Copying resource folder to binary directory"
)
add_dependencies(${PROJECT_NAME} copy_resources)

# Linking libraries
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} SDL2_ttf::SDL2_ttf)
target_link_libraries(${PROJECT_NAME} PUGIXML)
target_link_libraries(${PROJECT_NAME} IMGUI)
target_link_libraries(${PROJECT_NAME} -static)
