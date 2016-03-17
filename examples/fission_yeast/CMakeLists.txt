add_executable(${PROJECT_NAME}_fission_yeast
               ${CMAKE_CURRENT_SOURCE_DIR}/network.cpp
               ${CMAKE_CURRENT_SOURCE_DIR}/fission_yeast.cpp)
target_link_libraries(${PROJECT_NAME}_fission_yeast inform)
