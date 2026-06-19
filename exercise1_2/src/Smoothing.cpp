// This source code is property of the Computer Graphics and Visualization
// chair of the TU Dresden. Do not distribute!
// Copyright (C) CGV TU Dresden - All Rights Reserved

#include <memory>
#include <random>
#include "Smoothing.h"

void SmoothUniformLaplacian(HEMesh &m, float lambda)
{
	std::vector<OpenMesh::Vec3f> oldPositions(m.n_vertices());
	for (auto v : m.vertices())
		oldPositions[v.idx()] = m.point(v);

	for (auto v : m.vertices())
	{
		if (m.is_boundary(v))
			continue;

		OpenMesh::Vec3f laplacian(0, 0, 0);
		int n = 0;

		for (auto vv_it = m.vv_begin(v); vv_it.is_valid(); ++vv_it)
		{
			laplacian += oldPositions[vv_it->idx()] - oldPositions[v.idx()];
			++n;
		}
		if (n > 0)
			laplacian /= n;

		m.set_point(v, oldPositions[v.idx()] + lambda * laplacian);
	}
}

float Cotangent(const OpenMesh::Vec3f &a, const OpenMesh::Vec3f &b, const OpenMesh::Vec3f &c)
{
	OpenMesh::Vec3f ab = b - a;
	OpenMesh::Vec3f ac = c - a;
	float cos = (ab | ac) / (ab.norm() * ac.norm());
	float sin = ((ab % ac).norm()) / (ab.norm() * ac.norm());
	if (sin == 0.0f)
		return 0.0f;
	return cos / sin;
}

void SmoothCotanLaplacian(HEMesh &m, float lambda)
{
	std::vector<OpenMesh::Vec3f> positions(m.n_vertices());

	for (auto vh : m.vertices())
	{
		if (m.is_boundary(vh))
		{
			positions[vh.idx()] = m.point(vh);
			continue;
		}

		OpenMesh::Vec3f laplace(0, 0, 0);
		float weight = 0.0f;

		for (auto vv_it = m.vv_begin(vh); vv_it.is_valid(); ++vv_it)
		{
			auto neighbor = *vv_it;
			float cot = 0.0f;

			for (auto vf_it = m.vf_begin(vh); vf_it.is_valid(); ++vf_it)
			{
				auto f = *vf_it;

				bool hasNeighbor = false;
				OpenMesh::VertexHandle third;
				for (auto fv_it = m.fv_begin(f); fv_it.is_valid(); ++fv_it)
				{
					auto v = *fv_it;
					if (v == neighbor)
						hasNeighbor = true;
					if (v != vh && v != neighbor)
						third = v;
				}

				if (!hasNeighbor)
					continue;

				OpenMesh::Vec3f a = m.point(vh);
				OpenMesh::Vec3f b = m.point(neighbor);
				OpenMesh::Vec3f c = m.point(third);

				cot += Cotangent(b, a, c);
			}

			laplace += cot * (m.point(neighbor) - m.point(vh));
			weight += cot;
		}

		if (weight > 0)
			laplace /= weight;

		positions[vh.idx()] = m.point(vh) + lambda * laplace;
	}

	for (auto vh : m.vertices())
		m.set_point(vh, positions[vh.idx()]);
}

void AddNoise(HEMesh &m, OpenMesh::MPropHandleT<Viewer::BBoxType> bbox_prop)
{
	std::mt19937 rnd;
	std::normal_distribution<float> dist;

	for (auto v : m.vertices())
	{
		OpenMesh::Vec3f n;
		m.calc_vertex_normal_correct(v, n);
		const auto diag = m.property(bbox_prop).diagonal();
		const float base_diag = std::min(diag.x(), std::min(diag.y(), diag.z())) / 20.f;
		float base_nb = 0, nb_num = 0;
		for (auto vnb : m.vv_range(v))
		{
			base_nb += (m.point(v) - m.point(vnb)).norm();
			nb_num++;
		}
		base_nb /= 4.f * nb_num;

		m.point(v) += std::min(base_diag, base_nb) * dist(rnd) * n.normalized();
	}
}
