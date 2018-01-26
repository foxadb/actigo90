# Add the library source files
file(GLOB LIB_SOURCES pricer/*.cpp quote/*.cpp)
file(GLOB LIB_HEADERS pricer/*.hpp quote/*.hpp)
add_library(peps ${LIB_SOURCES} ${LIB_HEADERS})


# Add the quote executable
add_executable(quote quote/main.cpp)
target_link_libraries(quote peps curl ${LIBS})
