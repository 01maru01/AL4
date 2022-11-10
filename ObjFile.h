#pragma once
#include <vector>
#include "MyMath.h"
#include "VertBuff.h"
#include "Material.h"

class ObjFile
{
private:
	//bool ReadFile();
	void LoadMaterial(const std::string& directoryPath, const std::string& filename, Material& mtl);
public:
	ObjFile(const std::string modelname, std::vector<Vertex>& out_vertices, Material& mtl);
};

