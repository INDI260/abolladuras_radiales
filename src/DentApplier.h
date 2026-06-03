#ifndef __DentApplier__h__
#define __DentApplier__h__

#include "PreprocessSurface.h"
#include <string>

using namespace PreprocessTypes;

class DentApplier {
public:
  DentApplier(PreprocessSurface &ps) : preprocess(ps) {}
  bool applyDents(const std::string &rbf_type, TReal epsilon = 1.0);

private:
  PreprocessSurface &preprocess;

  static TReal rbfGaussian(TReal r2, TReal eps2);
  static TReal rbfMultiquadric(TReal r, TReal eps);
  static TReal rbfInverseMultiquadric(TReal r, TReal eps);
};

#endif // __DentApplier__h__
