cmake_minimum_required(VERSION 3.15)
project(pai_gow_engine LANGUAGES CXX)

# ---- Tooling / defaults ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Always drop exes into build/bin (no config subdirs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Make MSVC multi-config also use the same bin path
foreach(cfg IN ITEMS Debug Release RelWithDebInfo MinSizeRel)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfg} ${CMAKE_BINARY_DIR}/bin)
endforeach()

# ---- Library ----
add_library(pai_gow_engine src/engine.cpp)
target_include_directories(pai_gow_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# ---- Main demo/driver ----
add_executable(pai_gow_main src/main.cpp)
target_link_libraries(pai_gow_main PRIVATE pai_gow_engine)

# ---- Tests (optional) ----
option(BUILD_TESTS "Build test executable and register with CTest" ON)
if (BUILD_TESTS)
  add_executable(pai_gow_tests src/tests.cpp)
  target_link_libraries(pai_gow_tests PRIVATE pai_gow_engine)

  enable_testing()
  # Use generator expression so CTest runs the right binary path
  add_test(NAME pai_gow_tests COMMAND $<TARGET_FILE:pai_gow_tests>)

  # Config-aware 'check' target (MSVC needs -C <config>)
  if (CMAKE_CONFIGURATION_TYPES)
    add_custom_target(check
      COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG> --output-on-failure -V
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      USES_TERMINAL
    )
  else()
    add_custom_target(check
      COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -V
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      USES_TERMINAL
    )
  endif()
endif()

# ---- Install (optional) ----
install(TARGETS pai_gow_engine pai_gow_main
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
install(DIRECTORY include/ DESTINATION include)

