cmake_minimum_required(VERSION 3.0)
project(Game)

set(CMAKE_CXX_STANDARD 17)

# Add SFML
find_package(SFML 2.6 REQUIRED graphics window system)

# Add source files
file(GLOB SOURCES SFMLGame/*.cpp)
add_executable(game ${SOURCES})


# Link SFML libraries

# include_directories(/opt/homebrew/Cellar/sfml/2.6.1/include)
# link_directories(/opt/homebrew/Cellar/sfml/2.6.1/lib)
target_link_libraries(game sfml-graphics sfml-window sfml-system sfml-audio)