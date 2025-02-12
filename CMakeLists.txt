# cmake_minimum_required(VERSION 3.10)  # Update to avoid deprecation warning
# project(sf::Cat)

# set(CMAKE_CXX_STANDARD 17)

# # Find SFML
# find_package(SFML 2.6 REQUIRED graphics window system audio)

# # Add source files
# file(GLOB SOURCES *.cpp)
# add_executable(cat ${SOURCES})

# # Link SFML libraries
# target_link_libraries(cat sfml-graphics sfml-window sfml-system sfml-audio)

# # Copy assets directory after build (no dependency on "cat" to avoid cycles)
# add_custom_target(copy_assets
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
# )

# # Ensure assets are copied when building (without circular dependency)
# add_dependencies(copy_assets cat)


cmake_minimum_required(VERSION 3.0)
project(Cat)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
# Укажите путь к папке с DLL SFML
set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML/lib/cmake/SFML")
set(SFML_DLL_DIR "${CMAKE_SOURCE_DIR}/SFML/bin")
set(ASSET_DIR "${CMAKE_SOURCE_DIR}/Assets")


find_package(SFML 2.6 REQUIRED graphics window system)


file(GLOB SOURCES *.cpp)


add_executable(cat ${SOURCES})
target_link_libraries(cat sfml-graphics sfml-window sfml-system sfml-audio)


add_custom_command(TARGET cat POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SFML_DLL_DIR}
        $<TARGET_FILE_DIR:cat>
)

add_custom_command(TARGET cat POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSET_DIR}
        $<TARGET_FILE_DIR:cat>/Assets
)

else(APPLE)
    # Other platform-specific settings can be handled here
    message(STATUS "Apple detected.")
    # cmake_minimum_required(VERSION 3.10)  # Update to avoid deprecation warning
project(sf::Cat)

set(CMAKE_CXX_STANDARD 17)

# Find SFML
find_package(SFML 2.6 REQUIRED graphics window system audio)

# Add source files
file(GLOB SOURCES *.cpp)
add_executable(cat ${SOURCES})

# Link SFML libraries
target_link_libraries(cat sfml-graphics sfml-window sfml-system sfml-audio)
endif()