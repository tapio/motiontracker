set(EXENAME "tracker")

list(APPEND LIBS MotionTracker)

# Sources
include_directories("${CMAKE_SOURCE_DIR}/include")
file(GLOB SOURCES "*.cc")

add_executable(${EXENAME} ${SOURCES})
target_link_libraries(${EXENAME} ${LIBS})

