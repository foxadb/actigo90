include_directories(${CMAKE_SOURCE_DIR}/src/pricer)
include_directories(${CMAKE_SOURCE_DIR}/src/quote)

file(GLOB TEST_SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.cpp)
foreach(testsourcefile ${TEST_SOURCES})
string(REPLACE ".cpp" "" testname ${testsourcefile})
    add_executable(${testname} ${testsourcefile})
    target_link_libraries(${testname} peps ${LIBS})
endforeach(testsourcefile ${TEST_SOURCES})
