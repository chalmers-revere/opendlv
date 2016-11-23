project(opendlv-system-application-legacy-experimentation)

set(EXPERIMENTATION_LIB "${PROJECT_NAME}lib-static")

include_directories(include)

add_subdirectory(apps)
add_subdirectory(src)
add_subdirectory(testsuites)