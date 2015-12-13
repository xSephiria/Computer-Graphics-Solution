#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"

/******************************************************************************/
/*!
Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string &meshName, Color color);
	static Mesh* GenerateCube(const std::string &meshName, Color color);
	static Mesh* GenerateCircle(const std::string &meshName, Color color, unsigned numSlices);
	static Mesh* GenerateRing(const std::string &meshName, Color color, float innerRadius, unsigned numSlices);
	static Mesh* GenerateSphere(const std::string &meshName, Color color, unsigned numStacks, unsigned numSlices);
	static Mesh* GenerateDiamond(const std::string &meshName, Color color, unsigned numStacks, unsigned numSlices);
	static Mesh* GenerateHemiSphere(const std::string &meshName, Color color, unsigned numStacks, unsigned numSlices);
	static Mesh* GenerateCone(const std::string &meshName, Color color, unsigned numSlices, unsigned bottomRad, unsigned height);
	static Mesh* GenerateCylinder(const std::string& meshName, Color color, unsigned numSlices);
};

#endif