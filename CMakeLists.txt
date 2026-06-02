## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

# -- Basic configuration
cmake_minimum_required(VERSION 3.10)
project(DelaunayDecimation)
include("./baseConf.cmake")

include_directories(
  ${CMAKE_BINARY_DIR}/lib
  ${CMAKE_SOURCE_DIR}/lib
  )

# -- Packages
find_package(CGAL REQUIRED COMPONENTS Core)

# -- Executables
add_executable(pujCGAL_build_delaunay_from_pointcloud src/build_delaunay_from_pointcloud.cxx)
target_link_libraries(pujCGAL_build_delaunay_from_pointcloud CGAL::CGAL)

## eof - CMakeLists.txt
