include_directories(${CMAKE_SOURCE_DIR}/src)

file(GLOB TEST_SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.cpp)
foreach(testsourcefile ${TEST_SOURCES})
string(REPLACE ".cpp" "" testname ${testsourcefile})
    add_executable(${testname} ${testsourcefile})
    target_link_libraries(${testname} mc-pricer ${LIBS})
endforeach(testsourcefile ${TEST_SOURCES})