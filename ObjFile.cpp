#include "ObjFile.h"
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

//bool ObjFile::ReadFile()
//{
//	//vertexSize = 0;
//	//temp_vertices.resize(vertexSize);
//	//uvSize = 0;
//	//temp_uvs.resize(uvSize);
//	//normalSize = 0;
//	//temp_normals.resize(normalSize);
//
//	if (file == NULL) {
//		return false;
//	}
//	while (true)
//	{
//		char lineHeader[128];
//		// çsÇÃç≈èâÇÃï∂éöóÒÇì«Ç›çûÇ›Ç‹Ç∑ÅB
//		int res = fscanf_s(file, "%s", lineHeader, _countof(lineHeader));
//		if (res == EOF)	break;
//
//		if (strcmp(lineHeader, "v") == 0) {
//			//vertexSize++;
//			Vector3D vertex;
//			fscanf_s(file, "%f %f %fn", &vertex.x, &vertex.y, &vertex.z);
//			//temp_vertices.resize(vertexSize);
//			temp_poss.push_back(vertex);
//		}
//		else if (strcmp(lineHeader, "vt") == 0) {
//			Vector2D uv;
//			//uvSize++;
//			fscanf_s(file, "%f %fn", &uv.x, &uv.y);
//			//temp_uvs.resize(uvSize);
//			temp_uvs.push_back(uv);
//		}
//		else if (strcmp(lineHeader, "vn") == 0) {
//			Vector3D normal;
//			//normalSize++;
//			fscanf_s(file, "%f %f %fn", &normal.x, &normal.y, &normal.z);
//			//temp_normals.resize(normalSize);
//			temp_normals.push_back(normal);
//		}
//		else if (strcmp(lineHeader, "f") == 0) {
//			std::string vertex1, vertex2, vertex3;
//			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
//			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%dn", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
//			if (matches != 9) {
//				return false;
//			}
//			vertexIndices.push_back(vertexIndex[0]);
//			vertexIndices.push_back(vertexIndex[1]);
//			vertexIndices.push_back(vertexIndex[2]);
//			uvIndices.push_back(uvIndex[0]);
//			uvIndices.push_back(uvIndex[1]);
//			uvIndices.push_back(uvIndex[2]);
//			normalIndices.push_back(normalIndex[0]);
//			normalIndices.push_back(normalIndex[1]);
//			normalIndices.push_back(normalIndex[2]);
//		}
//	}
//	return true;
//}
//
//ObjFile::ObjFile(const char* filename, std::vector<Vertex>& out_vertices)
//{
//	fopen_s(&file, filename, "r");
//	
//	if (ReadFile()) {
//		out_vertices.resize(vertexIndices.size());
//		//for (size_t i = 0; i < out_vertices.size(); i++) {
//		//	out_vertices[i].pos = temp_vertices[i + 1];
//		//}
//		for (size_t i = 0; i < vertexIndices.size(); i++)
//		{
//			unsigned int vertexIndex = vertexIndices[i];
//			out_vertices[i].pos = temp_poss[vertexIndex - 1];
//			unsigned int uvIndex = uvIndices[i];
//			out_vertices[i].uv = temp_uvs[uvIndex - 1];
//			out_vertices[i].uv.y = 1.0f - out_vertices[i].uv.y;
//			unsigned int normalIndex = normalIndices[i];
//			out_vertices[i].normal = temp_normals[normalIndex - 1];
//		}
//		//for (size_t i = 0; i < uvIndices.size(); i++) {
//		//	unsigned int uvIndex = uvIndices[i];
//		//	Vector2D uv = temp_uvs[uvIndex - 1];
//		//}
//		//for (size_t i = 0; i < normalIndices.size(); i++) {
//		//	unsigned int normalIndex = normalIndices[i];
//		//	Vector3D normal = temp_normals[normalIndex - 1];
//		//}
//		
//		//index.resize(vertexIndices.size());
//		//index = vertexIndices;
//		//for (size_t i = 0; i < index.size(); i++)
//		//{
//		//	index[i] -= 1;
//		//}
//	}
//	fclose(file);
//}

