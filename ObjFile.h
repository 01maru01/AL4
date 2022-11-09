#pragma once
#include <vector>
#include "MyMath.h"
#include "VertBuff.h"

class ObjFile
{
private:
	//bool ReadFile();
public:
	ObjFile(const char* filename, std::vector<Vertex>& out_vertices);
};

