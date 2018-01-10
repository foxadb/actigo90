# Add the library source files
file(GLOB LIB_SOURCES *.cpp)
file(GLOB LIB_HEADERS *.hpp)
add_library(mc-pricer ${LIB_SOURCES} ${LIB_HEADERS})

# Add the pricer executable
add_executable(pricer pricer.cpp)

# Extra linking for the project
target_link_libraries(pricer mc-pricer ${LIBS})