void ObjFile::LoadMaterial(const std::string& directoryPath, const std::string& filename, Material& mtl)
{
	std::ifstream file;
	file.open(directoryPath + filename);
	if (file.fail()) { assert(0); }

	string line;
	while (getline(file, line))
	{
		std::istringstream line_stream(line);

		string key;
		getline(line_stream, key, ' ');

		if (key[0] == '\t') {
			key.erase(key.begin());
		}

		if (key == "newmtl") {
			line_stream >> mtl.name;
		}

		if (key == "Ka") {
			line_stream >> mtl.ambient.x;
			line_stream >> mtl.ambient.y;
			line_stream >> mtl.ambient.z;
		}

		if (key == "Kd") {
			line_stream >> mtl.diffuse.x;
			line_stream >> mtl.diffuse.y;
			line_stream >> mtl.diffuse.z;
		}

		if (key == "Ks") {
			line_stream >> mtl.specular.x;
			line_stream >> mtl.specular.y;
			line_stream >> mtl.specular.z;
		}

		if (key == "map_Kd") {
			line_stream >> mtl.textureFilename;

			string filepath = directoryPath + mtl.textureFilename;
			int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, mtl.wfilepath, _countof(mtl.wfilepath));
		}
	}
	file.close();
}

void ObjFile::AddSmoothData(unsigned short index, unsigned short vertex)
{
	smoothData[index].emplace_back(vertex);
}

void ObjFile::CalcSmoothedVertexNormals(std::vector<Vertex>& vertices)
{
	auto itr = smoothData.begin();
	for (; itr != smoothData.end(); ++itr) {
		std::vector<unsigned short>& v = itr->second;
		Vector3D normal;
		for (unsigned short index : v) {
			normal += vertices[index].normal;
		}
		normal /= (float)v.size();
		normal.normalize();
		for (unsigned short index : v) {
			vertices[index].normal = normal;
		}
	}
}

ObjFile::ObjFile(const string modelname, std::vector<Vertex>& out_vertices, Material& mtl, bool smoothing)
{
	std::vector<unsigned short> vertexIndices, uvIndices, normalIndices;
	std::vector<Vector3D> temp_poss;
	std::vector<Vector2D> temp_uvs;
	std::vector<Vector3D> temp_normals;

	std::ifstream file_;
	const string filename = modelname + ".obj";
	const string directoryPath = "Resources/Model/" + modelname + "/";
	file_.open(directoryPath + filename);
	assert(!file_.fail());

	string line;
	while (getline(file_, line)) {
		istringstream line_stream(line);

		string key;
		getline(line_stream, key, ' ');

		if (key == "mtllib") {
			string filename;
			line_stream >> filename;
			LoadMaterial(directoryPath, filename, mtl);
		}

		if (key == "v") {
			Vector3D pos;
			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> pos.z;

			temp_poss.emplace_back(pos);
		}

		if (key == "vt") {
			Vector2D uv;
			line_stream >> uv.x;
			line_stream >> uv.y;
			uv.y = 1.0f - uv.y;

			temp_uvs.emplace_back(uv);
		}

		if (key == "vn") {
			Vector3D normal;
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;

			temp_normals.emplace_back(normal);
		}

		if (key == "f") {
			string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				std::istringstream index_stream(index_string);
				unsigned short indexPos, indexNormal, indexUV;
				index_stream >> indexPos;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexUV;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;

				Vertex vertex{};
				vertex.pos = temp_poss[indexPos - 1];
				vertex.normal = temp_normals[indexNormal - 1];
				vertex.uv = temp_uvs[indexUV - 1];

				out_vertices.emplace_back(vertex);

				if (smoothing) {
					AddSmoothData(indexPos, (unsigned short)out_vertices.size() - 1);
				}
			}
		}
	}

	file_.close();

	if (smoothing) {
		CalcSmoothedVertexNormals(out_vertices);
	}
}
