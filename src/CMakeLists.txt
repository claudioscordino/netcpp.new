include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/include/${NETCPP_PLATFORM})

# Files to be compiled:
add_library(${CMAKE_PROJECT_NAME}_static STATIC abstract_socket.cpp logger.cpp ${NETCPP_PLATFORM}/posix_socket.cpp)
add_library(${CMAKE_PROJECT_NAME} SHARED abstract_socket.cpp logger.cpp ${NETCPP_PLATFORM}/posix_socket.cpp)

# Install file myproject. 
# The destination directory is relative to CMAKE_INSTALL_PREFIX:
install (TARGETS ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_static DESTINATION .)
