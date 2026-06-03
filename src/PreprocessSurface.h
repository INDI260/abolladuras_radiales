#ifndef __PreprocessSurface__h__
#define __PreprocessSurface__h__

#include <CGAL/Circle_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

namespace PreprocessTypes {

using TKernel = CGAL::Exact_predicates_inexact_constructions_kernel;
using TReal = typename TKernel::RT;
using TPoint3 = TKernel::Point_3;
using TPoint2 = TKernel::Point_2;
using TCircle2 = CGAL::Circle_2<TKernel>;
using TMesh = CGAL::Surface_mesh<TPoint3>;

struct VertexData {
  std::size_t idx;
  TReal z;
};

using TVertices =
    CGAL::Triangulation_vertex_base_with_info_2<VertexData, TKernel>;
using TTriangulationDS = CGAL::Triangulation_data_structure_2<TVertices>;
using TDelaunay = CGAL::Delaunay_triangulation_2<TKernel, TTriangulationDS>;

struct Dent {
  TCircle2 area;
  TReal intensity;
  std::vector<std::size_t> index_v;
};
} // namespace PreprocessTypes

class PreprocessSurface {
public:
  using TKernel = PreprocessTypes::TKernel;
  using TReal = PreprocessTypes::TReal;
  using TPoint3 = PreprocessTypes::TPoint3;
  using TPoint2 = PreprocessTypes::TPoint2;
  using TCircle2 = PreprocessTypes::TCircle2;
  using TMesh = PreprocessTypes::TMesh;
  using Dent = PreprocessTypes::Dent;
  using TVertices = PreprocessTypes::TVertices;
  using TDelaunay = PreprocessTypes::TDelaunay;

  PreprocessSurface() = default;
  bool readMesh(const std::string &filename);
  bool readDents(const std::string &filename);
  void collectDentPoints();
  void buildDelaunay();
  bool inicialize(const std::string &mesh_filename,
                  const std::string &dents_filename);
  bool saveOBJ(const std::string &filename);
  TMesh &getMesh() { return mesh; }
  TDelaunay &getDelaunay() { return T; }
  std::vector<Dent> &getDents() { return dents; }
  TReal getZ(std::size_t idx) {
    return mesh.point(typename TMesh::Vertex_index(idx)).z();
  }
  TPoint3 getPoint(std::size_t idx) {
    return mesh.point(typename TMesh::Vertex_index(idx));
  }
  std::size_t getMeshIndex(typename TDelaunay::Vertex_handle vh) {
    return vh->info().idx;
  }

private:
  TMesh mesh;
  TDelaunay T;
  std::vector<Dent> dents;
};

#endif // __PreprocessSurface__h__