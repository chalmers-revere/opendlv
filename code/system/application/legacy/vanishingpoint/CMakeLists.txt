project(opendlv-system-application-legacy-vanishingpoint)

set(VANISHINGPOINT_LIB "${PROJECT_NAME}lib-static")

include_directories(include)

add_subdirectory(apps)
add_subdirectory(src)
