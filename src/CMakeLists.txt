# CMakeList.txt : CMake project for invasion_game, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (invasion_game "main.cpp"  "game/game_logic.cpp" "algorithms/create_maze.cpp" "game/player.cpp" "utils/inputhandler.cpp" "graphics/renderer.cpp" "utils/maze_renderer.cpp" "game/entity.cpp" "game/invader.cpp" "utils/entity_renderer.cpp")
target_compile_definitions(invasion_game PRIVATE _HAS_STD_BYTE=0)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED) 
find_package(OpenGL REQUIRED)
target_link_libraries(invasion_game PRIVATE
    glfw
    glad::glad
    OpenGL::GL
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET invasion_game PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.


#find_package(glfw3 CONFIG REQUIRED)
#find_package(glad CONFIG REQUIRED)
target_link_libraries(invasion_game PRIVATE glfw opengl32)

target_include_directories(invasion_game PRIVATE ../include)