cmake_minimum_required(VERSION 3.0)
project(Cat)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)

    message(STATUS "Windows detected")

    set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML/lib/cmake/SFML")
    set(SFML_DLL_DIR "${CMAKE_SOURCE_DIR}/SFML/bin")
    set(ASSET_DIR "${CMAKE_SOURCE_DIR}/Assets")

    find_package(SFML 2.6 REQUIRED graphics window system audio)

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

    message(STATUS "Apple detected")

    find_package(SFML 2.6 REQUIRED graphics window system audio)

    # Add source files
    file(GLOB SOURCES *.cpp)
    add_executable(cat ${SOURCES})

    # Link SFML libraries
    target_link_libraries(cat sfml-graphics sfml-window sfml-system sfml-audio)

endif()