// This source code is property of the Computer Graphics and Visualization
// chair of the TU Dresden. Do not distribute!
// Copyright (C) CGV TU Dresden - All Rights Reserved

#include "Stripification.h"

#include <random>
#include "sample_set.h"

unsigned int ExtractTriStrips(HEMesh &mesh, OpenMesh::FPropHandleT<int> perFaceStripIdProperty, unsigned int nTrials)
{
	std::mt19937 eng;

	for (auto f : mesh.faces())
		mesh.property(perFaceStripIdProperty, f) = -1;

	int nStrips = 0;

	std::vector<OpenMesh::FaceHandle> triangles;
	sample_set<int> candidate_faces;
	for (auto f : mesh.faces())
	{
		triangles.push_back(f);
		candidate_faces.insert(triangles.size() - 1);
	}

	auto forward = [&](OpenMesh::HalfedgeHandle hi, int p) -> std::pair<OpenMesh::HalfedgeHandle, int>
    {
        if (p == 0)
           return {mesh.prev_halfedge_handle(mesh.opposite_halfedge_handle(hi)), 1};
        else
            return {mesh.next_halfedge_handle(mesh.opposite_halfedge_handle(hi)), 0};
    };

    auto backward = [&](OpenMesh::HalfedgeHandle hi, int p) -> std::pair<OpenMesh::HalfedgeHandle, int>
    {
        if (p == 0)
            return {mesh.opposite_halfedge_handle(mesh.prev_halfedge_handle(hi)), 1};
        else
            return {mesh.opposite_halfedge_handle(mesh.next_halfedge_handle(hi)), 0};
    };

	auto build_strip = [&](OpenMesh::HalfedgeHandle start_hi, int start_p, bool forward_dir)
	{
		std::vector<OpenMesh::FaceHandle> strip;
		std::set<OpenMesh::FaceHandle> visited;

		OpenMesh::HalfedgeHandle hi = start_hi;
		int p = start_p;

		while (true)
		{
			auto f = mesh.face_handle(hi);
			if (!f.is_valid() || mesh.property(perFaceStripIdProperty, f) != -1 || visited.count(f))
				break;

			strip.push_back(f);
			visited.insert(f);
            
            std::pair<OpenMesh::HalfedgeHandle,int> next;
            if (forward_dir)
                next = forward(hi, p);
            else
                next = backward(hi, p);

            
			hi = next.first;
			p = next.second;
		}

		return strip;
	};

	while (!candidate_faces.empty())
	{
		unsigned int best_len = 0;
		OpenMesh::HalfedgeHandle best_hi;
		int best_p = 0;

		for (unsigned int t = 0; t < nTrials; ++t)
		{
			int sample_idx = candidate_faces.sample(eng);
			OpenMesh::FaceHandle seed_face = triangles[sample_idx];
			OpenMesh::HalfedgeHandle hi = *mesh.fh_range(seed_face).begin();
			int p = 0;

			auto strip_fwd = build_strip(hi, p, true);
			auto strip_bwd = build_strip(hi, p, false);

			if (!strip_bwd.empty())
				strip_bwd.pop_back();
			std::reverse(strip_bwd.begin(), strip_bwd.end());
			strip_bwd.insert(strip_bwd.end(), strip_fwd.begin(), strip_fwd.end());

			if (strip_bwd.size() > best_len)
			{
				best_len = strip_bwd.size();
				best_hi = hi;
				best_p = p;
			}
		}

		if (best_len == 0)
			break;

		auto final_strip = build_strip(best_hi, best_p, true);
		auto final_strip_bwd = build_strip(best_hi, best_p, false);
		if (!final_strip_bwd.empty())
			final_strip_bwd.pop_back();
		std::reverse(final_strip_bwd.begin(), final_strip_bwd.end());
		final_strip_bwd.insert(final_strip_bwd.end(), final_strip.begin(), final_strip.end());

		for (auto f : final_strip_bwd)
		{
			mesh.property(perFaceStripIdProperty, f) = nStrips;

			for (unsigned int i = 0; i < triangles.size(); ++i)
				if (triangles[i] == f)
					candidate_faces.remove(i);
		}

		++nStrips;
	}

	return nStrips;
}
