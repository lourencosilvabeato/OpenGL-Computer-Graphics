// This source code is property of the Computer Graphics and Visualization
// chair of the TU Dresden. Do not distribute!
// Copyright (C) CGV TU Dresden - All Rights Reserved

#include "SurfaceArea.h"

#include <iostream>

float ComputeSurfaceArea(const HEMesh &m)
{
	float area = 0;

	for (auto face : m.faces())
	{
		std::vector<OpenMesh::Vec3f> verts;
		for (auto v : face.vertices())
		{
			verts.push_back(m.point(v));
		}
        for (size_t i = 1; i + 1 < verts.size(); i++)
        {
            OpenMesh::Vec3f u = verts[i] - verts[0];
            OpenMesh::Vec3f v = verts[i + 1] - verts[0];
            float tri_area = 0.5f * (u % v).norm();
            area += tri_area;
        }
	}

	std::cerr << "Area computation is not implemented." << std::endl;
	return area;
}