cmake_minimum_required(VERSION 2.8)
project(mc-pricer CXX)

# Prefix
if (NOT PREFIX)
    set(PREFIX ${CMAKE_CURRENT_BINARY_DIR})
endif (NOT PREFIX)
set(CMAKE_INSTALL_PREFIX ${PREFIX} CACHE STRING "Installation prefix." FORCE)

if (CXX)
    set(CMAKE_CXX_COMPILER ${CXX})
endif (CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# Release or Debug
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif ()

## Detect Boost
set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
message(STATUS "Boost include directory ${Boost_INCLUDE_DIR}")

## Detect PNL

# Set PNL directory
#set(Pnl_DIR "../pnl/build/lib/pnl")

find_package(Pnl REQUIRED)
set(LIBS ${LIBS} ${PNL_LIBRARIES})
include_directories(${PNL_INCLUDE_DIRS})
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_definitions(-DPNL_RANGE_CHECK_OFF)
endif()

# Add math lib
set(LIBS ${LIBS} m)

# Testing the parser
add_executable(parser-test src/parser.cpp src/test_parser.cpp  src/MonteCarlo.cpp  src/Basket.cpp src/BlackScholesModel.cpp)
target_link_libraries(parser-test ${LIBS})
add_executable(positifT-test src/testPositifT.cpp  src/MonteCarlo.cpp  src/Basket.cpp src/BlackScholesModel.cpp)
target_link_libraries(positifT-test ${LIBS})
add_executable(mc-example src/mc_example.cpp  src/MonteCarlo.cpp  src/Basket.cpp src/BlackScholesModel.cpp)
target_link_libraries(mc-example ${LIBS})
add_executable(test_basket1 src/test_basket_1.cpp src/MonteCarlo.cpp  src/Basket.cpp src/BlackScholesModel.cpp )
target_link_libraries(test_basket1 ${LIBS} )
add_executable(test_basket2 src/test_basket_2.cpp src/MonteCarlo.cpp  src/Basket.cpp src/BlackScholesModel.cpp )
target_link_libraries(test_basket2 ${LIBS} )
add_executable(test_perf src/test_perf.cpp src/MonteCarlo.cpp  src/Performance.cpp src/BlackScholesModel.cpp )
target_link_libraries(test_perf ${LIBS} )
add_executable(test_asian src/test_asian.cpp src/MonteCarlo.cpp  src/Asian.cpp src/BlackScholesModel.cpp )
target_link_libraries(test_asian ${LIBS} )
add_executable(testDelta src/testDelta.cpp  src/MonteCarlo.cpp  src/Basket.cpp src/BlackScholesModel.cpp)
target_link_libraries(testDelta ${LIBS})
add_executable(testPL src/testPL.cpp  src/MonteCarlo.cpp  src/Basket.cpp src/BlackScholesModel.cpp)
target_link_libraries(testPL ${LIBS})
add_executable(testBasketPriceT src/test_basket_priceT.cpp src/Basket.cpp src/MonteCarlo.cpp  src/BlackScholesModel.cpp)
target_link_libraries( testBasketPriceT ${LIBS})

add_executable(pricer src/parser.cpp src/pricer.cpp src/Basket.cpp src/MonteCarlo.cpp  src/BlackScholesModel.cpp src/Asian.cpp  src/Performance.cpp)
target_link_libraries(pricer ${LIBS})
#install(TARGETS mc-pricer DESTINATION bin)

# Print compiler flags
get_directory_property(DirDefs COMPILE_DEFINITIONS)
message(STATUS "COMPILE_DEFINITIONS = ${DirDefs}")
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_LINKER_EXE_FLAGS}" )
else()
    message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_LINKER_EXE_FLAGS}" )
endif()
