#include <iostream>

#include "DentApplier.h"
#include "PreprocessSurface.h"

bool debug = true;

int main(int argc, char **argv) {
  if (argc != 5 && argc != 6) {
    std::cerr << "Usage: " << argv[0]
              << " input.obj dents.txt rbf_type output.obj [before.obj]"
              << std::endl;
    return (EXIT_FAILURE);
  }

  PreprocessSurface preprocess;
  if (!preprocess.inicialize(argv[1], argv[2])) {
    std::cerr << "Error initializing PreprocessSurface with files: " << argv[1]
              << " and " << argv[2] << std::endl;
    return (EXIT_FAILURE);
  }

  if (argc == 6) {
    preprocess.saveOBJ(argv[5]);
    std::cout << "Before dents saved: " << argv[5] << std::endl;
  }

  if (debug) {
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

  DentApplier applier(preprocess);
  if (!applier.applyDents(argv[3])) {
    std::cerr << "Unsupported RBF type: " << argv[3] << std::endl;
    return (EXIT_FAILURE);
  }

  preprocess.saveOBJ(argv[4]);
  return (EXIT_SUCCESS);
}
