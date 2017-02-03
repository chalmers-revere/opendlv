file(GLOB thisproject-sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(${VANISHINGPOINT_LIB} STATIC ${thisproject-sources})
