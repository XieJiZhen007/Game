cmake_minimum_required(VERSION 3.1)
project(Snake VERSION 1.0)

# set output path
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#find package
find_package(SDL2 REQUIRED)
# find_package(SDL2_image REQUIRED)

set(BIN_NAME snake)

file(GLOB SRC src/*.cpp)

add_executable(${BIN_NAME} ${SRC})

target_include_directories(${BIN_NAME} PUBLIC src)
target_include_directories(${BIN_NAME} PUBLIC ${SDL2_INCLUDE_DIR})
# target_link_libraries(${BIN_NAME} SDL2::Main SDL2::Image)
target_link_libraries(${BIN_NAME} SDL2 SDL2main SDL2_image)