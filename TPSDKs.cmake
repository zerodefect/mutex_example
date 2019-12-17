
include(ExternalProject)

################################################################
# Find Boost.
# This flag may need to change depending on the version of
# Boost installed.
set(Boost_USE_MULTITHREADED ON)

find_package(Boost REQUIRED COMPONENTS system
    )


set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)
include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)
