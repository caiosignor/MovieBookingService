cmake_minimum_required(VERSION 3.20)
project(TheaterBookingSoftware VERSION 0.1.0 LANGUAGES CXX)

# ─────────────────────────────────────────────────────────────────────────────
# Global C++ standard
# ─────────────────────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ─────────────────────────────────────────────────────────────────────────────
# Output directories
# ─────────────────────────────────────────────────────────────────────────────
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) 

# ─────────────────────────────────────────────────────────────────────────────
# Sanitizer options
# ─────────────────────────────────────────────────────────────────────────────
option(ENABLE_ASAN  "Enable AddressSanitizer"         OFF)
option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)
option(ENABLE_TSAN  "Enable ThreadSanitizer"          OFF)

if(ENABLE_ASAN AND ENABLE_TSAN)
    message(FATAL_ERROR "AddressSanitizer and ThreadSanitizer cannot be used together.")
endif()

# ─────────────────────────────────────────────────────────────────────────────
# Compiler warning interface target (shared by all targets)
# ─────────────────────────────────────────────────────────────────────────────
add_library(project_warnings INTERFACE)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(project_warnings INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2
        -Wimplicit-fallthrough
    )
elseif(MSVC)
    target_compile_options(project_warnings INTERFACE /W4 /WX)
endif()

# ─────────────────────────────────────────────────────────────────────────────
# Sanitizer interface target
# ─────────────────────────────────────────────────────────────────────────────
add_library(project_sanitizers INTERFACE)

if(ENABLE_ASAN)
    message(STATUS "AddressSanitizer enabled")
    target_compile_options(project_sanitizers INTERFACE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(project_sanitizers INTERFACE -fsanitize=address)
endif()

if(ENABLE_UBSAN)
    message(STATUS "UndefinedBehaviorSanitizer enabled")
    target_compile_options(project_sanitizers INTERFACE -fsanitize=undefined)
    target_link_options(project_sanitizers INTERFACE -fsanitize=undefined)
endif()

if(ENABLE_TSAN)
    message(STATUS "ThreadSanitizer enabled")
    target_compile_options(project_sanitizers INTERFACE -fsanitize=thread)
    target_link_options(project_sanitizers INTERFACE -fsanitize=thread)
endif()

# ─────────────────────────────────────────────────────────────────────────────
# Subdirectories
# ─────────────────────────────────────────────────────────────────────────────
add_subdirectory(code)

include(CTest)
if(BUILD_TESTING)
     add_subdirectory(tests)
endif()

option(BUILD_DOCUMENTATION "Create the Doxygen documentation target" OFF)
if(BUILD_DOCUMENTATION)
    find_package(Doxygen REQUIRED)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating Doxygen documentation"
        VERBATIM
    )
endif()
