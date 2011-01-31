set(LIBNAME "MotionTracker")

# Sources
include_directories("${INCDIR}")
file(GLOB SOURCES *.cc ${INCDIR}/*.hh)

# Create a static library
add_library(${LIBNAME} ${SOURCES})
target_link_libraries(${LIBNAME} ${LIBS})

install(TARGETS ${LIBNAME} DESTINATION lib)

