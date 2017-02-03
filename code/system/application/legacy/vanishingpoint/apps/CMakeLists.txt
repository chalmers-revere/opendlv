add_executable(${PROJECT_NAME} LaneMarkingsVanishingPointDetectionModule.cpp)
target_link_libraries(${PROJECT_NAME} ${VANISHINGPOINT_LIB} ${LIBRARIES})

add_executable(ImageProcessingTestFixture ImageProcessingTestFixture.cpp)
target_link_libraries(ImageProcessingTestFixture ${VANISHINGPOINT_LIB} ${LIBRARIES} stdc++fs)
