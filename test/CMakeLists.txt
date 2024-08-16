cmake_minimum_required(VERSION 3.14)
project ("mGUI-test")

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Install CTest
include(CTest)

# Install GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
  ${PROJECT_NAME}
  mgui_test.cc
)
target_link_libraries(
  ${PROJECT_NAME}
  GTest::gtest_main
  GTest::gmock_main
)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
enable_testing()
