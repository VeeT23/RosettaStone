# CMakeList.txt : CMake project for RosettaStone, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (RosettaStone "RosettaStone.cpp" "RosettaStone.h")

add_custom_command(TARGET RosettaStone POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Data"
        "$<TARGET_FILE_DIR:RosettaStone>/Data"
    COMMENT "Copying Data directory to output directory"
)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET RosettaStone PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
