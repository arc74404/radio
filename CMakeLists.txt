cmake_minimum_required(VERSION 3.21.3)

set(CMAKE_TOOLCHAIN_FILE "C:/Users/arsbo/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(KusServer)

find_package(SFML COMPONENTS audio system window graphics CONFIG REQUIRED)


find_package(Boost COMPONENTS filesystem REQUIRED)


find_path(MINIMP3_INCLUDE_DIRS "minimp3/minimp3.h")


# Sources include
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake_scripts/cmake_scripts.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/sources/sources.cmake)
get_sources(SOURCES_LIST ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(${PROJECT_NAME} ${SOURCES_LIST})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sources)
target_include_directories(${PROJECT_NAME} PRIVATE ${MINIMP3_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-system sfml-audio sfml-network sfml-graphics sfml-window)


target_link_libraries(${PROJECT_NAME} PRIVATE  Boost::boost Boost::filesystem)





# Output file name settings
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/$<0:>)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX .exe)
