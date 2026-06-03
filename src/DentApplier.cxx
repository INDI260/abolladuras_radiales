#include "DentApplier.h"
#include <cmath>

TReal DentApplier::rbfGaussian(TReal r2, TReal eps2) {
  return std::exp(-eps2 * r2);
}

TReal DentApplier::rbfMultiquadric(TReal r, TReal eps) {
  return std::sqrt(1 + std::pow(eps * r, 2));
}

TReal DentApplier::rbfInverseMultiquadric(TReal r, TReal eps) {
  return 1 / std::sqrt(1 + std::pow(eps * r, 2));
}

bool DentApplier::applyDents(const std::string &rbf_type, TReal epsilon) {
  auto &mesh = preprocess.getMesh();
  auto &dents = preprocess.getDents();
  TReal eps2 = epsilon * epsilon;

  for (auto &dent : dents) {
    TPoint2 center = dent.area.center();
    TReal R = CGAL::sqrt(dent.area.squared_radius());
    TReal invR2 = 1.0 / (R * R);

    for (auto idx : dent.index_v) {
      TPoint3 p = preprocess.getPoint(idx);
      TReal r2 = CGAL::squared_distance(TPoint2(p[0], p[1]), center);
      TReal t2 = r2 * invR2;

      TReal w = 0;
      if (rbf_type == "gaussian")
        w = rbfGaussian(t2, eps2);
      else if (rbf_type == "multiquadric")
        w = rbfMultiquadric(std::sqrt(t2), epsilon);
      else if (rbf_type == "inverse_multiquadric")
        w = rbfInverseMultiquadric(std::sqrt(t2), epsilon);
      else
        return false;

      TReal new_z = p[2] - dent.intensity * w;
      mesh.point(typename TMesh::Vertex_index(idx)) =
          TPoint3(p[0], p[1], new_z);
    }
  }
  return true;
}
