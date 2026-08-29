add_executable(DatabaseTests
    DatabaseTest.cpp
)

target_link_libraries(DatabaseTests
    PRIVATE
        TheaterCore
        project_warnings
        project_sanitizers
)

target_compile_features(DatabaseTests PRIVATE cxx_std_23)

add_test(NAME DatabaseTests COMMAND DatabaseTests)
