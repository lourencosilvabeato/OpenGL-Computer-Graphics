// This source code is property of the Computer Graphics and Visualization 
// chair of the TU Dresden. Do not distribute! 
// Copyright (C) CGV TU Dresden - All Rights Reserved

#include <iostream>
#include <set>
#include <unordered_map>
#include <memory>
#include "Valence.h"


VertexValenceProperties AddValenceProperties(HEMesh &m)
{

	VertexValenceProperties properties;

	m.add_property(properties.faceValences, "faceValences");
	m.add_property(properties.vextexValences, "vertexValences");

	std::cerr << "Adding the custom valence properties is not implemented." << std::endl;

	return properties;
}


// Compute a histogram for the given vertex valence property
ValenceHistogram ComputeValenceHistogram (const HEMesh &m, const VertexValenceProperty valence)
{
	ValenceHistogram ret;
    for (auto vertex : m.vertices()) {
        int val = m.property(valence, vertex);
        ret[val]++;
    }
	return ret;
}

// Computes the per-vertex face incidence count (aka. vertex face valences) for the given mesh,
// using the indicated property to store the results
void ComputeVertexFaceValences (HEMesh &m, const VertexValenceProperty valence)
{
	for (auto vertex : m.vertices()) {
		int count = 0;
    	for (auto face : vertex.faces()) {
			count++;
        }
		m.property(valence, vertex) = count;
    }
	std::cerr << "Face-based valence computation is not implemented." << std::endl;
}

// Computes the vertex valences for the given mesh, using the indicated property to store the results
void ComputeVertexVertexValences(HEMesh &m, const VertexValenceProperty valence)
{
	using SetOfVertices = std::set<OpenMesh::VertexHandle>;
	using VertexAdjacencyMap = std::unordered_map<OpenMesh::VertexHandle, SetOfVertices>;

	VertexAdjacencyMap adj;

	for (auto f : m.faces()) {
		for (auto e : f.edges()) {
			auto v0 = e.v0();
			auto v1 = e.v1();

			adj[v0].insert(v1);
			adj[v1].insert(v0);
		}
	}

	for (auto v : m.vertices()) {
		m.property(valence, v) = adj[v].size();
	}
}
