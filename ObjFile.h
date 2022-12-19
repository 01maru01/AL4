#pragma once
#include <vector>
#include "MyMath.h"
#include "VertBuff.h"
#include "Material.h"
#include <unordered_map>

class ObjFile
{
private:
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;

	//bool ReadFile();
	void LoadMaterial(const std::string& directoryPath, const std::string& filename, Material& mtl);
	void AddSmoothData(unsigned short index, unsigned short vectex);
	void CalcSmoothedVertexNormals(std::vector<Vertex>& vertices);
public:
	ObjFile(const std::string modelname, std::vector<Vertex>& out_vertices, Material& mtl, bool smoothing);
};

