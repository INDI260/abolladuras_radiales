#include "PreprocessSurface.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <pujCGAL/IO.h>

bool PreprocessSurface::readMesh(const std::string &filename) {
  return (pujCGAL::IO::read(mesh, filename));
}

bool PreprocessSurface::readDents(const std::string &filename) {
  std::ifstream file(filename);
  std::string line;

  if (!file.is_open()) {
    std::cerr << "Error opening dents file: " << filename << std::endl;
    return false;
  }

  while (std::getline(file, line)) {
    if (line.empty())
      continue;

    std::stringstream ss(line);
    std::string token;

    // Read center field: "x,y" (z is ignored)
    std::getline(ss, token, ';');
    std::stringstream ss_center(token);
    double x, y;
    char comma;
    ss_center >> x >> comma >> y;

    // Read radius
    std::getline(ss, token, ';');
    double radius = std::stod(token);
    double radius2 = radius * radius;

    // Read intensity
    std::getline(ss, token, ';');
    double intensity = std::stod(token);

    Dent dent;
    dent.area = TCircle2(TPoint2(x, y), radius2);
    dent.intensity = intensity;
    dents.push_back(dent);
  }

  file.close();
  return true;
}

void PreprocessSurface::collectDentPoints() {
  // Compute euclidean distance from the center of each dent to all vertices
  for (auto &dent : dents) {
    for (auto v : mesh.vertices()) {
      auto p = mesh.point(v);
      TPoint2 p2(p[0], p[1]);
      if (CGAL::squared_distance(p2, dent.area.center()) <=
          dent.area.squared_radius()) {
        // Collect the vertex index if it is within the dent area
        dent.index_v.push_back((v.idx()));
      }
    }
  }
}

void PreprocessSurface::buildDelaunay() {
  // Build Delaunay from (x,y), storing mesh index and z height
  for (auto v : mesh.vertices()) {
    auto p = mesh.point(v);
    auto vh = T.insert(TPoint2(p[0], p[1]));
    vh->info() = {v.idx(), p[2]};
  }
}

bool PreprocessSurface::saveOBJ(const std::string &filename) {
  std::stringstream out;
  std::size_t i = 0;

  for (auto v = T.finite_vertices_begin(); v != T.finite_vertices_end(); ++v) {
    auto z = v->info().z;
    v->info().idx = ++i;
    out << "v " << v->point().x() << " " << v->point().y() << " " << z
        << std::endl;
  }

  for (auto f = T.finite_faces_begin(); f != T.finite_faces_end(); ++f) {
    out << "f "
        << f->vertex(0)->info().idx << " "
        << f->vertex(1)->info().idx << " "
        << f->vertex(2)->info().idx << std::endl;
  }

  std::ofstream fout(filename);
  if (!fout) return false;
  fout << out.str();
  return true;
}

bool PreprocessSurface::inicialize(const std::string &mesh_filename,
                                   const std::string &dents_filename) {
  if (!readMesh(mesh_filename)) {
    std::cerr << "Failed to read mesh from file: " << mesh_filename
              << std::endl;
    return false;
  }

  if (!readDents(dents_filename)) {
    return false;
  }

  collectDentPoints();
  buildDelaunay();

  return true;
}