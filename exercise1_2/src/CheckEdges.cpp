#include "CheckEdges.h"
#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <cmath>


bool is_edge_flip_ok(HEMesh &m, OpenMesh::EdgeHandle eh)
{
    if (m.is_boundary(eh))
        return false;

    auto heh0 = m.halfedge_handle(eh, 0);
    auto heh1 = m.halfedge_handle(eh, 1);

    auto fh0 = m.face_handle(heh0);
    auto fh1 = m.face_handle(heh1);

    if (!fh0.is_valid() || !fh1.is_valid())
        return false;

    auto v0 = m.to_vertex_handle(heh0);
    auto v1 = m.to_vertex_handle(m.opposite_halfedge_handle(heh0));
    auto v2 = m.to_vertex_handle(m.next_halfedge_handle(heh0));
    auto v3 = m.to_vertex_handle(m.next_halfedge_handle(heh1));

    OpenMesh::Vec3f p0 = m.point(v0);
    OpenMesh::Vec3f p1 = m.point(v1);
    OpenMesh::Vec3f p2 = m.point(v2);
    OpenMesh::Vec3f p3 = m.point(v3);

    float area1 = ((p0 - p2) % (p1 - p2)).norm() * 0.5f;
    float area2 = ((p1 - p3) % (p2 - p3)).norm() * 0.5f;
    

    if (area1 < 1e-6f || area2 < 1e-6f)
        return false;
    return true;
}

std::vector<OpenMesh::EdgeHandle> get_non_flippableedges(HEMesh &m)
{
    std::vector<OpenMesh::EdgeHandle> edges;

    for (auto e : m.edges())
    {
        if (!is_edge_flip_ok(m, e))
            edges.push_back(e);
    }
}