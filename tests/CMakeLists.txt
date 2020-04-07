add_executable(
    runUnitTests
    main.cpp
    NumberAnimationUnitTests.cpp
    EngineSimulationUnitTests.cpp
    LoopAnimationUnitTests.cpp
    ParallelAnimationUnitTests.cpp
)

target_include_directories(
    runUnitTests PUBLIC
    "../build/googletest/googletest-src/googletest/include"
    "../build/googletest/googletest-src/googlemock/include"
)

link_directories(
    runUnitTests PUBLIC
    "../build/lib"
)

target_link_libraries(
    runUnitTests
    gtest
    gmock
    pthread
)

add_test(UnitTests runUnitTests)
