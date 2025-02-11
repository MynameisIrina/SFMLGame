cmake_minimum_required(VERSION 3.0)
project(sf::Cat)

set(CMAKE_CXX_STANDARD 17)

# Add SFML
find_package(SFML 2.6 REQUIRED graphics window system)

# Add source files
file(GLOB SOURCES *.cpp)
add_executable(cat ${SOURCES})


# Link SFML libraries

# include_directories(/opt/homebrew/Cellar/sfml/2.6.1/include)
# link_directories(/opt/homebrew/Cellar/sfml/2.6.1/lib)
target_link_libraries(cat sfml-graphics sfml-window sfml-system sfml-audio)
