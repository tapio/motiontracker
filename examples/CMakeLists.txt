set(EXENAME "tracker")

list(APPEND LIBS MotionTracker)

# Sources
include_directories("${INCDIR}")
file(GLOB SOURCES *.cc ${INCDIR}/*.hh)

add_executable(${EXENAME} ${SOURCES})
target_link_libraries(${EXENAME} ${LIBS})

install(TARGETS ${EXENAME} DESTINATION bin)

