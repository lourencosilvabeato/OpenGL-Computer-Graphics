// This source code is property of the Computer Graphics and Visualization
// chair of the TU Dresden. Do not distribute!
// Copyright (C) CGV TU Dresden - All Rights Reserved

#include <map>
#include <queue>
#include "util/UnionFind.h"
#include "ShellExtraction.h"

unsigned int ExtractShells(HEMesh &m, OpenMesh::FPropHandleT<int> perFaceShellIndex)
{
	for (auto f : m.faces())
		m.property(perFaceShellIndex, f) = -1;

	int current_shell = 0;
	std::queue<OpenMesh::FaceHandle> Q;

	for (auto f : m.faces())
	{
		if (m.property(perFaceShellIndex, f) == -1)
		{
			m.property(perFaceShellIndex, f) = current_shell;
			Q.push(f);

			while (!Q.empty())
			{
				auto cur = Q.front();
				Q.pop();

				for (auto h : m.fh_range(cur))
				{
					auto neigh = m.opposite_face_handle(h);

					if (!neigh.is_valid())
						continue;

					if (m.property(perFaceShellIndex, neigh) == -1)
					{
						m.property(perFaceShellIndex, neigh) = current_shell;
						Q.push(neigh);
					}
				}
			}

			current_shell++;
		}
	}

	return current_shell;
}
