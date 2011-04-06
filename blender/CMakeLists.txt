set(LIBNAME "PyMotionTracker")

# Sources
include_directories("${INCDIR}")
file(GLOB SOURCES *.cc *.hh ${INCDIR}/*.hh)

# Create a shared library
add_library(${LIBNAME} SHARED ${SOURCES})
target_link_libraries(${LIBNAME} ${LIBS})

install(TARGETS ${LIBNAME} DESTINATION lib)

