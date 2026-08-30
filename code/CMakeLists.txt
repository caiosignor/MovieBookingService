# ─────────────────────────────────────────────────────────────────────────────
# BookingServiceCore — domain + application logic
# ─────────────────────────────────────────────────────────────────────────────
file(GLOB_RECURSE BOOKING_SERVICE_CORE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/MovieSessionDatabase/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Application/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Infrastructure/*.cpp"
)

if(BOOKING_SERVICE_CORE_SOURCES)
    add_library(BookingServiceCore STATIC ${BOOKING_SERVICE_CORE_SOURCES})
else()
    add_library(BookingServiceCore INTERFACE)
endif()

set(BOOKING_SERVICE_INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/code/include"
    "${CMAKE_SOURCE_DIR}/code/include/Application"
    "${CMAKE_SOURCE_DIR}/code/include/MovieSessionDatabase"
    "${CMAKE_SOURCE_DIR}/code/include/MovieSessionDatabase/Operations"
    "${CMAKE_SOURCE_DIR}/code/include/MovieSessionDatabase/Operations/Set"
    "${CMAKE_SOURCE_DIR}/code/include/MovieSessionDatabase/Operations/Get"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/MovieSessionDatabase"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/MovieSessionDatabase/Operations"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/MovieSessionDatabase/Operations/Set"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/MovieSessionDatabase/Operations/Get"
)

if(BOOKING_SERVICE_CORE_SOURCES)
    target_include_directories(BookingServiceCore
        PUBLIC
            ${BOOKING_SERVICE_INCLUDE_DIRS}
    )
    target_compile_features(BookingServiceCore PUBLIC cxx_std_23)
    target_link_libraries(BookingServiceCore
        PUBLIC
            project_warnings
            project_sanitizers
    )
else()
    target_include_directories(BookingServiceCore
        INTERFACE
            ${BOOKING_SERVICE_INCLUDE_DIRS}
    )
    target_compile_features(BookingServiceCore INTERFACE cxx_std_23)
    target_link_libraries(BookingServiceCore
        INTERFACE
            project_warnings
            project_sanitizers
    )
endif()

# ─────────────────────────────────────────────────────────────────────────────
# BookingServiceExecutable — entry point executable
# ─────────────────────────────────────────────────────────────────────────────
add_executable(BookingServiceExecutable
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Application/main.cpp"
)

target_compile_features(BookingServiceExecutable PRIVATE cxx_std_23)

target_link_libraries(BookingServiceExecutable
    PRIVATE
        BookingServiceCore
        project_warnings
        project_sanitizers
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_options(BookingServiceExecutable PRIVATE -static-libstdc++ -static-libgcc)
endif()
