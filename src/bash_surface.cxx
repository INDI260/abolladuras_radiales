
#include <iostream>

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include "PreprocessSurface.h"
#include <pujCGAL/IO.h>

bool debug = true;

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " input.obj dents.txt output.obj"
              << std::endl;
    return (EXIT_FAILURE);
  } // end if

  PreprocessSurface preprocess;
  if (!preprocess.inicialize(argv[1], argv[2])) {
    std::cerr << "Error initializing PreprocessSurface with files: " << argv[1]
              << " and " << argv[2] << std::endl;
    return (EXIT_FAILURE);
  }
  auto &T = preprocess.getDelaunay();
  if (debug) {
    auto mesh = preprocess.getMesh();
    auto &dents = preprocess.getDents();
    std::cout << "Dents read: " << dents.size() << std::endl;
    int i = 0;
    for (const auto &dent : dents) {
      std::cout << "Dent " << ++i << std::endl;
      for (const auto &index : dent.index_v) {
        std::cout << "Dent vertex index: " << index << std::endl;
        std::cout << "Dent vertex position: " << preprocess.getPoint(index)
                  << std::endl;
      }
    }
  }

  preprocess.saveOBJ(argv[3]);
  return (EXIT_SUCCESS);
}

// eof - build_delaunay_from_pointcloud.cxx
