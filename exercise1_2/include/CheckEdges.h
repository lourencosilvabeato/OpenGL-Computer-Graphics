// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute! 
// Copyright (C) CGV TU Dresden - All Rights Reserved

#pragma once

#include "util/OpenMeshUtils.h"

bool is_edge_flip_ok(HEMesh &m, OpenMesh::EdgeHandle eh);

std::vector<OpenMesh::EdgeHandle> get_non_flippableedges(HEMesh &m);