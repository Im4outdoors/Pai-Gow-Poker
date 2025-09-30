cmake_minimum_required(VERSION 3.15)
project(pai_gow_engine LANGUAGES CXX)

# ---- Tooling / defaults ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Warnings
if (MSVC)
  add_compile_options(/W4 /permissive- /EHsc)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Always drop exes into build/bin (same dir for all configs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
foreach(cfg IN ITEMS Debug Release RelWithDebInfo MinSizeRel)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfg} ${CMAKE_BINARY_DIR}/bin)
endforeach()

# ---- Library ----
# Add more engine .cpps here if you split files later
set(PAI_GOW_ENGINE_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/engine.cpp
)

# Guard: make sure the expected source exists
if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/engine.cpp")
  message(FATAL_ERROR "Expected engine source not found: engine/src/engine.cpp")
endif()

add_library(pai_gow_engine ${PAI_GOW_ENGINE_SOURCES})
target_include_directories(pai_gow_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# ---- Main demo/driver (optional) ----
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
  add_executable(pai_gow_main src/main.cpp)
  target_link_libraries(pai_gow_main PRIVATE pai_gow_engine)
endif()

# ---- Tests (optional) ----
option(BUILD_TESTS "Build test executable and register with CTest" ON)
if (BUILD_TESTS)
  include(CTest)
  enable_testing()
  set(CTEST_OUTPUT_ON_FAILURE ON)

  # Prefer src/tests.cpp; otherwise allow tests/*.cpp or *test*.cpp
  set(TEST_SOURCES "")
  if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/tests.cpp")
    list(APPEND TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/tests.cpp")
  else()
    file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
         "${CMAKE_CURRENT_SOURCE_DIR}/src/*test*.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
  endif()

  if (NOT TEST_SOURCES)
    message(FATAL_ERROR "BUILD_TESTS=ON but no test sources found (looked for src/tests.cpp, src/*test*.cpp, tests/*.cpp)")
  endif()

  add_executable(pai_gow_tests ${TEST_SOURCES})
  target_link_libraries(pai_gow_tests PRIVATE pai_gow_engine)

  # Register the test; $<TARGET_FILE:...> picks the right EXE for the active config
  add_test(NAME pai_gow_tests COMMAND $<TARGET_FILE:pai_gow_tests>)

  # Config-aware 'check' target (MSVC needs -C <config>)
  if (CMAKE_CONFIGURATION_TYPES)
    add_custom_target(check
      COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG> --output-on-failure -V
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      USES_TERMINAL
      DEPENDS pai_gow_tests)
  else()
    add_custom_target(check
      COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -V
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      USES_TERMINAL
      DEPENDS pai_gow_tests)
  endif()
endif()

# ---- Install (optional) ----
install(TARGETS pai_gow_engine
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
install(DIRECTORY include/ DESTINATION include)
