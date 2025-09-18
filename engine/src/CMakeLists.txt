cmake_minimum_required(VERSION 3.15)
project(pai_gow_engine LANGUAGES CXX)

# Use modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Library target: contains your engine logic
add_library(pai_gow_engine
  src/engine.cpp
)

target_include_directories(pai_gow_engine
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# CLI target: small executable that calls into the engine
add_executable(pai_gow_cli
  src/cli.cpp
)

target_link_libraries(pai_gow_cli PRIVATE pai_gow_engine)

# Optional: installation (so "make install" works)
install(TARGETS pai_gow_engine pai_gow_cli
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
install(DIRECTORY include/ DESTINATION include)
