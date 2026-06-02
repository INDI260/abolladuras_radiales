
#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Surface_mesh.h>

#include <pujCGAL/IO.h>

int main( int argc, char** argv )
{
  if( argc < 3 )
  {
    std::cerr << "Usage: " << argv[ 0 ] << " input.obj output.obj" << std::endl;
    return( EXIT_FAILURE );
  } // end if

  using TKernel = CGAL::Exact_predicates_inexact_constructions_kernel;
  using TReal   = typename TKernel::RT;
  using TPoint3 = TKernel::Point_3;
  using TMesh   = CGAL::Surface_mesh< TPoint3 >;

  using TVertices       = CGAL::Triangulation_vertex_base_with_info_2< TReal, TKernel >;
  using TTriangulationDS = CGAL::Triangulation_data_structure_2< TVertices >;
  using TDelaunay       = CGAL::Delaunay_triangulation_2< TKernel, TTriangulationDS >;
  using TPoint2         = TKernel::Point_2;

  // Read 3D point cloud from OBJ (faces are ignored if absent)
  TMesh mesh;
  pujCGAL::IO::read( mesh, argv[ 1 ] );

  // Build Delaunay from (x,y), storing z as vertex height
  TDelaunay T;
  for( auto v : mesh.vertices( ) )
  {
    auto p  = mesh.point( v );
    auto vh = T.insert( TPoint2( p[ 0 ], p[ 1 ] ) );
    vh->info( ) = p[ 2 ];
  } // end for

  pujCGAL::IO::save( T, argv[ 2 ] );
  return( EXIT_SUCCESS );
}

// eof - build_delaunay_from_pointcloud.cxx
