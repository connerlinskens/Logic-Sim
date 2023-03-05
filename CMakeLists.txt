cmake_minimum_required(VERSION 3.24)
project(Logic_Sim)

set(CMAKE_CXX_STANDARD 17)

# Set window specific packages
if(WIN32)
    set(SDL2_DIR libs/SDL2-2.26.3/cmake)
    set(SDL2_TTF_DIR libs/SDL2_ttf-2.20.2/cmake)
endif()

# Packages
find_package(SDL2 REQUIRED)
find_package(SDL2_TTF REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

# Source files
set(SIM_LOGIC_FILES SimLogic/IONode.cpp SimLogic/Chip.cpp SimLogic/Wire.cpp SimLogic/BasicChips/ChipAND.cpp SimLogic/BasicChips/ChipOR.cpp SimLogic/BasicChips/ChipNOT.cpp SimLogic/ProgrammableChip.cpp)
set(SIM_MANAGER_FILES SimManagers/SimManager.cpp SimManagers/RenderManager.cpp SimManagers/MouseCollisionManager.cpp SimManagers/SimControlManager.cpp)

# Create executable
add_executable(${PROJECT_NAME}
        ${SIM_LOGIC_FILES}
        ${SIM_MANAGER_FILES}
        main.cpp)

# Linking libraries
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} SDL2_ttf::SDL2_ttf)