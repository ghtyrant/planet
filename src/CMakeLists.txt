file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
add_executable(${EXECUTABLE_NAME} ${SOURCES})

find_package(SFML 2.1 REQUIRED system window graphics)
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
find_package(SFGUI 0.2.0 REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} ${SFGUI_LIBRARY})

execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ../../media ${CMAKE_BINARY_DIR}/src/media)
