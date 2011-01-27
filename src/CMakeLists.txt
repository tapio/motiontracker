set(LIBNAME "MotionTracker")

# Sources
include_directories("${CMAKE_SOURCE_DIR}/include")
file(GLOB SOURCES *.cc)

# Create a static library
add_library(${LIBNAME} STATIC ${SOURCES})

install(TARGETS ${LIBNAME} DESTINATION lib)

