# Add the library source files
file(GLOB LIB_SOURCES pricer/*.cpp quote/*.cpp)
file(GLOB LIB_HEADERS pricer/*.hpp quote/*.hpp)
add_library(mc-pricer ${LIB_SOURCES} ${LIB_HEADERS})

# Add the pricer executable
add_executable(pricer pricer/pricer.cpp)

# Extra linking for the project
target_link_libraries(pricer mc-pricer ${LIBS})