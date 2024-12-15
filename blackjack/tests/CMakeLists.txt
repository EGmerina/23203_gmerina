cmake_minimum_required(VERSION 3.5.0)
project(play VERSION 0.1.0 LANGUAGES C CXX)

set (CMAKE_CXX_FLAGS "-Wall -gtest")

set(CMAKE_CXX_FLAGS "-lboost_program_options")


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost CONFIG COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
enable_testing()
add_subdirectory(googletest)
add_executable(test test.cpp ../players/nina.cpp ../players/yan.cpp ../players/masha.cpp ../players/vasya.cpp ../players/petya.cpp ../players/dealer.cpp ../modes/mode.cpp ../deckes/deck.cpp ../deckes/ndeck.cpp ../deckes/simpledeck.cpp ../modes/detailedmode.cpp ../modes/fastmode.cpp ../modes/tournamentmode.cpp)

target_compile_options(test PRIVATE -Wall -Wextra -g) #-fsanitize=address)
#target_link_options(play PRIVATE -lgcov --coverage) #-fsanitize=address)
target_link_libraries(test ${Boost_LIBRARIES} gtest_main gmock_main)
