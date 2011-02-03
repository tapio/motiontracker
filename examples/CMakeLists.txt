
list(APPEND LIBS MotionTracker)

# Common sources
include_directories("${INCDIR}")
file(GLOB HEADERS ${INCDIR}/*.hh)

# Extra args for this macro are interpreted as extra libraries
macro(example_app EXENAME)
	add_executable(${EXENAME} ${HEADERS} ${EXENAME}.cc)
	target_link_libraries(${EXENAME} ${LIBS} ${ARGN})
	install(TARGETS ${EXENAME} DESTINATION bin)
endmacro()

example_app(edgefinder)
example_app(calibrator)
example_app(chessboardfinder)
example_app(tracker)
example_app(glchessboardtracker)
example_app(gputest)

