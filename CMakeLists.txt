

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/googletest.cmake)
fetch_googletest(
    ${PROJECT_SOURCE_DIR}/cmake
    ${PROJECT_BINARY_DIR}/googletest
)

enable_testing()
add_subdirectory(tests)
