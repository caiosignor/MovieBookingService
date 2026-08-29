if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/DatabaseTest.cpp")
    add_executable(DatabaseTests
        DatabaseTest.cpp
    )

    target_link_libraries(DatabaseTests
        PRIVATE
            BookingServiceCore
            project_warnings
            project_sanitizers
    )

    target_compile_features(DatabaseTests PRIVATE cxx_std_23)

    add_test(NAME DatabaseTests COMMAND DatabaseTests)
else()
    message(STATUS "No database test source found; skipping DatabaseTests target.")
endif()
