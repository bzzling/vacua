cmake_minimum_required(VERSION 3.25)
project(vacua LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Static library
add_library(vacua STATIC
  src/vacua.cpp
  src/circuit/circuit.cpp
  src/circuit/library.cpp
  src/cpu/statevector.cpp
  src/cpu/measurements.cpp
  src/cpu/jacobian.cpp
  src/gates/gates.cpp
  src/device/device.cpp
)
target_include_directories(vacua PUBLIC include)

# Tests
include(CTest)
enable_testing()

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1
)
FetchContent_MakeAvailable(Catch2)

add_executable(vacua_tests
  tests/test_circuit.cpp
  tests/test_statevector.cpp
  tests/test_statevector_ops.cpp
  tests/test_device.cpp
)
target_link_libraries(vacua_tests PRIVATE vacua Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(vacua_tests)

# Examples
option(VACUA_BUILD_EXAMPLES "Build example programs" ON)
if(VACUA_BUILD_EXAMPLES)
  foreach(example bell ghz)
    add_executable(${example} examples/cpp/${example}.cpp)
    target_link_libraries(${example} PRIVATE vacua)
    set_target_properties(${example} PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/cpp"
    )
  endforeach()
endif()
