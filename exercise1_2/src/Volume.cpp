// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute! 
// Copyright (C) CGV TU Dresden - All Rights Reserved

#include "Volume.h"

#include <iostream>

float ComputeVolume(const HEMesh& m)
{
    float volume = 0;

    for (auto face : m.faces()) {
        std::vector<OpenMesh::Vec3f> verts;
        for (auto v : face.vertices()){
            verts.push_back(m.point(v));
        }

		OpenMesh::Vec3f p0 = verts[0];

        for (size_t i = 1; i + 1 < verts.size(); ++i){
            OpenMesh::Vec3f p1 = verts[i];
            OpenMesh::Vec3f p2 = verts[i + 1];

            volume += fabs((p0 | (p1 % p2)) / 6.0f);
        }
    }

	std::cerr << "Volume computation is not implemented." << std::endl;
    return volume;
}
