# Add the library source files
file(GLOB LIB_SOURCES quote/*.cpp pricer/*.cpp)
file(GLOB LIB_HEADERS quote/*.hpp pricer/*.hpp)
add_library(peps ${LIB_SOURCES} ${LIB_HEADERS})

# Add the quote executable
add_executable(quote quote/main.cpp)
target_link_libraries(quote peps curl ${LIBS})
