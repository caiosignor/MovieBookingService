if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Database/DatabaseTest.cpp")
    add_executable(DatabaseTests
        Database/DatabaseTest.cpp
    )

    target_include_directories(DatabaseTests
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
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

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ProtocolDecoder/ProtocolDecoderTests.cpp")
    add_executable(ProtocolDecoderTests
        ProtocolDecoder/ProtocolDecoderTests.cpp
    )

    target_include_directories(ProtocolDecoderTests
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    target_link_libraries(ProtocolDecoderTests
        PRIVATE
            BookingServiceCore
            project_warnings
            project_sanitizers
    )

    target_compile_features(ProtocolDecoderTests PRIVATE cxx_std_23)

    add_test(NAME ProtocolDecoderTests COMMAND ProtocolDecoderTests)
else()
    message(STATUS "No protocol decoder test source found; skipping ProtocolDecoderTests target.")
endif()
