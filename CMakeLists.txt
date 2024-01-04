cmake_minimum_required(VERSION 3.21.3)

# include(/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake)
if(NOT DEFINED VCPKG_ROOT)
    set(VCPKG_ROOT "C:/Users/arsbo/vcpkg")
endif()

set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")


set(SFML_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.5.1)

project(Radio)


# Compile options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find package
find_package(FFMPEG REQUIRED)
find_package(SFML 2.5.1 COMPONENTS system window graphics audio REQUIRED)
find_package(Boost COMPONENTS filesystem REQUIRED)

find_package(Crow CONFIG REQUIRED)

# Sources include
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake_scripts/cmake_scripts.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/sources/sources.cmake)
get_sources(SOURCES_LIST ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${SOURCES_LIST})

#target
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sources)
target_include_directories(${PROJECT_NAME} PRIVATE ${MINIMP3_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-system sfml-audio sfml-network sfml-graphics sfml-window)
target_link_libraries(${PROJECT_NAME} PRIVATE  Boost::boost Boost::filesystem)
target_link_libraries(${PROJECT_NAME} PRIVATE Crow::Crow)

target_include_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FFMPEG_LIBRARIES})

# Output file name settings
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/$<0:>)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX .exe)
