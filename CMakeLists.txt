cmake_minimum_required(VERSION 3.28)
project(arbalesto LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/src)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(game src/main.cpp src/game.cpp src/logger.cpp src/server.cpp src/client.cpp src/game_state.cpp src/player.cpp src/state_man.cpp)

target_compile_features(game PRIVATE cxx_std_17)
target_link_libraries(game PRIVATE SFML::Graphics SFML::Network)